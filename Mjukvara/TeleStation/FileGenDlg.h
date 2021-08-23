#if !defined(AFX_FILEGENDLG_H__EAA1F6E6_40DD_45D5_B97A_6A8051D9387A__INCLUDED_)
#define AFX_FILEGENDLG_H__EAA1F6E6_40DD_45D5_B97A_6A8051D9387A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileGenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileGenDlg dialog

class CFileGenDlg : public CDialog
{
// Construction
public:
	CFileGenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileGenDlg)
	enum { IDD = IDD_FILE_GEN_LOG };
	CButton	m_doneButton;
	CListBox	m_logList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:
	void			InsertString(CString str);
	void			EnableClose( void );

protected:

	// Generated message map functions
	//{{AFX_MSG(CFileGenDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEGENDLG_H__EAA1F6E6_40DD_45D5_B97A_6A8051D9387A__INCLUDED_)
