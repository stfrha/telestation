#if !defined(AFX_NOPROPDLG_H__63B27EA3_342A_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_NOPROPDLG_H__63B27EA3_342A_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoPropDlg dialog

class CNoPropDlg : public CDialog
{
// Construction
public:
	CNoPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNoPropDlg)
	enum { IDD = IDD_NO_PROP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoPropDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOPROPDLG_H__63B27EA3_342A_11D4_9855_0000E873277A__INCLUDED_)
