// NewPBEntry.cpp : implementation file
//

#include "stdafx.h"
#include "TeleStation.h"
#include "NewPBEntryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntryDlg dialog


CNewPBEntryDlg::CNewPBEntryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPBEntryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPBEntryDlg)
	m_name = _T("");
	m_number = _T("");
	m_catList = 0;
	//}}AFX_DATA_INIT
	m_multiSelection = FALSE;
	m_windowTitle = "";
}


void CNewPBEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPBEntryDlg)
	DDX_Control(pDX, IDC_NUMBER_EDIT, m_numberCtrl);
	DDX_Control(pDX, IDC_NAME_EDIT, m_nameCtrl);
	DDX_Control(pDX, IDC_CAT_COMBO, m_catListCtrl);
	DDX_Text(pDX, IDC_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_NUMBER_EDIT, m_number);
	DDX_CBIndex(pDX, IDC_CAT_COMBO, m_catList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPBEntryDlg, CDialog)
	//{{AFX_MSG_MAP(CNewPBEntryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntryDlg message handlers

void CNewPBEntryDlg::OnOK() 
{
/*
	UpdateData(TRUE);
	if (m_doc->FindEntry(m_number)) {
		MessageBox("Number already in phone book", "Error");
		return;
	}
*/	
	CDialog::OnOK();
}


void CNewPBEntryDlg::PopulateList(CPhoneBookDoc* doc)
{
	m_doc = doc;
}

void CNewPBEntryDlg::SetMultiSel(BOOL multiSelection)
{
	m_multiSelection = multiSelection;
}

BOOL CNewPBEntryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int		i;
	CString	tmp;
	
	for (i=0 ; i<256 ; i++) {
		if (m_doc->m_categoryList[i].IsEmpty()) {
			tmp.Format("%d", i);
		} else {
			tmp = m_doc->m_categoryList[i];
		}
		m_catListCtrl.AddString(tmp);
	}
	if ((m_catList > 255) || (m_catList < 0)) {
		m_catList = 0;
	}

	if (m_multiSelection) {
		m_nameCtrl.EnableWindow(FALSE);
		m_name = "";
		m_numberCtrl.EnableWindow(FALSE);
		m_number = "";
	}

	if (!m_windowTitle.IsEmpty()) {
		SetWindowText(m_windowTitle);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
