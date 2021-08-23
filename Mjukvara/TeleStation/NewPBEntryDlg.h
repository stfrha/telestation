#if !defined(AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_)
#define AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPBEntry.h : header file
//

#include "PhoneBookDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntryDlg dialog

class CNewPBEntryDlg : public CDialog
{
// Construction
public:
	CNewPBEntryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPBEntryDlg)
	enum { IDD = IDD_NEW_PB_ENTRY };
	CEdit	m_numberCtrl;
	CEdit	m_nameCtrl;
	CComboBox	m_catListCtrl;
	CString	m_name;
	CString	m_number;
	int		m_catList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPBEntryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CPhoneBookDoc*	m_doc;
	BOOL			m_multiSelection;
	CString			m_windowTitle;

	
	void			PopulateList(CPhoneBookDoc*	doc);
	void			SetMultiSel(BOOL multiSelection);


protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPBEntryDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_)
