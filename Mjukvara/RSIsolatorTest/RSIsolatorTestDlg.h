// RSIsolatorTestDlg.h : header file
//

#if !defined(AFX_RSISOLATORTESTDLG_H__10222E16_9679_11D3_9274_0060084CBD8A__INCLUDED_)
#define AFX_RSISOLATORTESTDLG_H__10222E16_9679_11D3_9274_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRSIsolatorTestDlg dialog

class CRSIsolatorTestDlg : public CDialog
{
// Construction
public:
	CRSIsolatorTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRSIsolatorTestDlg)
	enum { IDD = IDD_RSISOLATORTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIsolatorTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRSIsolatorTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSISOLATORTESTDLG_H__10222E16_9679_11D3_9274_0060084CBD8A__INCLUDED_)
