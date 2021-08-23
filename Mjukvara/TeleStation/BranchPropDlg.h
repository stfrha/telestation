#if !defined(AFX_BRANCHPROPDLG_H__196B2671_6879_406C_A6EA_6036F83A9A88__INCLUDED_)
#define AFX_BRANCHPROPDLG_H__196B2671_6879_406C_A6EA_6036F83A9A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BranchPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBranchPropDlg dialog

class CBranchPropDlg : public CDialog
{
// Construction
public:
	CBranchPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBranchPropDlg)
	enum { IDD = IDD_BRANCH_PROP };
	CListCtrl	m_catSelList;
	CString	m_catSelEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBranchPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString*	m_catList;
	CString*	m_source;

protected:

	// Generated message map functions
	//{{AFX_MSG(CBranchPropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickCatSelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCatSelList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRANCHPROPDLG_H__196B2671_6879_406C_A6EA_6036F83A9A88__INCLUDED_)
