#if !defined(AFX_NEWPROJECTDLG_H__6D4513C3_394B_11D4_B782_0000E8732786__INCLUDED_)
#define AFX_NEWPROJECTDLG_H__6D4513C3_394B_11D4_B782_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog

class CNewProjectDlg : public CDialog
{
// Construction
public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor

	CString		m_scriptPath;
	CString		m_scriptTitle;
	CString		m_phoneBookPath;
	CString		m_phoneBookTitle;
	CString		m_projectTitle;
	CString		m_projectPath;
	CString		m_selectedLocation;


// Dialog Data
	//{{AFX_DATA(CNewProjectDlg)
	enum { IDD = IDD_NEW_PROJECT_DLG };
	CEdit	m_projectNameEdit;
	CString	m_newProjectScript;
	CString	m_newProjectPhoneBook;
	CString	m_location;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CString		m_nameBase;

	// Generated message map functions
	//{{AFX_MSG(CNewProjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrwsScript();
	afx_msg void OnBrwsPhoneBook();
	afx_msg void OnChangeProjectName();
	afx_msg void OnBrwsLocation();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTDLG_H__6D4513C3_394B_11D4_B782_0000E8732786__INCLUDED_)
