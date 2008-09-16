// CPCBoosterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cpcfsext.h"
#include "CPCBoosterDlg.h"
#include "CAksFileTransfert.h"
#include "CRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPCBoosterDlg dialog

#define CPCBOOSTER_PATH "HKEY_LOCAL_MACHINE/SOFTWARE/Arkos/CPCBooster"
#define CPCBOOSTER_COM_PORT "COMPort"

CCPCBoosterDlg::CCPCBoosterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPCBoosterDlg::IDD, pParent),
	_transfert(NULL),
	_nbTransfertNeed(0),
	_currentFile(0),
	_comPortNumber(0)
{
	//{{AFX_DATA_INIT(CCPCBoosterDlg)
	//}}AFX_DATA_INIT
	
	CRegistryKey key(CPCBOOSTER_PATH);
	key.GetValue(CPCBOOSTER_COM_PORT, _comPortNumber);
	
	_comPortNumberStr.Format("%d",_comPortNumber);
}

CCPCBoosterDlg::~CCPCBoosterDlg()
{
	if (_transfert != NULL)
		delete _transfert;
}

void CCPCBoosterDlg::SetTransfertFile(const std::string &file)
{
	_cpcBoosterFile.clear();
	_cpcBoosterFile.push_back(file);
	_currentFile = 0;
	_nbTransfertNeed = 1;
	_filepath = "";
}

void CCPCBoosterDlg::SetTransfertFile(const std::vector<std::string> &file)
{
	_cpcBoosterFile = file;
	_currentFile = 0;
	_nbTransfertNeed = _cpcBoosterFile.size();
	_filepath = "";
}

void CCPCBoosterDlg::SetTransfertPath(const std::string &filepath)
{
	_filepath = filepath;
	_currentFile = 0;
	_nbTransfertNeed = 1;
	_cpcBoosterFile.clear();
}

void CCPCBoosterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPCBoosterDlg)
	DDX_Control(pDX, IDOK, _buttonOK);
	DDX_Text(pDX, IDC_COM_PORT_NUMBER, _comPortNumberStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPCBoosterDlg, CDialog)
	//{{AFX_MSG_MAP(CCPCBoosterDlg)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_COM_PORT_NUMBER, OnChangeComPortNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPCBoosterDlg message handlers

BOOL CCPCBoosterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (_transfert != NULL)
	{
		delete _transfert;
	}

	_transfert = new CAksFileTransfert(_comPortNumber);
	_currentFile = 0;

	if (_transfert != NULL)
	{
		if (_transfert->IsOpen())
		{
			SetTimer(6128, 50, NULL);
		}
		else
		{
			CString str;
			str.Format("No Booster found, please select another COM port number");
			GetDlgItem(IDC_CPCBOOSTER_TEXT)->SetWindowText(str);
		}
	}
	
	_buttonOK.EnableWindow(_currentFile == _nbTransfertNeed);

	_comPortNumberStr.Format("%d",_comPortNumber);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPCBoosterDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

	if (_transfert != NULL)
	{
		int nbTransfert = _transfert->GetNbTransfertDone();

		if (nbTransfert == _nbTransfertNeed)
		{
			_buttonOK.EnableWindow(TRUE);
			return;
		}

		std::string currentFile;
		CString str;
		if (_cpcBoosterFile.size() != 0)
		{
			if (nbTransfert == _currentFile)
			{
				_transfert->SetForceFilename(_cpcBoosterFile[_currentFile]);
				_currentFile++;
			}

			currentFile = _cpcBoosterFile[_currentFile-1];
			
			currentFile = currentFile.substr(currentFile.find_last_of('\\')+1, currentFile.size() - currentFile.find_last_of('\\') - 1);
		}
		else
		{
			_transfert->SetFilepath(_filepath);

			currentFile = _transfert->GetFilename();

			currentFile = currentFile.substr(currentFile.find_last_of('\\')+1, currentFile.size() - currentFile.find_last_of('\\') - 1);
		}
		
		_transfert->Run(false);

		nbTransfert = _transfert->GetNbTransfertDone();
		
		if (_transfert->IsTransfering())
		{
			str.Format("Transfering \"%s\"", currentFile.c_str());
		}
		else
		{
			if (nbTransfert == _nbTransfertNeed)
			{
				str.Format("Transfert completed");
			}
			else
			{
				str.Format("Waiting CPC for \"%s\"", currentFile.c_str());
			}
		}

		GetDlgItem(IDC_CPCBOOSTER_TEXT)->SetWindowText(str);
	}
}

BOOL CCPCBoosterDlg::DestroyWindow() 
{
	KillTimer(6128);
	
	return CDialog::DestroyWindow();
}

void CCPCBoosterDlg::OnChangeComPortNumber() 
{
	int oldCOMPort = _comPortNumber;
	
	UpdateData(TRUE);

	int newCOMPort = strtol(_comPortNumberStr, NULL, 0);

	if (errno != ERANGE && _comPortNumberStr != "")
	{
		_comPortNumber = newCOMPort;
		CRegistryKey key(CPCBOOSTER_PATH);
		key.SetValue(CPCBOOSTER_COM_PORT, _comPortNumber);

		if (oldCOMPort != _comPortNumber)
		{
			if (_transfert != NULL)
			{
				if (_transfert->IsOpen())
				{
					KillTimer(6128);
				}
				delete _transfert;
			}

			_transfert = new CAksFileTransfert(_comPortNumber);
			_currentFile = 0;

			if (_transfert != NULL)
			{
				if (_transfert->IsOpen())
				{
					SetTimer(6128, 50, NULL);
				}
				else
				{
					CString str;
					str.Format("No Booster found, please select another COM port number");
					GetDlgItem(IDC_CPCBOOSTER_TEXT)->SetWindowText(str);
				}
			}
		}
	}
	else
	{
		CString str;
		str.Format("No Booster found, please select another COM port number");
		GetDlgItem(IDC_CPCBOOSTER_TEXT)->SetWindowText(str);
	}
}
