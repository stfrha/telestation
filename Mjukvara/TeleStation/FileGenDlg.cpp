// FileGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "telestation.h"
#include "FileGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileGenDlg dialog


CFileGenDlg::CFileGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileGenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileGenDlg)
	DDX_Control(pDX, IDCANCEL, m_doneButton);
	DDX_Control(pDX, IDC_LOG_LIST, m_logList);
	//}}AFX_DATA_MAP
}

BOOL CFileGenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_doneButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CFileGenDlg, CDialog)
	//{{AFX_MSG_MAP(CFileGenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileGenDlg message handlers

void CFileGenDlg::EnableClose( void )
{
	m_doneButton.EnableWindow(TRUE);
}

void CFileGenDlg::InsertString(CString str)
{
	m_logList.AddString(str);
}

void CFileGenDlg::OnCancel() 
{
	//CDialog::OnCancel();
	DestroyWindow();
}
