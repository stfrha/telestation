// NoPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TeleStation.h"
#include "NoPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoPropDlg dialog


CNoPropDlg::CNoPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoPropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNoPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoPropDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoPropDlg, CDialog)
	//{{AFX_MSG_MAP(CNoPropDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoPropDlg message handlers
