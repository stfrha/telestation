#if !defined(AFX_ENTERCODEDLG_H__0B9B4A20_34AD_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_ENTERCODEDLG_H__0B9B4A20_34AD_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnterCodeDlg.h : header file
//

#include "ProjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CEnterCodeDlg dialog

class CEnterCodeDlg : public CDialog
{
// Construction
public:
	CEnterCodeDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetProject(CProjectDoc* projectDoc) { m_projectDoc = projectDoc; }
	void	SetSelection(CString selectedTitle) { m_selectedTitle = selectedTitle; }
	CString	GetSelection( void ) { return m_selectedTitle; }

// Dialog Data
	//{{AFX_DATA(CEnterCodeDlg)
	enum { IDD = IDD_ENTER_CODE_PROP };
	CComboBox	m_announcementsBox;
	CString	m_code;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnterCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProjectDoc*	m_projectDoc;
	CString			m_selectedTitle;

	// Generated message map functions
	//{{AFX_MSG(CEnterCodeDlg)
	afx_msg void OnAddFile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPlayFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTERCODEDLG_H__0B9B4A20_34AD_11D4_9855_0000E873277A__INCLUDED_)
