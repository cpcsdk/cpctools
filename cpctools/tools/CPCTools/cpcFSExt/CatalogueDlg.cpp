// CatalogueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cpcfsext.h"
#include "CatalogueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatalogueDlg dialog


CCatalogueDlg::CCatalogueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatalogueDlg::IDD, pParent),
	_dsk(NULL)
{
	//{{AFX_DATA_INIT(CCatalogueDlg)
	_catalogueStatus = _T("");
	//}}AFX_DATA_INIT
}


void CCatalogueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatalogueDlg)
	DDX_Control(pDX, IDC_CATALOGUE, _catalogueListCtrl);
	DDX_Text(pDX, IDC_CATALOGUE_STAT, _catalogueStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatalogueDlg, CDialog)
	//{{AFX_MSG_MAP(CCatalogueDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatalogueDlg message handlers

BOOL CCatalogueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect           rect;
	long        lStyle, lStyleOld;

	_catalogueListCtrl.GetWindowRect(&rect);
	int w = rect.Width();
	w -= 10;
	_catalogueListCtrl.InsertColumn(0, "User", LVCFMT_LEFT, w * 1/8,0);
	_catalogueListCtrl.InsertColumn(1, "Name", LVCFMT_LEFT, w * 4/8,1);
	_catalogueListCtrl.InsertColumn(2, "Size", LVCFMT_LEFT, w * 1/8,2);
	_catalogueListCtrl.InsertColumn(3, "System", LVCFMT_LEFT, w * 1/8,3);
	_catalogueListCtrl.InsertColumn(4, "WritePro", LVCFMT_LEFT, w * 1/8,4);

	lStyle = LVS_REPORT;
	lStyleOld = GetWindowLong(_catalogueListCtrl.m_hWnd, GWL_STYLE);
	lStyleOld &= ~(LVS_TYPEMASK);  // turn off all the style (view mode) bits
	lStyleOld |= lStyle;        // Set the new style for the control
	SetWindowLong(_catalogueListCtrl.m_hWnd, GWL_STYLE, lStyleOld);

	LV_ITEM         lvitem;
	int             iItem, iActualItem;

	int nbFile = 0;
	if (_dsk != NULL)
	{
		nbFile = _dsk->GetNbFiles();
	}

	int discOccup = 0;
	int discNbFile = 0;

	for (iItem = 0; iItem < nbFile; iItem++)
	{
		const CCPCDisc::CDiscFileEntryKey &fileEntry = _dsk->GetFileEntry(iItem);

		CString str;

		str.Format("%d",fileEntry.User);

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iItem;
		lvitem.iSubItem = 0;
		lvitem.pszText = (char*)(LPCTSTR)str;

		iActualItem = _catalogueListCtrl.InsertItem(&lvitem);

		str = fileEntry.getFilename().c_str();

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iActualItem;
		lvitem.iSubItem = 1;
		lvitem.pszText = (char*)(LPCTSTR)str;

		_catalogueListCtrl.SetItem(&lvitem);

		int size = _dsk->GetFileSize(iItem);
		str.Format("%d Ko",size);

		discOccup += (fileEntry.User != 229) ? size : 0;
		discNbFile += (fileEntry.User != 229) ? 1 : 0;

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iActualItem;
		lvitem.iSubItem = 2;
		lvitem.pszText = (char*)(LPCTSTR)str;

		_catalogueListCtrl.SetItem(&lvitem);

		str = fileEntry.System ? "S" : "";

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iActualItem;
		lvitem.iSubItem = 3;
		lvitem.pszText = (char*)(LPCTSTR)str;

		_catalogueListCtrl.SetItem(&lvitem);

		str = fileEntry.WriteProtected ? "*" : "";

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iActualItem;
		lvitem.iSubItem = 4;
		lvitem.pszText = (char*)(LPCTSTR)str;

		_catalogueListCtrl.SetItem(&lvitem);
	}

	int discCapat = _dsk->GetDiscCapacity();
	int discFree = discCapat - discOccup;

	_catalogueStatus.Format("%d files, %d Ko/%d Ko (%d Ko free)", discNbFile, discOccup, discCapat, discFree);


	CString discInfo, str;
	const DSK_GEOMETRY &geom = _dsk->GetDiscGeometry();
	
	str.Format("Format name : %s\n",_dsk->GetDiscFormatName().c_str());
	discInfo += str;
	str.Format("Format description : %s\n",_dsk->GetDiscFormatDescription().c_str());
	discInfo += str;
	str.Format("Nb tracks : %d\n",geom.dg_cylinders);
	discInfo += str;
	if (_dsk->NeedAdvancedMode())
	{
		str.Format("Advanced transfert mode\n");
	}
	else
	{
		if (_dsk->IsInterlaced())
		{
			str.Format("Normal transfert mode\n");
		}
		else
		{
			str.Format("Normal transfert mode (need interlace)\n");
		}
	}
	discInfo += str;

	GetDlgItem(IDC_DISC_INFO1)->SetWindowText(discInfo);

	str.Format("Nb sector per tracks : %d\n",geom.dg_sectors);
	discInfo = str;
	str.Format("Sector size : %d\n",geom.dg_secsize);
	discInfo += str;
	str.Format("Sector base : 0x%X\n",geom.dg_secbase);
	discInfo += str;
	str.Format("GAP3 : 0x%X\n",geom.dg_fmtgap);
	discInfo += str;

	GetDlgItem(IDC_DISC_INFO2)->SetWindowText(discInfo);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
