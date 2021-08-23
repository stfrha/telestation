#if !defined(AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_)
#define AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPBEntry.h : header file
//

#include "CListTestDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPBEntry dialog

class CNewPBEntry : public CDialog
{
// Construction
public:
	CNewPBEntry(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPBEntry)
	enum { IDD = IDD_NEW_PB_ENTRY };
	CString	m_name;
	CString	m_number;
	CString	m_categoryStr;
	int		m_catSpin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPBEntry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CCListTestDoc*	m_doc;


protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPBEntry)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPBENTRY_H__820FEE94_0CCA_4DA6_9911_CA5CF88A54D6__INCLUDED_)
