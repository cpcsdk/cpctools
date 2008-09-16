// cpcFSShellExt.cpp : Implementation of CcpcFSShellExt
#include "stdafx.h"
#include "CpcFSExt.h"
#include "cpcFSShellExt.h"

#include "resource.h"

#include "XDialog\XFileDialog.h"
#include "XDialog\XFolderDialog.h"
#include "EditBinaryHeaderDlg.h"
#include "CatalogueDlg.h"
#include "CPCBoosterDlg.h"
#include "TransfertADlg.h"
#include "ProgressDialog.h"

#include "CCPCBinaryFile.h"
#include "CCPCAsciiFile.h"
#include "CAksFileTransfert.h"

#include "transfertThread.h"

#include "fileTools.h"

#include <sstream>
#include <memory>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CcpcFSShellExt

CcpcFSShellExt::CcpcFSShellExt() :
_mode(Unknown),
_directoryPath(""),
_fileList(),
_dskFilename(""),
_dskSide(0),
_dskFormat(CCPCDisc::Data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

HRESULT CcpcFSShellExt::Initialize ( 
    LPCITEMIDLIST pidlFolder,
    LPDATAOBJECT pDataObj,
    HKEY hProgID )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	if (pDataObj != NULL)
	{
		// Look for CF_HDROP data in the data object.
		if ( FAILED( pDataObj->GetData ( &fmt, &stg )))
		{
			// Nope! Return an "invalid argument" error back to Explorer.
			return E_INVALIDARG;
		}

		// Get a pointer to the actual data.
		hDrop = (HDROP) GlobalLock ( stg.hGlobal );

		// Make sure it worked.
		if ( NULL == hDrop )
		{
			return E_INVALIDARG;
		}

		// Sanity check – make sure there is at least one filename.
		UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );

		if ( 0 == uNumFiles )
		{
			GlobalUnlock ( stg.hGlobal );
			ReleaseStgMedium ( &stg );
			return E_INVALIDARG;
		}

		HRESULT hr = S_OK;

		UINT numFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0  );

		for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
		{
			TCHAR     szFile    [MAX_PATH];

			// Get the next filename.
			if ( DragQueryFile ( hDrop, uFile, szFile, MAX_PATH ) == 0)
				continue;

			bool isFloppy = (strcmp(szFile, "A:\\") == 0) || (strcmp(szFile, "B:\\") == 0);

			if ( (GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY) != 0 && !isFloppy)
			{
				_directoryList.push_back(szFile);
			}
			else
			{
				_fileList.push_back(szFile);
			}
		}

		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );
		
		if (_fileList.size() == 0 && _directoryList.size() == 0)
		{
			return E_INVALIDARG;
		}

		if (_fileList.size() > 0)
		{
			_directoryPath = _fileList[0];
			_directoryPath = _directoryPath.substr(0,_directoryPath.find_last_of('\\'));
		}
		else
		{
			_directoryPath = _directoryList[0];
		}

		if (_fileList.size() == 1)
		{
			if (CCPCDisc::IsDSK(_fileList[0]))
			{
				_dskFilename = _fileList[0];
				_fileList.clear();
				_mode = RightClickDsk;
			}
			else
			{
				_mode = RightClickFile;
			}
		}
		else
		{
			if (_fileList.size() > 1)
			{
				_mode = RightClickFiles;
				if (CCPCDisc::IsDSK(_fileList[0]))
				{
					_dskList.clear();
					_dskList.push_back(_fileList[0]);
					for (int i=1;i<_fileList.size();i++)
					{
						if (CCPCDisc::IsDSK(_fileList[i]))
						{
							_dskList.push_back(_fileList[i]);
						}
					}
					_mode = RightClickDsks;
				}
				else
				{
					_mode = RightClickFiles;
				}
			}
			else
			{
				if (_directoryList.size() == 1)
				{
					_mode = RightClickDirectory;
				}
				else
				{
					_mode = RightClickDirectories;
				}
			}
		}

		return S_OK;
	}
	else
	{
		TCHAR dir [MAX_PATH];
		SHGetPathFromIDList ( pidlFolder, dir );
		_directoryPath = dir;

		_mode = RightClickBackground;

		return ( 1 > 0 ) ? S_OK : E_INVALIDARG;
	}
}

HRESULT CcpcFSShellExt::QueryContextMenu (
    HMENU hmenu,
    UINT  uMenuIndex, 
    UINT  uidFirstCmd,
    UINT  uidLastCmd,
    UINT  uFlags )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
    if ( uFlags & CMF_DEFAULTONLY )
        return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );

    // First, create and populate a submenu.
    HMENU hSubmenu = CreatePopupMenu();
    UINT uID = uidFirstCmd;

	switch (_mode)
	{
	case RightClickFile:
	case RightClickFiles:
		{
			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Add to...") );
			string dskFile = _fileList[0];
			dskFile = dskFile.substr(0, dskFile.find_last_of('\\'));
			dskFile = dskFile.substr(dskFile.find_last_of('\\')+1, dskFile.size() - dskFile.find_last_of('\\'));
			dskFile += ".dsk";
			string message = "&Add to ";
			message += dskFile;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, message.c_str() );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Send files to A:") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Send files to CPCBooster") );
			if (_mode == RightClickFile)
			{
				InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
				if (IsBinary(_fileList[0]))
				{
					InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Edit header") );
					InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Remove header") );
				}
				else
				{
					InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Add header") );
				}
			}
			else
			{
				InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
				InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Add headers if not") );
				InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Remove headers if any") );
			}
			break;
		}
	case RightClickDsk:
		{
			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all to...") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all here") );
			string dir = _dskFilename;
			dir = dir.substr(dir.find_last_of('\\')+1, dir.size()-dir.find_last_of('\\'));
			dir = dir.substr(0,dir.find_last_of('.'));
			string message = "&Extract to ";
			message += dir;
			message += "\\..";
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T(message.c_str()) );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Send DSK to A:") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Send DSK to CPCBooster") );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Catalogue") );
			break;
		}
	case RightClickDsks:
		{
			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all to directories") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all here") );
			break;
		}
	case RightClickDirectory:
		{
			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Add all files to...") );
			string dir = _directoryList[0];
			dir = dir.substr(dir.find_last_of('\\')+1, dir.size()-dir.find_last_of('\\'));
			string message = "&Add all files to ";
			message += dir;
			message += ".dsk";
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T(message.c_str()) );
			break;
		}
	case RightClickDirectories:
		{
			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Add all files to *.dsk") );
			break;
		}
	case RightClickBackground:
		{
			string title;
			string dskFilename = _directoryPath;
			dskFilename = dskFilename.substr(dskFilename.find_last_of('\\')+1,dskFilename.size() - dskFilename.find_last_of('\\'));
			dskFilename += ".dsk";

			int idx = 0;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Create ...") );
			title = "&Create ";
			title += dskFilename;
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T(title.c_str()) );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Create ... from A:") );
			title = "&Create ";
			title += dskFilename;
			title += "  from A:";
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T(title.c_str()) );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all A: to...") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Extract all A: here") );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Catalogue A:") );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Format A:") );
			InsertMenu ( hSubmenu, idx++, MF_SEPARATOR, uID++, NULL );
			InsertMenu ( hSubmenu, idx++, MF_BYPOSITION, uID++, _T("&Wait for CPCBooster here ...") );
			break;
		}
	}

    // Insert the submenu into the ctx menu provided by Explorer.
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

    mii.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_ID;
    mii.wID = uID++;
    mii.hSubMenu = hSubmenu;
    mii.dwTypeData = _T("C&pcFS Tools");

    InsertMenuItem ( hmenu, uMenuIndex, TRUE, &mii );

    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, uID - uidFirstCmd );
}

HRESULT CcpcFSShellExt::GetCommandString (
    UINT  idCmd,
    UINT  uFlags,
    UINT* pwReserved,
    LPSTR pszName,
    UINT  cchMax )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    USES_CONVERSION;

    // Check idCmd, it must be 0 since we have only one menu item.
    if ( 0 != idCmd )
        return E_INVALIDARG;

    // If Explorer is asking for a help string, copy our string into the
    // supplied buffer.
    if ( uFlags & GCS_HELPTEXT )
        {
        LPCTSTR szText = _T("CPCFS extension help");

        if ( uFlags & GCS_UNICODE )
            {
            // We need to cast pszName to a Unicode string, and then use the
            // Unicode string copy API.
            lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
            }
        else
            {
            // Use the ANSI string copy API to return the help string.
            lstrcpynA ( pszName, T2CA(szText), cchMax );
            }

        return S_OK;
        }

    return E_INVALIDARG;
}

HRESULT CcpcFSShellExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // If lpVerb really points to a string, ignore this function call and bail out.
    if ( 0 != HIWORD( pCmdInfo->lpVerb ))
        return E_INVALIDARG;

	switch (_mode)
	{
	case RightClickFile:
	case RightClickFiles:
		{			
			// Add files to ...
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					string dskFile = _fileList[0];
					dskFile = dskFile.substr(0, dskFile.find_last_of('\\'));
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1, dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _fileList[0].substr(0, _fileList[0].find_last_of('\\')+1) + dskFile;

					CXFileDialog newDsk(TRUE,
						".dsk",
						dskFile.c_str(),
						OFN_OVERWRITEPROMPT,
						"DSK Image file (*.dsk)|*.dsk||");

					newDsk.SetTitle(_T("Add files to..."));

					if(newDsk.DoModal() == IDOK)
					{
						_dskFilename = newDsk.GetPathName();

						TransfertThread::FileCopy(_dskFilename, _dskSide, _fileList);
						break;
					}

					break;
				}
			// Add files to ??.dsk
			case 1:
				{
					string dskFile = _fileList[0];
					dskFile = dskFile.substr(0, dskFile.find_last_of('\\'));
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1, dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _fileList[0].substr(0, _fileList[0].find_last_of('\\')+1) + dskFile;
					
					_dskFilename = dskFile;

					TransfertThread::FileCopy(_dskFilename, _dskSide, _fileList);
					break;
				}
			// Send files to A:
			case 3:
				{
					CTransfertADlg transfertDlg;
					
					transfertDlg.SetTransfertMode(CTransfertADlg::WritingFile);
					
					if (transfertDlg.DoModal() == IDOK)
					{
						_dskSide = transfertDlg.GetSide();
						_dskFilename = "a:";
						TransfertThread::FileCopy(_dskFilename, _dskSide, _fileList);
					}
					break;
				}
			// Send files to Booster
			case 4:
				{
					unsigned int nbFiles = _fileList.size();

					if (nbFiles != 0)
					{
						CCPCBoosterDlg boosterDlg;
						
						boosterDlg.SetTransfertFile(_fileList);

						boosterDlg.DoModal();
					}

					break;
				}
			// Edit header / set default header
			case 6:
				{
					if (_mode == RightClickFile)
					{
						CCPCBinaryFile file;
						if (IsBinary(_fileList[0]))
						{
							file.openFile(_fileList[0], false);
						}
						else
						{
							file.openFile(_fileList[0], true);
						}
						CEditBinaryHeaderDlg editDlg;

						editDlg.SetStartValue(file.getAdress());
						editDlg.SetSizeValue(file.getSize());
						editDlg.SetExecValue(file.getExecute());

						if (editDlg.DoModal() == IDOK)
						{
							file.setAdress(editDlg.GetStartValue());
							file.setExecute(editDlg.GetExecValue());
							file.saveFile(_fileList[0], true);
						}
					}
					else
					{
						CEditBinaryHeaderDlg editDlg;

						editDlg.SetStartValue(0);
						editDlg.SetSizeValue(-1);
						editDlg.SetExecValue(0);

						if (editDlg.DoModal() != IDOK)
							break;

						unsigned int start = editDlg.GetStartValue();
						unsigned int exec = editDlg.GetExecValue();

						ostringstream str;
						str << "Add default header will set Start #" << hex << start << dec << " and Exec #" << hex << exec << dec << "\nAre you sure to add header ?";
						if (MessageBox ( pCmdInfo->hwnd, str.str().c_str() , "CPC File System", MB_ICONQUESTION| MB_OKCANCEL ) != IDOK)
							break;

						for (int f=0 ; f<_fileList.size() ; f++)
						{
							if (IsBinary(_fileList[f]))
								continue;

							CCPCBinaryFile file;
							file.openFile(_fileList[f], true);
							file.setAdress( start );
							file.setExecute( exec );
							file.saveFile(_fileList[f], true);
						}
					}
					break;
				}
			// Remove header
			case 7:
				{
					if (MessageBox ( pCmdInfo->hwnd, _T("Remove header will delete AMSDOS data\nAre you sure to remove header ?"), "CPC File System", MB_ICONQUESTION| MB_OKCANCEL ) != IDOK)
						break;

					for (int f=0 ; f<_fileList.size() ; f++)
					{
						if (!IsBinary(_fileList[f]))
							continue;

						CCPCBinaryFile file;
						file.openFile(_fileList[f], false);
						file.saveFile(_fileList[f], false);
					}
					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	case RightClickDsk:
		{
			// Extract ?.DSK to directory
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					
					string initPath = _dskFilename;
					initPath = _directoryPath.substr(0,_directoryPath.find_last_of('.'));

					CXFolderDialog dlg(initPath.c_str());

					string message = "Select directory to extract ";
					string file = _dskFilename;
					file = file.substr(file.find_last_of('\\')+1, file.size() - file.find_last_of('\\'));
					message += file;
							
					dlg.SetTitle(message.c_str());

					if(dlg.DoModal() == IDOK)
					{
						string dirPath = dlg.GetPath();

						TransfertThread::ExtractFile(_dskFilename, _dskSide, dirPath);
					}

					break;
				}
			// Extract DSK to current directory
			case 1:
				{
					TransfertThread::ExtractFile(_dskFilename, _dskSide, _directoryPath);

					break;
				}
			// Extract DSK to directory
			case 2:
				{
					_directoryPath = _dskFilename;
					_directoryPath = _directoryPath.substr(0,_directoryPath.find_last_of('.'));
					
					TransfertThread::ExtractFile(_dskFilename, _dskSide, _directoryPath);
					break;
				}
			// Send to A:
			case 4:
				{
					int startT;
					int endT;
					int nbSide;
					int format;
					bool needAdvancedMode;
					bool interlaced;

					GetDSKInfo(_dskFilename, _dskSide, startT, endT, nbSide, format, needAdvancedMode, interlaced);

					CTransfertADlg transfertDlg;

					transfertDlg.SetTransfertMode(CTransfertADlg::WritingDSK);
					transfertDlg.SetDiscDefaultFormat(startT, endT, nbSide, format, needAdvancedMode, interlaced);
					
					if (transfertDlg.DoModal() == IDOK)
					{
						_dskSide = transfertDlg.GetSide();
						string drive = "a:";
						bool extended = transfertDlg.GetExtendedMode();
						bool interlace = (!extended) ? transfertDlg.GetInterlaceMode() : false;
						int startT = transfertDlg.GetStartTrack();
						int endT = transfertDlg.GetEndTrack();
						int nbSide = transfertDlg.GetNbSide();
						TransfertThread::DSKCopy(_dskFilename, 0, drive, _dskSide, startT, endT, nbSide, extended, interlace);
					}
					break;
				}
			// Send to Booster
			case 5:
				{
					CCPCBoosterDlg boosterDlg;
					
					boosterDlg.SetTransfertFile(_dskFilename);

					boosterDlg.DoModal();

					break;
				}
			// Catalogue
			case 7:
				{
					TransfertThread::CatalogueDSK(_dskFilename, _dskSide);
		
					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	case RightClickDsks:
		{
			// Extract all DSK to directories
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					for (int i=0;i<_dskList.size();i++)
					{
						_dskFilename = _dskList[i];
						_directoryPath = _dskFilename;
						_directoryPath = _directoryPath.substr(0,_directoryPath.find_last_of('.'));

						TransfertThread::ExtractFile(_dskFilename, _dskSide, _directoryPath);
					}

					break;
				}
			// Extract all to current directory
			case 1:
				{
					for (int i=0;i<_dskList.size();i++)
					{
						_dskFilename = _dskList[i];

						TransfertThread::ExtractFile(_dskFilename, _dskSide, _directoryPath);
					}

					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	case RightClickDirectory:
		{
			// Add directory to ...DSK
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					string dskFile = _directoryList[0];
					dskFile += ".dsk";
					CXFileDialog newDsk(TRUE,
						".dsk",
						dskFile.c_str(),
						OFN_OVERWRITEPROMPT,
						"DSK Image file (*.dsk)|*.dsk||");
					
					newDsk.SetTitle(_T("Add all files to..."));

					if(newDsk.DoModal() == IDOK)
					{
						_dskFilename = newDsk.GetPathName();

						vector<string> fileList;

						FileTool::FillFileList(_directoryList[0], "*.*", fileList);
						
						TransfertThread::FileCopy(_dskFilename, _dskSide, fileList);
					}
					break;
				}
			// Add directory to ?.DSK
			case 1:
				{
					_dskFilename = _directoryList[0];
					_dskFilename += ".dsk";

					vector<string> fileList;
					FileTool::FillFileList(_directoryList[0], "*.*", fileList);
					
					TransfertThread::FileCopy(_dskFilename, _dskSide, fileList);

					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	case RightClickDirectories:
		{
			// Add all directories to DSK
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					for (int i=0 ; i<_directoryList.size() ; i++)
					{
						_dskFilename = _directoryList[i];
						_dskFilename += ".dsk";

						vector<string> fileList;
						FileTool::FillFileList(_directoryList[i], "*.*", fileList);
						
						TransfertThread::FileCopy(_dskFilename, _dskSide, fileList);
					}					

					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	case RightClickBackground:
		{
			// Create DSK
			switch ( LOWORD( pCmdInfo->lpVerb ))
			{
			case 0:
				{
					string dskFile = _directoryPath;
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1,dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _directoryPath + "\\" + dskFile;

					CXFileDialog newDsk(TRUE,
						".dsk",
						dskFile.c_str(),
						OFN_OVERWRITEPROMPT,
						"DSK Image file (*.dsk)|*.dsk||");
					
					newDsk.SetTitle(_T("Create DSK..."));

					if(newDsk.DoModal() == IDOK)
					{
						try
						{
							_dskFilename = newDsk.GetPathName();

							if (!CCPCDisc::IsDSK(_dskFilename))
							{
								CCPCDisc *disc = CCPCDisc::CreateDisc(_dskFilename,_dskFormat,_dskSide);
							
								disc->Format();
							
								disc->Close();
								delete disc;
							}
						}
						catch (tools::CException &e)
						{
							string errorMessage = e.what();
							MessageBox ( pCmdInfo->hwnd, errorMessage.c_str(), "CPC File System error", MB_ICONWARNING );
							return E_INVALIDARG;
						}
						break;
					}

					break;
				}
			// Create ?.DSK
			case 1:
				{
					string dskFile = _directoryPath;
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1,dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _directoryPath + "\\" + dskFile;

					try
					{
						if (!CCPCDisc::IsDSK(dskFile))
						{
							CCPCDisc *disc = CCPCDisc::CreateDisc(dskFile,_dskFormat,_dskSide);
						
							disc->Format();
						
							disc->Close();
							delete disc;
						}
					}
					catch (tools::CException &e)
					{
						string errorMessage = e.what();
						MessageBox ( pCmdInfo->hwnd, "Error", errorMessage.c_str(), MB_ICONINFORMATION );
						return E_INVALIDARG;
					}
					
					break;
				}
			// Create DSK from A:
			case 2:
				{
					string dskFile = _directoryPath;
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1,dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _directoryPath + "\\" + dskFile;

					CXFileDialog newDsk(TRUE,
						".dsk",
						dskFile.c_str(),
						OFN_OVERWRITEPROMPT,
						"DSK Image file (*.dsk)|*.dsk||");
					
					newDsk.SetTitle(_T("Create DSK from A:..."));

					if(newDsk.DoModal() == IDOK)
					{
						_dskFilename = newDsk.GetPathName();

						CTransfertADlg transfertDlg;

						transfertDlg.SetTransfertMode(CTransfertADlg::ReadingDSK);
						
						if (transfertDlg.DoModal() == IDOK)
						{
							_dskSide = transfertDlg.GetSide();
							string drive = "a:";
							bool extended = transfertDlg.GetExtendedMode();
							bool doubleStep = false;
							int startT = transfertDlg.GetStartTrack();
							int endT = transfertDlg.GetEndTrack();
							int nbSide = transfertDlg.GetNbSide();
							TransfertThread::DSKCopy(drive, _dskSide, _dskFilename, 0, startT, endT, nbSide, extended, doubleStep);
						}
					}
					break;
				}
			// Create ?.DSK from A:
			case 3:
				{
					string dskFile = _directoryPath;
					dskFile = dskFile.substr(dskFile.find_last_of('\\')+1,dskFile.size() - dskFile.find_last_of('\\'));
					dskFile += ".dsk";
					dskFile = _directoryPath + "\\" + dskFile;

					CTransfertADlg transfertDlg;

					transfertDlg.SetTransfertMode(CTransfertADlg::ReadingDSK);
					
					if (transfertDlg.DoModal() == IDOK)
					{
						int dskSide = transfertDlg.GetSide();
						string drive = "a:";
						bool extended = transfertDlg.GetExtendedMode();
						bool doubleStep = false;
						int startT = transfertDlg.GetStartTrack();
						int endT = transfertDlg.GetEndTrack();
						int nbSide = transfertDlg.GetNbSide();
						TransfertThread::DSKCopy(drive, dskSide, dskFile, 0, startT, endT, nbSide, extended, doubleStep);
					}

					break;
				}
			// Extract all A: to...
			case 5:
				{
					string initPath = _dskFilename;
					initPath = _directoryPath.substr(0,_directoryPath.find_last_of('.'));

					CXFolderDialog dlg(initPath.c_str());

					string message = "Select directory to extract ";
					string file = _dskFilename;
					file = file.substr(file.find_last_of('\\')+1, file.size() - file.find_last_of('\\'));
					message += file;
							
					dlg.SetTitle(message.c_str());

					if(dlg.DoModal() == IDOK)
					{
						string dirPath = dlg.GetPath();

						CTransfertADlg transfertDlg;

						transfertDlg.SetTransfertMode(CTransfertADlg::ReadingFile);
						
						if (transfertDlg.DoModal() == IDOK)
						{
							TransfertThread::ExtractFile("a:", transfertDlg.GetSide(), dirPath);
						}
					}
					break;
				}
			// Extract all A: here
			case 6:
				{
					CTransfertADlg transfertDlg;

					transfertDlg.SetTransfertMode(CTransfertADlg::ReadingFile);
					
					if (transfertDlg.DoModal() == IDOK)
					{
						TransfertThread::ExtractFile("a:", transfertDlg.GetSide(), _directoryPath);
					}
					break;
				}
			// Catalogue A:
			case 8:
				{
					CTransfertADlg transfertDlg;

					transfertDlg.SetTransfertMode(CTransfertADlg::DirectoryFloppy);
					
					if (transfertDlg.DoModal() != IDOK)
						break;

					_dskSide = transfertDlg.GetSide();
					_dskFilename = "a:";

					TransfertThread::CatalogueDSK(_dskFilename, _dskSide);
					break;
				}
			// Format A:
			case 9:
				{
					CTransfertADlg transfertDlg;

					transfertDlg.SetTransfertMode(CTransfertADlg::FormatFloppy);
					
					if (transfertDlg.DoModal() == IDOK)
					{
						TransfertThread::FormatFloppy("a:", transfertDlg.GetSide(), transfertDlg.GetNbSide(), (CCPCDisc::TDisc)transfertDlg.GetDiscFormat());
					}

					break;
				}
			// Wait booster here
			case 11:
				{
					CCPCBoosterDlg boosterDlg;
					
					boosterDlg.SetTransfertPath(_directoryPath);

					boosterDlg.DoModal();

					break;
				}
			default:
				{
					return E_INVALIDARG;
				}
			}
			return S_OK;
			break;
		}
	default:
		{
			return E_INVALIDARG;
		}
	}
}

bool CcpcFSShellExt::IsBinary(const string &file)
{
	try
	{
		return CCPCBinaryFile::isFileBinary(file);
	}
	catch (tools::CException)
	{
		return false;
	}
}

void CcpcFSShellExt::GetDSKInfo(const string &file, int side, int &startTrack, int &endTrack, int &nbSide, int &format, bool &needAvancedMode, bool &interlaced)
{
	CCPCDisc *src = CCPCDisc::OpenDisc(file, side);

	const DSK_GEOMETRY &geom = src->GetDiscGeometry();

	startTrack = 0;
	endTrack = geom.dg_cylinders - 1;
	nbSide = geom.dg_heads;
	format = (int)src->GetDiscFormat();
	needAvancedMode = src->NeedAdvancedMode();
	interlaced = src->IsInterlaced();

	src->Close();
}
