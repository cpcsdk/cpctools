#if !defined(AFX_TRANSFERTADLG_H__AD9EEE5B_1132_4D0B_95FE_7B0904472BEA__INCLUDED_)
#define AFX_TRANSFERTADLG_H__AD9EEE5B_1132_4D0B_95FE_7B0904472BEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransfertADlg.h : header file
//

#include "resource.h"
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CTransfertADlg dialog

class CTransfertADlg : public CDialog
{
public:
	enum CTransfertMode
	{
		WritingFile,
		ReadingFile,
		WritingDSK,
		ReadingDSK,
		FormatFloppy,
		DirectoryFloppy
	};
protected:
	CTransfertMode		_mode;

	int		_nbSide;
	
// Dialog Data
	//{{AFX_DATA(CTransfertADlg)
	enum { IDD = IDD_TRANSFERT_A };
	int		_discFormatCbn;
	BOOL	_twoSideChk;
	UINT	_startTrack;
	UINT	_endTrack;
	BOOL	_extendedChk;
	BOOL	_doubleSideChk;
	BOOL	_interlacedChk;
	//}}AFX_DATA

	int _side;

// Construction
public:
	CTransfertADlg(CWnd* pParent = NULL);   // standard constructor

	void SetTransfertMode(const CTransfertMode &mode);

	void SetDiscDefaultFormat(int startTrack, int endTrack, int nbSide, int format, bool needAdvancedMode, bool interlaced);
	
	std::string GetDrive() const;
	int GetSide() const;
	int GetNbSide() const;
	int GetStartTrack() const;
	int GetEndTrack() const;
	bool GetExtendedMode() const;
	bool GetInterlaceMode() const;
	bool GetDoubleStepMode() const;
	int GetDiscFormat() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransfertADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void getRegistryConfig();
	void setRegistryConfig();

	// Generated message map functions
	//{{AFX_MSG(CTransfertADlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStartTrack();
	afx_msg void OnSelchangeFormatCbn();
	afx_msg void OnSidea();
	afx_msg void OnSideb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERTADLG_H__AD9EEE5B_1132_4D0B_95FE_7B0904472BEA__INCLUDED_)
