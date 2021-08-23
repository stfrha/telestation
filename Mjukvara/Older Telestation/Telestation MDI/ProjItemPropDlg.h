#if !defined(AFX_PROJITEMPROPDLG_H__B09C8CC7_3B27_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_PROJITEMPROPDLG_H__B09C8CC7_3B27_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjItemPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjItemPropDlg dialog

class CProjItemPropDlg : public CDialog
{
// Construction
public:
	CProjItemPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjItemPropDlg)
	enum { IDD = IDD_PROJ_PROP_DLG };
	CString	m_title;
	CString	m_path;
	//}}AFX_DATA

	CString	m_t;
	CString	m_p;

	void	SetDlgData(CString title, CString path) {m_t = title; m_p = path; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjItemPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjItemPropDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJITEMPROPDLG_H__B09C8CC7_3B27_11D4_9855_0000E873277A__INCLUDED_)
