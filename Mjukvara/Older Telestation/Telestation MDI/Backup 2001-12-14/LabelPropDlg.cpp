// LabelPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "LabelPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabelPropDlg dialog


CLabelPropDlg::CLabelPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLabelPropDlg)
	m_label = _T("");
	//}}AFX_DATA_INIT
}


void CLabelPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLabelPropDlg)
	DDX_Text(pDX, IDC_LABEL, m_label);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLabelPropDlg, CDialog)
	//{{AFX_MSG_MAP(CLabelPropDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelPropDlg message handlers
