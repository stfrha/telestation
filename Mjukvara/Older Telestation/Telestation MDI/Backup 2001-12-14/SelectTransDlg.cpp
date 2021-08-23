// SelectTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "SelectTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg dialog


CSelectTransDlg::CSelectTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectTransDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_stringList.RemoveAll();
}


void CSelectTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectTransDlg)
	DDX_Control(pDX, IDC_TRANSITION_LIST, m_transitionList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectTransDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectTransDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg message handlers

BOOL CSelectTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	tmpStr;
	
	POSITION pos = m_stringList.GetHeadPosition();
	while (pos != NULL) {
		tmpStr = m_stringList.GetNext(pos);
		m_transitionList.AddString(tmpStr);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectTransDlg::OnOK() 
{
	m_selection = m_transitionList.GetCurSel();
	
	CDialog::OnOK();
}
