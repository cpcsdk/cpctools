// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "cpcfsext.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( CProgressDialog, CDialog )

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent),
	_hasAbort(false)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	_progressText = _T("");
	//}}AFX_DATA_INIT
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Text(pDX, IDC_PROGRESS_TXT, _progressText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	ON_BN_CLICKED(IDC_ABORT, OnAbort)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CProgressDialog::SetProgressText(const CString &str)
{
	_progressText = str;
	GetDlgItem(IDC_PROGRESS_TXT)->SetWindowText(str);
}

void CProgressDialog::SetLongProgressText(const CString &str)
{
	_progressText = str;
	GetDlgItem(IDC_LONG_PROGRESS_TXT)->SetWindowText(str);
}

void CProgressDialog::OnAbort() 
{
	_hasAbort = true;
	GetDlgItem(IDC_ABORT)->EnableWindow(FALSE);
}

bool CProgressDialog::HasAbort() const
{
	return _hasAbort;
}

void CProgressDialog::OnClose() 
{
	_hasAbort = true;

	//CDialog::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers

IMPLEMENT_DYNCREATE(CProgressDialogThread, CWinThread)

CProgressDialogThread::CProgressDialogThread(CRuntimeClass* pClass, UINT id) :
m_pClass(pClass),
m_ID(id),
m_pDialog(NULL)
{
}

CProgressDialogThread::~CProgressDialogThread()
{
}

BOOL CProgressDialogThread::InitInstance()
{
	CWinThread::InitInstance();
	
	if ( m_pClass != NULL)
	{
		CObject* pObject = m_pClass->CreateObject();

		if ( pObject->IsKindOf( RUNTIME_CLASS( CProgressDialog ) ) )
		{
			//CWnd	desktop;
			//desktop.Attach( ::GetDesktopWindow() );

			m_pDialog = ( CProgressDialog* )pObject;
			m_pDialog->Create( m_ID, NULL );
		
			m_pMainWnd = m_pDialog;
			m_pActiveWnd = m_pDialog;
			m_pMainWnd->ShowWindow( SW_SHOW );
			m_pMainWnd->UpdateWindow();

			//desktop.Detach();
		}
	}

	return TRUE;
}

int CProgressDialogThread::ExitInstance()
{
	if ( m_pDialog != NULL)
	{
		m_pDialog->ShowWindow(SW_HIDE);
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}

	return CWinThread::ExitInstance();
}

CProgressDialog* CProgressDialogThread::GetProgressDialog() const
{
	return m_pDialog;
}

void CProgressDialogThread::Quit()
{
	
	//PostThreadMessage()
}

BEGIN_MESSAGE_MAP(CProgressDialogThread, CWinThread)
END_MESSAGE_MAP()
