// BranchPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "telestation.h"
#include "BranchPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBranchPropDlg dialog


CBranchPropDlg::CBranchPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBranchPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBranchPropDlg)
	m_catSelEdit = _T("");
	//}}AFX_DATA_INIT
}


void CBranchPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBranchPropDlg)
	DDX_Control(pDX, IDC_CAT_SEL_LIST, m_catSelList);
	DDX_Text(pDX, IDC_CAT_SEL_EDIT, m_catSelEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBranchPropDlg, CDialog)
	//{{AFX_MSG_MAP(CBranchPropDlg)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_CAT_SEL_LIST, OnClickCatSelList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CAT_SEL_LIST, OnItemchangedCatSelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBranchPropDlg message handlers

BOOL CBranchPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char	col0Head[] = "Category";

	m_catSelList.InsertColumn(0, col0Head, LVCFMT_LEFT, 175, -1);

	m_catSelList.SetExtendedStyle(LVS_EX_CHECKBOXES/* | LVS_EX_GRIDLINES*/);

	CWaitCursor wc;

	CString	tmpStr;
	int		i;

	for (i=0 ; i<256 ; i++) {
		if (!m_catList[i].IsEmpty()) {
			m_catSelList.InsertItem(i, m_catList[i]);
		} else {
			tmpStr.Format("%d", i);
			m_catSelList.InsertItem(i, tmpStr);
		}
	}

	m_catSelEdit = *m_source;

	CString	strRest = m_catSelEdit;
	CString	separator;
	int		rangeStart = -1;
	int		tmpV;

	while (strRest.GetLength()) {
		tmpStr = strRest.SpanIncluding("0123456789");
		sscanf(tmpStr, "%d", &tmpV);
		strRest = strRest.Right(strRest.GetLength() - tmpStr.GetLength());
		if (rangeStart > -1) {
			for (i=rangeStart ; i<=tmpV ; i++) {
				m_catSelList.SetCheck(i, TRUE);
			}
			rangeStart = -1;
			strRest = strRest.Right(strRest.GetLength() - 1);
		} else {
			m_catSelList.SetCheck(tmpV, TRUE);
			if (strRest.GetLength()) {
				separator = strRest.GetAt(0);
				strRest = strRest.Right(strRest.GetLength() - 1);
				if (separator == "-") {
					rangeStart = tmpV;
				}
			}
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBranchPropDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);

}

void CBranchPropDlg::OnClickCatSelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CBranchPropDlg::OnItemchangedCatSelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int		i = 0, lastChecked, firstChecked;
	CString	tmpStr; 

	m_catSelEdit = "";

	while (i<256) {
		while ((i<256) && (!m_catSelList.GetCheck(i++)));
		if (i<256) {
			firstChecked = i - 1;
			tmpStr.Format("%d", firstChecked);
			m_catSelEdit += tmpStr;
		} else if ((i == 256) && m_catSelList.GetCheck(255)) {
			firstChecked = 255;
			tmpStr.Format("%d", firstChecked);
			m_catSelEdit += tmpStr;
		} else {
			firstChecked = -1;
		}
		while ((i<256) && (m_catSelList.GetCheck(i++)));
		if ((i == 256) && (m_catSelList.GetCheck(255))) lastChecked = 255;
		else lastChecked = i - 2;
		tmpStr.Format("%d", lastChecked);
		if (firstChecked == lastChecked) m_catSelEdit += ",";
		else if (firstChecked > -1) m_catSelEdit += ("-" + tmpStr + ",");
	}


	if (m_catSelEdit.GetLength()) {
		if (m_catSelEdit.GetAt(m_catSelEdit.GetLength()-1) == ',') {
			m_catSelEdit = m_catSelEdit.Left(m_catSelEdit.GetLength()-1);
		}
	}

	UpdateData(FALSE);	
	
	*pResult = 0;
}
