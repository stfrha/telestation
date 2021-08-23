#if !defined(AFX_PLAYFILEPROPDLG_H__FFEB23C0_32FA_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_PLAYFILEPROPDLG_H__FFEB23C0_32FA_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayFilePropDlg.h : header file
//

#include "ProjectDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CPlayFilePropDlg dialog

class CPlayFilePropDlg : public CDialog
{
// Construction
public:
	CPlayFilePropDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetProject(CProjectDoc* projectDoc) { m_projectDoc = projectDoc; }
	void	SetSelection(CString selectedTitle) { m_selectedTitle = selectedTitle; }
	CString	GetSelection( void ) { return m_selectedTitle; }


// Dialog Data
	//{{AFX_DATA(CPlayFilePropDlg)
	enum { IDD = IDD_PLAY_FILE_PROP };
	CComboBox	m_announcementsBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayFilePropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProjectDoc*	m_projectDoc;
	CString			m_selectedTitle;

	// Generated message map functions
	//{{AFX_MSG(CPlayFilePropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddFile();
	virtual void OnOK();
	afx_msg void OnPlayFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYFILEPROPDLG_H__FFEB23C0_32FA_11D4_9855_0000E873277A__INCLUDED_)
