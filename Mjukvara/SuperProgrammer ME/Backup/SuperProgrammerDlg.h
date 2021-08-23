// SuperProgrammerDlg.h : header file
//

#if !defined(AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_)
#define AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemMap.h"
#include "serialco.h"

/////////////////////////////////////////////////////////////////////////////
// CSuperProgrammerDlg dialog

class CSuperProgrammerDlg : public CDialog
{
// Construction
public:
	CSuperProgrammerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSuperProgrammerDlg)
	enum { IDD = IDD_SUPERPROGRAMMER_DIALOG };
	CProgressCtrl	m_progress;
	int		m_comPort;
	CString	m_filename;
	CString	m_status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProgrammerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CMemMap			m_memMap;
	CSerialCom		m_serialCom;
	CString			m_fileNPath;

	HICON			m_hIcon;


	unsigned char	ReadNextByte(FILE *hexfile);

	// Generated message map functions
	//{{AFX_MSG(CSuperProgrammerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnProgram();
	afx_msg void OnBrowse();
	afx_msg void OnConntest();
	afx_msg void OnFinalize();
	afx_msg void OnGethigh();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_)
