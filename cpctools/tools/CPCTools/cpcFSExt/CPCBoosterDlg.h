#if !defined(AFX_CPCBOOSTERDLG_H__F2738812_389E_486A_93FB_023B0ED95E63__INCLUDED_)
#define AFX_CPCBOOSTERDLG_H__F2738812_389E_486A_93FB_023B0ED95E63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <vector>
#include <string>

// CPCBoosterDlg.h : header file
//

class CAksFileTransfert;

/////////////////////////////////////////////////////////////////////////////
// CCPCBoosterDlg dialog

class CCPCBoosterDlg : public CDialog
{

protected:
	CAksFileTransfert			*_transfert;
	std::vector<std::string>	_cpcBoosterFile;
	std::string					_filepath;
	int							_nbTransfertNeed;
	int							_currentFile;
	int							_comPortNumber;

// Construction
public:
	CCPCBoosterDlg(CWnd* pParent = NULL);   // standard constructor
	~CCPCBoosterDlg();

	//! Set file to transfert to booster
	void SetTransfertFile(const std::string &file);
	//! Set file to transfert to booster
	void SetTransfertFile(const std::vector<std::string> &file);
	//! Set file path to transfert from booster
	void SetTransfertPath(const std::string &filePath);

// Dialog Data
	//{{AFX_DATA(CCPCBoosterDlg)
	enum { IDD = IDD_CPCBOOSTER_TRANSFERT };
	CButton	_buttonOK;
	CString	_comPortNumberStr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPCBoosterDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPCBoosterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeComPortNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPCBOOSTERDLG_H__F2738812_389E_486A_93FB_023B0ED95E63__INCLUDED_)
