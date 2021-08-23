// ComSelect.cpp : implementation file
//

#include "stdafx.h"
#include "Dumper3.h"
#include "ComSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSelect dialog


CComSelect::CComSelect(CWnd* pParent /*=NULL*/, int State)
	: CDialog(CComSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSelect)
	m_ComState = State;
	//}}AFX_DATA_INIT
}


void CComSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSelect)
	DDX_Radio(pDX, IDC_COM1, m_ComState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSelect, CDialog)
	//{{AFX_MSG_MAP(CComSelect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSelect message handlers
