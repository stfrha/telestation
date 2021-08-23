#if !defined(AFX_COMSELECT_H__6FD73232_D427_11D1_9191_0060084CBD8A__INCLUDED_)
#define AFX_COMSELECT_H__6FD73232_D427_11D1_9191_0060084CBD8A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ComSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSelect dialog

class CComSelect : public CDialog
{
// Construction
public:
	CComSelect(CWnd* pParent = NULL, int State = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComSelect)
	enum { IDD = IDD_COMSELECT };
	int		m_ComState;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSelect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSELECT_H__6FD73232_D427_11D1_9191_0060084CBD8A__INCLUDED_)
