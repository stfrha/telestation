#if !defined(AFX_LABELPROPDLG_H__63B27EA2_342A_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_LABELPROPDLG_H__63B27EA2_342A_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LabelPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLabelPropDlg dialog

class CLabelPropDlg : public CDialog
{
// Construction
public:
	CLabelPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLabelPropDlg)
	enum { IDD = IDD_LABEL_PROP };
	CString	m_label;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabelPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLabelPropDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABELPROPDLG_H__63B27EA2_342A_11D4_9855_0000E873277A__INCLUDED_)
