// cpcFSShellExt.h : Declaration of the CcpcFSShellExt

#ifndef __CPCFSSHELLEXT_H_
#define __CPCFSSHELLEXT_H_

#pragma warning(disable : 4530)

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>
#include <vector>

#include "CCPCDisc.h"

class CProgressDialog;

/////////////////////////////////////////////////////////////////////////////
// CcpcFSShellExt
class ATL_NO_VTABLE CcpcFSShellExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CcpcFSShellExt, &CLSID_cpcFSShellExt>,
	public IDispatchImpl<IcpcFSShellExt, &IID_IcpcFSShellExt, &LIBID_CPCFSEXTLib>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CcpcFSShellExt();

DECLARE_REGISTRY_RESOURCEID(IDR_CPCFSSHELLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CcpcFSShellExt)
	COM_INTERFACE_ENTRY(IcpcFSShellExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

protected:
	enum CPCFSInterfaceMode
	{
		Unknown,
		RightClickBackground,
		RightClickFile,
		RightClickFiles,
		RightClickDsk,
		RightClickDsks,
		RightClickDirectory,
		RightClickDirectories
	};
protected:
	CPCFSInterfaceMode						_mode;

	std::string								_directoryPath;
	std::vector< std::basic_string<TCHAR> >	_fileList;
	std::vector< std::basic_string<TCHAR> >	_dskList;
	std::vector< std::basic_string<TCHAR> >	_directoryList;

	std::string								_dskFilename;
	int										_dskSide;
	CCPCDisc::TDisc							_dskFormat;

public:
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
	
protected:
	//! Check if file has binary header
	static bool IsBinary(const std::string &file);
	//! Return DSK information if DSK is a file
	static void GetDSKInfo(const std::string &file, int side, int &startTrack, int &endTrack, int &nbSide, int &format, bool &needAdvancedMode, bool &interlaced);
};

#endif //__CPCFSSHELLEXT_H_
