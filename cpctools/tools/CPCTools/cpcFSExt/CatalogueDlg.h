#if !defined(AFX_CATALOGUEDLG_H__C0F10CFF_4471_47F2_B219_99061D921B8C__INCLUDED_)
#define AFX_CATALOGUEDLG_H__C0F10CFF_4471_47F2_B219_99061D921B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatalogueDlg.h : header file
//

#include "Resource.h"
#include "CCPCDisc.h"

/////////////////////////////////////////////////////////////////////////////
// CCatalogueDlg dialog

class CCatalogueDlg : public CDialog
{
protected:
// Dialog Data
	//{{AFX_DATA(CCatalogueDlg)
	enum { IDD = IDD_CATALOGUE };
	CListCtrl	_catalogueListCtrl;
	CString	_catalogueStatus;
	//}}AFX_DATA

	CCPCDisc *_dsk;

// Construction
public:
	CCatalogueDlg(CWnd* pParent = NULL);   // standard constructor

	inline void SetDSK(CCPCDisc *disc) {_dsk = disc;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatalogueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCatalogueDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATALOGUEDLG_H__C0F10CFF_4471_47F2_B219_99061D921B8C__INCLUDED_)
