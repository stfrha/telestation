// NewPBEntry.cpp : implementation file
//

#include "stdafx.h"
#include "CListTest.h"
#include "NewPBEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntry dialog


CNewPBEntry::CNewPBEntry(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPBEntry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPBEntry)
	m_name = _T("");
	m_number = _T("");
	m_categoryStr = _T("");
	m_catSpin = 0;
	//}}AFX_DATA_INIT
}


void CNewPBEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPBEntry)
	DDX_Text(pDX, IDC_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_NUMBER_EDIT, m_number);
	DDX_CBString(pDX, IDC_CAT_COMBO, m_categoryStr);
	DDX_Text(pDX, IDC_CAT_SPIN, m_catSpin);
	DDV_MinMaxInt(pDX, m_catSpin, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPBEntry, CDialog)
	//{{AFX_MSG_MAP(CNewPBEntry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntry message handlers

void CNewPBEntry::OnOK() 
{
	UpdateData(TRUE);
	if (m_doc->FindEntry(m_number)) {
		MessageBox("Number already in phone book", "Error");
		return;
	}
	
	CDialog::OnOK();
}
