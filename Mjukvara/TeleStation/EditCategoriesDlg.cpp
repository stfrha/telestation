// EditCategoriesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "telestation.h"
#include "EditCategoriesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCategoriesDlg dialog


CEditCategoriesDlg::CEditCategoriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCategoriesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCategoriesDlg)
	m_categoryEdit = _T("");
	//}}AFX_DATA_INIT
	m_listMirror = NULL;
	m_isModified = FALSE;
}


void CEditCategoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCategoriesDlg)
	DDX_Control(pDX, IDC_EDIT_CATEGORY, m_editCategory);
	DDX_Control(pDX, IDC_CAT_EDIT, m_categoryEditCtrl);
	DDX_Control(pDX, IDC_DELETE_CATEGORY, m_deleteCategory);
	DDX_Control(pDX, IDC_CATEGORY_COMBO, m_categoryCombo);
	DDX_Control(pDX, IDC_CATEGORY_LIST, m_categoryListCtrl);
	DDX_Text(pDX, IDC_CAT_EDIT, m_categoryEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditCategoriesDlg, CDialog)
	//{{AFX_MSG_MAP(CEditCategoriesDlg)
	ON_NOTIFY(NM_CLICK, IDC_CATEGORY_LIST, OnClickCategoryList)
	ON_CBN_SELCHANGE(IDC_CATEGORY_COMBO, OnSelchangeCategoryCombo)
	ON_BN_CLICKED(IDC_DELETE_CATEGORY, OnDeleteCategory)
	ON_NOTIFY(NM_DBLCLK, IDC_CATEGORY_LIST, OnDblclkCategoryList)
	ON_BN_CLICKED(IDC_EDIT_CATEGORY, OnEditCategory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCategoriesDlg message handlers

BOOL CEditCategoriesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_listMirror) {
		MessageBox("m_listMirror uninitialized", "Error");
	} else {
		
		char	col0Head[] = "Category";
		char	col1Head[] = "Explanation";

		m_categoryListCtrl.InsertColumn(0, col0Head, LVCFMT_LEFT, 80, -1);
		m_categoryListCtrl.InsertColumn(1, col1Head, LVCFMT_LEFT, 250, -1);

		m_categoryListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);

		UpdateDialog();
	}

	m_deleteCategory.EnableWindow(FALSE);

	int		i;
	CString	tmp;

	for (i=0 ; i<256 ; i++) {
		tmp.Format("%d", i);
		m_categoryCombo.AddString(tmp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCategoriesDlg::OnClickCategoryList(NMHDR* pNMHDR, LRESULT* pResult) 
{
}

void CEditCategoriesDlg::OnSelchangeCategoryCombo() 
{
	CString	tmp;
	int		selCategory;

	m_categoryCombo.GetLBText(m_categoryCombo.GetCurSel(), tmp);
	sscanf(tmp, "%d", &selCategory);

	if (m_listMirror[selCategory].IsEmpty()) {
		m_categoryEdit = "";
		UpdateData(FALSE);
//		m_categoryEditCtrl.SetFocus();
		m_editCategory.SetWindowText("New");
		m_deleteCategory.EnableWindow(FALSE);
	} else {
		m_categoryEdit = m_listMirror[selCategory];
		UpdateData(FALSE);
//		m_categoryEditCtrl.SetFocus();
		m_categoryEditCtrl.SetSel(0, -1);
		m_editCategory.SetWindowText("Rename");
		m_deleteCategory.EnableWindow(TRUE);
	}
}

void CEditCategoriesDlg::UpdateDialog( void )
{
	int		i, j;
	CString	tmp;

	m_categoryListCtrl.DeleteAllItems();
	
	j = 0;
	for (i=0 ; i<256 ; i++) {
		if (!m_listMirror[i].IsEmpty()) {
			tmp.Format("%d", i);
			m_categoryListCtrl.InsertItem(j, tmp);
			m_categoryListCtrl.SetItemText(j, 1, m_listMirror[i]);
			j++;
		}
	}
}

void CEditCategoriesDlg::OnDeleteCategory() 
{
	CString	tmp;
	int		selCategory;

	m_categoryCombo.GetLBText(m_categoryCombo.GetCurSel(), tmp);
	sscanf(tmp, "%d", &selCategory);

	m_listMirror[selCategory] = "";
	m_isModified = TRUE;

	UpdateDialog();
}

void CEditCategoriesDlg::OnDblclkCategoryList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int		selCategory;

	POSITION pos = m_categoryListCtrl.GetFirstSelectedItemPosition();
	int	selIndex = m_categoryListCtrl.GetNextSelectedItem(pos);

	CString tmp = m_categoryListCtrl.GetItemText(selIndex, 0);
	if (!tmp.IsEmpty()) {
		sscanf(tmp, "%d", &selCategory);

		m_categoryEdit = m_listMirror[selCategory];
		UpdateData(FALSE);
		m_categoryCombo.SelectString(0, tmp);
		m_categoryEditCtrl.SetFocus();
		m_categoryEditCtrl.SetSel(0, -1);
		m_editCategory.SetWindowText("Rename");
		m_deleteCategory.EnableWindow(TRUE);
	}

	*pResult = 0;
}

void CEditCategoriesDlg::OnEditCategory() 
{
	CString	state;
	CString	tmp;
	int		selCategory;

	m_editCategory.GetWindowText(state);

	if (state == "Rename") {
		m_categoryCombo.GetLBText(m_categoryCombo.GetCurSel(), tmp);
		sscanf(tmp, "%d", &selCategory);

		UpdateData(TRUE);

		m_listMirror[selCategory] = m_categoryEdit;
		m_isModified = TRUE;

		UpdateDialog();	
	} else if (state == "New") {
		m_categoryCombo.GetLBText(m_categoryCombo.GetCurSel(), tmp);
		sscanf(tmp, "%d", &selCategory);

		UpdateData(TRUE);

		m_listMirror[selCategory] = m_categoryEdit;
		m_isModified = TRUE;

		UpdateDialog();
	} else {
		MessageBox("Button text is neither New nor Rename", "Error");
	}
}
