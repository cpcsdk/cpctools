// TransfertADlg.cpp : implementation file
//

#include "stdafx.h"
#include "cpcfsext.h"
#include "TransfertADlg.h"
#include "CRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransfertADlg dialog

#define TRANSFERTFLOPPY_PATH "HKEY_LOCAL_MACHINE/SOFTWARE/Arkos/TransfertFloppy"
#define TRANSFERTFLOPPY_DRIVE		"Drive"
#define TRANSFERTFLOPPY_SIDE		"Side"
#define TRANSFERTFLOPPY_FORMAT		"Format"
#define TRANSFERTFLOPPY_START_TRACK	"StartTrack"
#define TRANSFERTFLOPPY_END_TRACK	"EndTrack"
#define TRANSFERTFLOPPY_EXTENDED	"Extended"
#define TRANSFERTFLOPPY_DOUBLE_STEP	"DoubleStep"
#define TRANSFERTFLOPPY_DOUBLE_SIDE	"DoubleSide"

CTransfertADlg::CTransfertADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransfertADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransfertADlg)
	_discFormatCbn = 0;
	_startTrack = 0;
	_endTrack = 40;
	_extendedChk = FALSE;
	_doubleSideChk = FALSE;
	_interlacedChk = FALSE;
	//}}AFX_DATA_INIT
	
	_side = 0;
	
	getRegistryConfig();	
}


void CTransfertADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransfertADlg)
	DDX_CBIndex(pDX, IDC_FORMAT_CBN, _discFormatCbn);
	DDX_Text(pDX, IDC_START_TRACK, _startTrack);
	DDX_Text(pDX, IDC_END_TRACK, _endTrack);
	DDX_Check(pDX, IDC_EXTENDED_CHK, _extendedChk);
	DDX_Check(pDX, IDC_DOUBLE_SIDE_CHK, _doubleSideChk);
	DDX_Check(pDX, IDC_INTERLACED_CHK, _interlacedChk);
	//}}AFX_DATA_MAP

	setRegistryConfig();
}

void CTransfertADlg::getRegistryConfig()
{
	CRegistryKey key(TRANSFERTFLOPPY_PATH);
	key.GetValue(TRANSFERTFLOPPY_SIDE, _side);
	key.GetValue(TRANSFERTFLOPPY_FORMAT, _discFormatCbn);
	int val = 0;
	key.GetValue(TRANSFERTFLOPPY_START_TRACK, val);
	_startTrack = val;
	val = 42;
	key.GetValue(TRANSFERTFLOPPY_END_TRACK, val);
	_endTrack = val;
	key.GetValue(TRANSFERTFLOPPY_EXTENDED, _extendedChk);
	key.GetValue(TRANSFERTFLOPPY_DOUBLE_SIDE, _doubleSideChk);
}
void CTransfertADlg::setRegistryConfig()
{
	CRegistryKey key(TRANSFERTFLOPPY_PATH);
	key.SetValue(TRANSFERTFLOPPY_SIDE, _side);
	key.SetValue(TRANSFERTFLOPPY_FORMAT, _discFormatCbn);
	key.SetValue(TRANSFERTFLOPPY_START_TRACK, _startTrack);
	key.SetValue(TRANSFERTFLOPPY_END_TRACK, _endTrack);
	key.SetValue(TRANSFERTFLOPPY_EXTENDED, _extendedChk);
	key.SetValue(TRANSFERTFLOPPY_DOUBLE_SIDE, _doubleSideChk);
}

BEGIN_MESSAGE_MAP(CTransfertADlg, CDialog)
	//{{AFX_MSG_MAP(CTransfertADlg)
	ON_EN_CHANGE(IDC_START_TRACK, OnChangeStartTrack)
	ON_CBN_SELCHANGE(IDC_FORMAT_CBN, OnSelchangeFormatCbn)
	ON_BN_CLICKED(IDC_SIDEA, OnSidea)
	ON_BN_CLICKED(IDC_SIDEB, OnSideb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransfertADlg message handlers

void CTransfertADlg::SetTransfertMode(const CTransfertMode &mode)
{
	_mode = mode;
}

void CTransfertADlg::SetDiscDefaultFormat(int startTrack, int endTrack, int nbSide, int format, bool needAdvancedMode, bool interlaced)
{
	_startTrack = startTrack;
	_endTrack = endTrack;
	_discFormatCbn = (format - 1);
	_doubleSideChk = (nbSide == 2) ? TRUE : FALSE;
	_nbSide = nbSide;
	_extendedChk = needAdvancedMode ? TRUE : FALSE;
	_interlacedChk = (!interlaced && !needAdvancedMode) ? TRUE : FALSE;
	setRegistryConfig();
}

int CTransfertADlg::GetSide() const
{
	return _side;
}
int CTransfertADlg::GetNbSide() const
{
	return ((_doubleSideChk == TRUE) ? 2 : 1);
}
int CTransfertADlg::GetStartTrack() const
{
	return _startTrack;
}
int CTransfertADlg::GetEndTrack() const
{
	return _endTrack;
}
bool CTransfertADlg::GetExtendedMode() const
{
	return (_extendedChk == TRUE);
}
bool CTransfertADlg::GetInterlaceMode() const
{
	return (_interlacedChk == TRUE);
}
int CTransfertADlg::GetDiscFormat() const
{
	return _discFormatCbn;
}

BOOL CTransfertADlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch (_mode)
	{
	case WritingFile:
		{
			CString str;
			str.Format("Writing files");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);

			GetDlgItem(IDC_EXTENDED_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORMAT_CBN)->EnableWindow(FALSE);
			GetDlgItem(IDC_DOUBLE_SIDE_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_END_TRACK)->EnableWindow(FALSE);

			break;
		}
	case ReadingFile:
		{
			CString str;
			str.Format("Reading files");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);

			GetDlgItem(IDC_EXTENDED_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORMAT_CBN)->EnableWindow(FALSE);
			GetDlgItem(IDC_DOUBLE_SIDE_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_END_TRACK)->EnableWindow(FALSE);

			break;
		}
	case WritingDSK:
		{
			CString str;
			str.Format("Writing DSK");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);

			GetDlgItem(IDC_FORMAT_CBN)->EnableWindow(FALSE);
			if (_nbSide != 2)
			{
				GetDlgItem(IDC_DOUBLE_SIDE_CHK)->EnableWindow(FALSE);
			}
			if (_extendedChk == TRUE || _interlacedChk == FALSE)
			{
				GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);
			}

			break;
		}
	case ReadingDSK:
		{
			CString str;
			str.Format("Reading DSK");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);

			GetDlgItem(IDC_FORMAT_CBN)->EnableWindow(FALSE);
			GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);

			break;
		}
	case FormatFloppy:
		{
			CString str;
			str.Format("Formating Floppy");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);
			GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);
			break;
		}
	case DirectoryFloppy:
		{
			CString str;
			str.Format("Directory floppy");
			GetDlgItem(IDC_TRANSFERT_TXT)->SetWindowText(str);

			GetDlgItem(IDC_EXTENDED_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORMAT_CBN)->EnableWindow(FALSE);
			GetDlgItem(IDC_DOUBLE_SIDE_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_END_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_INTERLACED_CHK)->EnableWindow(FALSE);
			break;
		}
	}

	CheckRadioButton(IDC_SIDEA,IDC_SIDEB, (_side == 0) ? IDC_SIDEA : IDC_SIDEB);

	return TRUE;
}

void CTransfertADlg::OnChangeStartTrack() 
{
	UpdateData(TRUE);
	_startTrack = (_startTrack < _endTrack) ? _startTrack : _endTrack-1;
	UpdateData(FALSE);
}

void CTransfertADlg::OnSelchangeFormatCbn() 
{
	// TODO: Add your control notification handler code here
	
}

void CTransfertADlg::OnSidea() 
{
	_side = 0;
	
	setRegistryConfig();
}

void CTransfertADlg::OnSideb() 
{
	_side = 1;

	setRegistryConfig();
}
