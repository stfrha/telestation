#if !defined(AFX_EDITCATEGORIESDLG_H__AAED1063_F0E5_11D5_B171_0000E8732786__INCLUDED_)
#define AFX_EDITCATEGORIESDLG_H__AAED1063_F0E5_11D5_B171_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCategoriesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditCategoriesDlg dialog

class CEditCategoriesDlg : public CDialog
{
// Construction
public:
	CEditCategoriesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditCategoriesDlg)
	enum { IDD = IDD_EDIT_CATEGORIES };
	CButton	m_editCategory;
	CEdit	m_categoryEditCtrl;
	CButton	m_deleteCategory;
	CComboBox	m_categoryCombo;
	CListCtrl	m_categoryListCtrl;
	CString	m_categoryEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCategoriesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	CString*	m_listMirror;
	BOOL		m_isModified;

	void		UpdateDialog( void );


	// Generated message map functions
	//{{AFX_MSG(CEditCategoriesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCategoryList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCategoryCombo();
	afx_msg void OnDeleteCategory();
	afx_msg void OnDblclkCategoryList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCategory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCATEGORIESDLG_H__AAED1063_F0E5_11D5_B171_0000E8732786__INCLUDED_)
