#if !defined(AFX_EDITBINARYHEADERDLG_H__D7B2CFF0_6A8E_4EFC_AE47_14C2E4A6301B__INCLUDED_)
#define AFX_EDITBINARYHEADERDLG_H__D7B2CFF0_6A8E_4EFC_AE47_14C2E4A6301B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBinaryHeaderDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CEditBinaryHeaderDlg dialog

class CEditBinaryHeaderDlg : public CDialog
{
protected:
// Dialog Data
	//{{AFX_DATA(CEditBinaryHeaderDlg)
	enum { IDD = IDD_EDIT_HEADER };
	BOOL	_hexaDisplay;
	CString	_execEdit;
	CString	_sizeEdit;
	CString	_startEdit;
	//}}AFX_DATA

	int _startValue;
	int _sizeValue;
	int _execValue;
	
// Construction
public:
	CEditBinaryHeaderDlg(CWnd* pParent = NULL);   // standard constructor

	void SetStartValue(int value);
	int GetStartValue() const;
	void SetSizeValue(int value);
	int GetSizeValue() const;
	void SetExecValue(int value);
	int GetExecValue() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBinaryHeaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditBinaryHeaderDlg)
	afx_msg void OnChangeExecEdit();
	afx_msg void OnHexaChk();
	afx_msg void OnChangeSizeEdit();
	afx_msg void OnChangeStartEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBINARYHEADERDLG_H__D7B2CFF0_6A8E_4EFC_AE47_14C2E4A6301B__INCLUDED_)
