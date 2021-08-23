// TeleFileSel.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProgrammer.h"
#include "TeleFileSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeleFileSel dialog


CTeleFileSel::CTeleFileSel(CWnd* pParent /*=NULL*/)
	: CDialog(CTeleFileSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeleFileSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTeleFileSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeleFileSel)
	DDX_Control(pDX, IDC_FILESEL, m_fileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTeleFileSel, CDialog)
	//{{AFX_MSG_MAP(CTeleFileSel)
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_FILESEL, DoubleClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeleFileSel message handlers

BOOL CTeleFileSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	POSITION	pos;

	for( pos = m_fileStrings.GetHeadPosition(); pos != NULL; )   {
		m_fileList.AddString(m_fileStrings.GetNext(pos));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeleFileSel::DoubleClick( void )
{
	OnOK();
}

void CTeleFileSel::OnOK() 
{
	UpdateData(TRUE);

	*m_nSelected = m_fileList.GetSelCount();
	m_fileList.GetSelItems(m_maxSelected, m_selIndexArray);

/*
	m_fileList.GetText(m_fileList.GetCurSel(), m_retStr);
	*m_retIndex = m_fileList.GetCurSel();
*/	
	CDialog::OnOK();
}
