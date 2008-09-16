// EditBinaryHeaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cpcfsext.h"
#include "EditBinaryHeaderDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBinaryHeaderDlg dialog


CEditBinaryHeaderDlg::CEditBinaryHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditBinaryHeaderDlg::IDD, pParent),
	_startValue(0),
	_sizeValue(0),
	_execValue(0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//{{AFX_DATA_INIT(CEditBinaryHeaderDlg)
	_hexaDisplay = TRUE;
	_execEdit = _T("0");
	_sizeEdit = _T("0");
	_startEdit = _T("0");
	//}}AFX_DATA_INIT
}


void CEditBinaryHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBinaryHeaderDlg)
	DDX_Check(pDX, IDC_HEXA_CHK, _hexaDisplay);
	DDX_Text(pDX, IDC_EXEC_EDIT, _execEdit);
	DDX_Text(pDX, IDC_SIZE_EDIT, _sizeEdit);
	DDX_Text(pDX, IDC_START_EDIT, _startEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBinaryHeaderDlg, CDialog)
	//{{AFX_MSG_MAP(CEditBinaryHeaderDlg)
	ON_EN_CHANGE(IDC_EXEC_EDIT, OnChangeExecEdit)
	ON_BN_CLICKED(IDC_HEXA_CHK, OnHexaChk)
	ON_EN_CHANGE(IDC_SIZE_EDIT, OnChangeSizeEdit)
	ON_EN_CHANGE(IDC_START_EDIT, OnChangeStartEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBinaryHeaderDlg message handlers

void CEditBinaryHeaderDlg::OnChangeStartEdit() 
{
	UpdateData(TRUE);

	std::string val;

	if (_hexaDisplay)
	{
		val = "0x"+_startEdit;
	}
	else
	{
		val = _startEdit;
	}
	int valInt = strtol(val.c_str(), NULL, 0);
	valInt = (valInt < 0) ? 0 : ((valInt > 65535) ? 65535 : valInt);
	SetStartValue(valInt);
	UpdateData(FALSE);
}

void CEditBinaryHeaderDlg::OnChangeSizeEdit() 
{
	UpdateData(TRUE);
	std::string val;

	if (_hexaDisplay)
	{
		val = "0x"+_sizeEdit;
	}
	else
	{
		val = _sizeEdit;
	}
	int valInt = strtol(val.c_str(), NULL, 0);
	valInt = (valInt < 0) ? 0 : ((valInt > 65535) ? 65535 : valInt);
	SetSizeValue(valInt);
	UpdateData(FALSE);
}

void CEditBinaryHeaderDlg::OnChangeExecEdit() 
{
	UpdateData(TRUE);
	std::string val;

	if (_hexaDisplay)
	{
		val = "0x"+_execEdit;
	}
	else
	{
		val = _execEdit;
	}
	int valInt = strtol(val.c_str(), NULL, 0);
	valInt = (valInt < 0) ? 0 : ((valInt > 65535) ? 65535 : valInt);
	SetExecValue(valInt);
	UpdateData(FALSE);
}

void CEditBinaryHeaderDlg::OnHexaChk() 
{
	UpdateData(TRUE);
	SetStartValue(_startValue);
	SetSizeValue(_sizeValue);
	SetExecValue(_execValue);
	UpdateData(FALSE);
}

void CEditBinaryHeaderDlg::SetStartValue(int value)
{
	_startValue = value;

	char val[256];
	if (_hexaDisplay)
	{
		sprintf(val, "%x", value);
	}
	else
	{
		sprintf(val, "%d", value);
	}
	
	_startEdit = val;
}
int CEditBinaryHeaderDlg::GetStartValue() const
{
	return _startValue;
}
void CEditBinaryHeaderDlg::SetSizeValue(int value)
{
	_sizeValue = value;

	char val[256];
	if (_hexaDisplay)
	{
		sprintf(val, "%x", value);
	}
	else
	{
		sprintf(val, "%d", value);
	}
	
	_sizeEdit = val;
}
int CEditBinaryHeaderDlg::GetSizeValue() const
{
	return _sizeValue;
}
void CEditBinaryHeaderDlg::SetExecValue(int value)
{
	_execValue = value;

	char val[256];
	if (_hexaDisplay)
	{
		sprintf(val, "%x", value);
	}
	else
	{
		sprintf(val, "%d", value);
	}
	
	_execEdit = val;
}
int CEditBinaryHeaderDlg::GetExecValue() const
{
	return _execValue;
}
