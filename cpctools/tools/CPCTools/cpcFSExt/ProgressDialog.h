#if !defined(AFX_PROGRESSDIALOG_H__EEC9442C_AF85_4D7C_B4F9_D19431222063__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__EEC9442C_AF85_4D7C_B4F9_D19431222063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

class CProgressDialog : public CDialog
{
	DECLARE_DYNCREATE( CProgressDialog )
	
// Construction
public:
	CProgressDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_PROGRESS };
	CString	_progressText;
	//}}AFX_DATA

public:
	void SetProgressText(const CString &str);
	void SetLongProgressText(const CString &str);

	bool HasAbort() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	bool	_hasAbort;

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
	afx_msg void OnAbort();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CProgressDialogThread : public CWinThread
{
	DECLARE_DYNCREATE(CProgressDialogThread)

private:
	CRuntimeClass* m_pClass;
	UINT m_ID;

protected:
	CProgressDialog* m_pDialog;

public:
	CProgressDialogThread( CRuntimeClass* pClass = NULL, UINT id = 0 );
	virtual ~CProgressDialogThread();

	CProgressDialog* GetProgressDialog() const;

	void Quit();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__EEC9442C_AF85_4D7C_B4F9_D19431222063__INCLUDED_)

