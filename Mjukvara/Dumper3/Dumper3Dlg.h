// Dumper3Dlg.h : header file
//

#if !defined(AFX_DUMPER3DLG_H__40D24D48_CEB6_11D1_918D_0060084CBD8A__INCLUDED_)
#define AFX_DUMPER3DLG_H__40D24D48_CEB6_11D1_918D_0060084CBD8A__INCLUDED_

#include "SERIALCO.H"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CDumper3Dlg dialog

class CDumper3Dlg : public CDialog
{
// Construction
public:
	CSerialCom MySerCom;
	CDumper3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDumper3Dlg)
	enum { IDD = IDD_DUMPER3_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDumper3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDumper3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnDump();
	afx_msg void OnConntest();
	afx_msg void OnSetup();
	afx_msg void OnBackload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMPER3DLG_H__40D24D48_CEB6_11D1_918D_0060084CBD8A__INCLUDED_)
