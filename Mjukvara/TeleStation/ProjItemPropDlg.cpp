// ProjItemPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TeleStation.h"
#include "ProjItemPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjItemPropDlg dialog


CProjItemPropDlg::CProjItemPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjItemPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjItemPropDlg)
	m_title = _T("");
	m_path = _T("");
	//}}AFX_DATA_INIT
}


void CProjItemPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjItemPropDlg)
	DDX_Text(pDX, IDC_TITLE, m_title);
	DDX_Text(pDX, IDC_PATH, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjItemPropDlg, CDialog)
	//{{AFX_MSG_MAP(CProjItemPropDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjItemPropDlg message handlers

BOOL CProjItemPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_title = m_t;
	m_path = m_p;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
