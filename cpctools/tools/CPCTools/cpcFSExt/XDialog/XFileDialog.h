// XFileDialog.h  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based partially on Paul DiLascia's Aug 2000 MSDN Magazine article
//     "Windows 2000 File Dialog Revisited"
//     http://msdn.microsoft.com/msdnmag/issues/0800/c/
//
// This software is released into the public domain.  You are free to use it
// in any way you like.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this software
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XFILEDIALOG_H
#define XFILEDIALOG_H

#include "XHistoryCombo.h"

// Windows 2000 version of OPENFILENAME.
// The new version has three extra members.
// This is copied from commdlg.h
//
#ifndef XFOLDERDIALOG_H
struct OPENFILENAMEEX : public OPENFILENAME
{
	void *	pvReserved;
	DWORD	dwReserved;
	DWORD	FlagsEx;
};
#endif

/////////////////////////////////////////////////////////////////////////////
// CXFileDialog: Encapsulate Windows-2000 style open dialog.
//
class CXFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CXFileDialog)

// Construction
public:
	CXFileDialog(BOOL bOpenFileDialog, // TRUE for Open, FALSE for Save As
				 LPCTSTR lpszDefExt = NULL,
				 LPCTSTR lpszFileName = NULL,
				 DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				 LPCTSTR lpszFilter = NULL,
				 CWnd* pParentWnd = NULL);

// Dialog Data
	//{{AFX_DATA(CXFileDialog)
	CHistoryCombo	m_cmbRecentFolders;
	//CHistoryCombo m_cmbCombo1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFileDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Operations
public:
	// override
	virtual int DoModal();

// Attributes
public:
	enum XFILEDIALOG_OS_VERSION
	{
		XFILEDIALOG_AUTO_DETECT_OS_VERSION = 0,
		XFILEDIALOG_OS_VERSION_4,
		XFILEDIALOG_OS_VERSION_5
	};
	CString GetPath();
	enum XFILEDIALOG_OS_VERSION GetOsVersion() { return m_eOsVersion; }
	void SetOsVersion(enum XFILEDIALOG_OS_VERSION eOsVersion) { m_eOsVersion = eOsVersion; }
	void SetTitle(LPCTSTR lpszTitle) { m_strTitle = lpszTitle; }

// Implementation
protected:
	OPENFILENAMEEX	m_ofnEx;	// new Windows 2000 version of OPENFILENAME
	UINT			m_nIdFileNameStatic;
	UINT			m_nIdFileNameCombo;
	UINT			m_nIdFilesOfTypeStatic;
	UINT			m_nIdFilesOfTypeCombo;
	UINT			m_nIdReadOnly;
	int				m_nComboLeftMargin;
	CString			m_strTitle;
	DWORD			m_dwFlags;

	enum XFILEDIALOG_OS_VERSION m_eOsVersion;

	BOOL IsWin2000();
	virtual BOOL OnFileNameOK();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelendokMruCombo();
	//}}AFX_MSG
};

#endif //XFILEDIALOG_H
