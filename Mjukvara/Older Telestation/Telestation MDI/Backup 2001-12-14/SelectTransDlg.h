#if !defined(AFX_SELECTTRANSDLG_H__0B9B4A21_34AD_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_SELECTTRANSDLG_H__0B9B4A21_34AD_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg dialog

class CSelectTransDlg : public CDialog
{
// Construction
public:
	CSelectTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectTransDlg)
	enum { IDD = IDD_TRANS_SELECT };
	CListBox	m_transitionList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CStringList		m_stringList;
	int				m_selection;

protected:


	// Generated message map functions
	//{{AFX_MSG(CSelectTransDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTRANSDLG_H__0B9B4A21_34AD_11D4_9855_0000E873277A__INCLUDED_)
