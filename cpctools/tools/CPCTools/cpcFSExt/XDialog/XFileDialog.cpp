// XFileDialog.cpp  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// Description:
//     XFileDialog implements CXFileDialog that uses the new Windows
//     2000 style open/save dialog.
//
// History
//     Version 1.1 - 2003 June 25
//     - Fixed some focus bugs;  added check for directory existence
//
//     Version 1.0 - 2003 June 24
//     - Initial public release
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

#include "stdafx.h"
#include <afxpriv.h>
#include "XFileDialog.h"
#include "XFileDialogRes.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXFileDialog

IMPLEMENT_DYNAMIC(CXFileDialog, CFileDialog)

BEGIN_MESSAGE_MAP(CXFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CXFileDialog)
	ON_WM_SIZE()
	ON_CBN_SELENDOK(IDC_MRU_COMBO, OnSelendokMruCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXFileDialog::CXFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt,
	LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
		dwFlags, lpszFilter, pParentWnd)
{
	m_dwFlags              = dwFlags;
	m_eOsVersion           = XFILEDIALOG_AUTO_DETECT_OS_VERSION;
	m_strTitle             = _T("");
	m_nIdFileNameStatic    = 1090;
	m_nIdFileNameCombo     = 1148;	// or 1152
	m_nIdFilesOfTypeStatic = 1089;
	m_nIdFilesOfTypeCombo  = 1136;
	m_nIdReadOnly          = 1040;

	SetTemplate(0, _T("IDD_XFILEDIALOG"));
}

///////////////////////////////////////////////////////////////////////////////
// DoDataExchange
void CXFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXFileDialog)
	DDX_Control(pDX, IDC_MRU_COMBO, m_cmbRecentFolders);
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
// DoModal override copied mostly from MFC, with modification to use
// m_ofnEx instead of m_ofn.
int CXFileDialog::DoModal()
{
	//TRACE(_T("in CXFileDialog::DoModal\n"));

	ASSERT_VALID(this);
	ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
	ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

	// zero out the file buffer for consistent parsing later
	ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
	DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
	ASSERT(nOffset <= m_ofn.nMaxFile);
	memset(m_ofn.lpstrFile+nOffset, 0, (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

	// WINBUG: This is a special case for the file open/save dialog,
	// which sometimes pumps while it is coming up but before it has
	// disabled the main window.
	HWND hWndFocus = ::GetFocus();
	BOOL bEnableParent = FALSE;
	m_ofn.hwndOwner = PreModal();
	AfxUnhookWindowCreate();
	if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
	{
		bEnableParent = TRUE;
		::EnableWindow(m_ofn.hwndOwner, FALSE);
	}

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	ASSERT(pThreadState->m_pAlternateWndInit == NULL);

	if (m_ofn.Flags & OFN_EXPLORER)
		pThreadState->m_pAlternateWndInit = this;
	else
		AfxHookWindowCreate(this);

	memset(&m_ofnEx, 0, sizeof(m_ofnEx));
	memcpy(&m_ofnEx, &m_ofn, sizeof(m_ofn));

	if (IsWin2000())
	{
		m_ofnEx.lStructSize = sizeof(m_ofnEx);
	}

	int nResult;
	if (m_bOpenFileDialog)
		nResult = ::GetOpenFileName((OPENFILENAME*)&m_ofnEx);
	else
		nResult = ::GetSaveFileName((OPENFILENAME*)&m_ofnEx);

	memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);

	if (nResult)
		ASSERT(pThreadState->m_pAlternateWndInit == NULL);
	pThreadState->m_pAlternateWndInit = NULL;

	// WINBUG: Second part of special case for file open/save dialog.
	if (bEnableParent)
		::EnableWindow(m_ofnEx.hwndOwner, TRUE);
	if (::IsWindow(hWndFocus))
		::SetFocus(hWndFocus);

	PostModal();

	return nResult ? nResult : IDCANCEL;
}

///////////////////////////////////////////////////////////////////////////////
// OnNotify - when the open dialog sends a notification, copy m_ofnEx to m_ofn
// in case handler function is expecting updated information in the
// OPENFILENAME struct.
//
BOOL CXFileDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);

	return CFileDialog::OnNotify( wParam, lParam, pResult);
}

///////////////////////////////////////////////////////////////////////////////
// OnFileNameOK
BOOL CXFileDialog::OnFileNameOK()
{
	//TRACE(_T("CXFileDialog::OnFileNameOK\n"));

	// save current folder to history list
	CString strFolder = GetPath();
	m_cmbRecentFolders.AddString(strFolder);
	m_cmbRecentFolders.SetWindowText(strFolder);
	m_cmbRecentFolders.SaveHistory(TRUE);

	return CFileDialog::OnFileNameOK();
}

///////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL CXFileDialog::OnInitDialog()
{
	//TRACE(_T("in CXFileDialog::OnInitDialog\n"));

	CFileDialog::OnInitDialog();

	// load folder history - check if valid folder
	m_cmbRecentFolders.SetCheckAccess(TRUE);
	m_cmbRecentFolders.LoadHistory(_T("FolderHistory"), _T("Folder"));

	CString str;
	if (m_cmbRecentFolders.GetCount() > 0)
	{
		m_cmbRecentFolders.GetLBText(0, str);
		m_cmbRecentFolders.SetWindowText(str);
	}

	// save left margin for combo boxes
	CWnd *pWndFileNameCombo = GetParent()->GetDlgItem(m_nIdFileNameCombo);
	if (!pWndFileNameCombo)
	{
		// the File Name control ID is either 1148 or 1152, depending on whether
		// it is used as an edit box (1152) or a combo box (1148).  If the OS
		// version is < 5, it is 1152; if >= 5, it is 1148.  It will also be
		// 1152 if the registry key
		//    HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\comdlg32\NoFileMru
		// is set to 1.  For convenience, we will always refer to this control
		// as a combobox.
		m_nIdFileNameCombo = 1152;
		pWndFileNameCombo = GetParent()->GetDlgItem(m_nIdFileNameCombo);
	}
	if (pWndFileNameCombo)
	{
		CRect rectFileNameCombo;
		pWndFileNameCombo->GetWindowRect(&rectFileNameCombo);
		GetParent()->ScreenToClient(&rectFileNameCombo);
		if (IsWin2000())
			m_nComboLeftMargin = rectFileNameCombo.left;
		else
			m_nComboLeftMargin = rectFileNameCombo.left + 8;
	}
	else
	{
		ASSERT(FALSE);
		m_nComboLeftMargin = 90;
	}

	// set title if specified
	if (!m_strTitle.IsEmpty())
		GetParent()->SetWindowText(m_strTitle);

	return TRUE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
// OnSize - position and size controls on default and new dialogs
void CXFileDialog::OnSize(UINT nType, int cx, int cy)
{
	CFileDialog::OnSize(nType, cx, cy);

	// original dialog box
	CWnd *pWndParent = GetParent();
	ASSERT(pWndParent);
	if (!pWndParent)
		return;

	// File name combobox
	CWnd *pWndFileNameCombo = GetParent()->GetDlgItem(m_nIdFileNameCombo);
	if (!pWndFileNameCombo)
		return;
	CRect rectFileNameCombo;
	pWndFileNameCombo->GetWindowRect(&rectFileNameCombo);
	GetParent()->ScreenToClient(&rectFileNameCombo);
	rectFileNameCombo.left = m_nComboLeftMargin;
	pWndFileNameCombo->MoveWindow(&rectFileNameCombo);

	// File name static
	CWnd *pWndFileNameStatic = GetParent()->GetDlgItem(m_nIdFileNameStatic);
	ASSERT(pWndFileNameStatic);
	CRect rectFileNameStatic;
	pWndFileNameStatic->GetWindowRect(&rectFileNameStatic);
	GetParent()->ScreenToClient(&rectFileNameStatic);
	rectFileNameStatic.right = m_nComboLeftMargin - 2;
	pWndFileNameStatic->MoveWindow(&rectFileNameStatic);

	// Files of type combobox
	CWnd *pWndFilesOfTypeCombo = GetParent()->GetDlgItem(m_nIdFilesOfTypeCombo);
	ASSERT(pWndFilesOfTypeCombo);
	CRect rectFilesOfTypeCombo;
	pWndFilesOfTypeCombo->GetWindowRect(&rectFilesOfTypeCombo);
	GetParent()->ScreenToClient(&rectFilesOfTypeCombo);
	rectFilesOfTypeCombo.left = m_nComboLeftMargin;
	pWndFilesOfTypeCombo->MoveWindow(&rectFilesOfTypeCombo);

	// Files of type static
	CWnd *pWndFilesOfTypeStatic = GetParent()->GetDlgItem(m_nIdFilesOfTypeStatic);
	ASSERT(pWndFilesOfTypeStatic);
	CRect rectFilesOfTypeStatic;
	pWndFilesOfTypeStatic->GetWindowRect(&rectFilesOfTypeStatic);
	GetParent()->ScreenToClient(&rectFilesOfTypeStatic);
	rectFilesOfTypeStatic.right = m_nComboLeftMargin - 2;
	pWndFilesOfTypeStatic->MoveWindow(&rectFilesOfTypeStatic);

	int nDeltaYCombo = rectFilesOfTypeCombo.top - rectFileNameCombo.bottom;
	int nDeltaYStatic = rectFilesOfTypeStatic.top - rectFileNameStatic.bottom;

	// Recent folders combobox
	CWnd *pWndRecentFoldersCombo = GetDlgItem(IDC_MRU_COMBO);
	ASSERT(pWndRecentFoldersCombo);
	CRect rectRecentFoldersCombo;
	pWndRecentFoldersCombo->GetWindowRect(&rectRecentFoldersCombo);
	GetParent()->ScreenToClient(&rectRecentFoldersCombo);
	rectRecentFoldersCombo.left = rectFilesOfTypeCombo.left;
	rectRecentFoldersCombo.right = rectFilesOfTypeCombo.right;
	int h = rectRecentFoldersCombo.Height();
	rectRecentFoldersCombo.top = rectFilesOfTypeCombo.bottom + nDeltaYCombo;
	rectRecentFoldersCombo.bottom = rectRecentFoldersCombo.top + h;
	pWndRecentFoldersCombo->MoveWindow(&rectRecentFoldersCombo);

	// Recent folders static
	CWnd *pWndRecentFoldersStatic = GetDlgItem(IDC_MRU_CAPTION);
	ASSERT(pWndRecentFoldersStatic);
	CRect rectRecentFoldersStatic;
	pWndRecentFoldersStatic->GetWindowRect(&rectRecentFoldersStatic);
	GetParent()->ScreenToClient(&rectRecentFoldersStatic);
	rectRecentFoldersStatic.left = rectFilesOfTypeStatic.left;
	rectRecentFoldersStatic.right = rectFilesOfTypeStatic.right;
	h = rectRecentFoldersStatic.Height();
	rectRecentFoldersStatic.top = rectFilesOfTypeStatic.bottom + nDeltaYStatic;
	rectRecentFoldersStatic.bottom = rectRecentFoldersStatic.top + h;
	pWndRecentFoldersStatic->MoveWindow(&rectRecentFoldersStatic);

	// Read only checkbox
	if ((m_dwFlags & OFN_HIDEREADONLY) == 0)
	{
		CWnd *pWndReadOnlyCheckbox = GetParent()->GetDlgItem(m_nIdReadOnly);
		ASSERT(pWndReadOnlyCheckbox);
		CRect rectReadOnlyCheckbox;
		pWndReadOnlyCheckbox->GetWindowRect(&rectReadOnlyCheckbox);
		GetParent()->ScreenToClient(&rectReadOnlyCheckbox);
		rectReadOnlyCheckbox.left = rectFilesOfTypeCombo.left;
		rectReadOnlyCheckbox.right = rectFilesOfTypeCombo.right;
		int h = rectReadOnlyCheckbox.Height();
		rectReadOnlyCheckbox.top = rectRecentFoldersCombo.bottom + nDeltaYCombo;
		rectReadOnlyCheckbox.bottom = rectReadOnlyCheckbox.top + h;
		pWndReadOnlyCheckbox->MoveWindow(&rectReadOnlyCheckbox);
	}


	///////////////////////////////////////////////////////////////////////////
	// following code is necessary to prevent having the
	// Recent folders combobox highlighted
	static BOOL bFirstTime = TRUE;
	if (bFirstTime)
	{
		bFirstTime = FALSE;
		m_cmbRecentFolders.SetFocus();
	}
	else
	{
		m_cmbRecentFolders.SetEditSel(-1, 0);
		pWndFileNameCombo->SetFocus();
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetPath
CString CXFileDialog::GetPath()
{
	CString str = _T("");

	str = GetPathName();
	if (!str.IsEmpty())
	{
		int index = str.ReverseFind(_T('\\'));
		if (index != -1)
		{
			str = str.Left(index);
		}
	}

	//TRACE(_T("str=%s\n"), str);
	return str;
}

///////////////////////////////////////////////////////////////////////////////
// OnSelendokMruCombo
void CXFileDialog::OnSelendokMruCombo()
{
	//TRACE(_T("in CXFileDialog::OnSelendokMruCombo\n"));

	if (!::IsWindow(m_cmbRecentFolders.m_hWnd))
		return;

	CString strFolder;
	int index = m_cmbRecentFolders.GetCurSel();
	if (index >= 0)
	{
		m_cmbRecentFolders.GetLBText(index, strFolder);
		if (!strFolder.IsEmpty() && (_taccess(strFolder, 00) == 0))
		{
			// change to new folder, leave file name control unchanged
			TCHAR szText[_MAX_PATH*2];
			memset(szText, 0, sizeof(szText));
			GetParent()->GetDlgItem(m_nIdFileNameCombo)->SendMessage(WM_GETTEXT,
							sizeof(szText)/sizeof(TCHAR)-1, (LPARAM)szText);
			GetParent()->SendMessage(CDM_SETCONTROLTEXT, m_nIdFileNameCombo,
							(LPARAM)(LPCTSTR)strFolder);
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED),
							(LPARAM)GetDlgItem(IDOK)->GetSafeHwnd());
			GetParent()->SendMessage(CDM_SETCONTROLTEXT, m_nIdFileNameCombo,
							(LPARAM)szText);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// IsWin2000
BOOL CXFileDialog::IsWin2000()
{
	if (GetOsVersion() == XFILEDIALOG_OS_VERSION_4)
		return FALSE;
	else if (GetOsVersion() == XFILEDIALOG_OS_VERSION_5)
		return TRUE;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// which is supported on Windows 2000.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	BOOL bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) &osvi);
	if (!bOsVersionInfoEx)
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *) &osvi))
			return FALSE;
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_NT:
			if (osvi.dwMajorVersion >= 5)
				return TRUE;
			break;
	}
	return FALSE;
}


