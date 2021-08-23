// SuperProgrammerDlg.h : header file
//

#if !defined(AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_)
#define AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "MemMap.h"
#include "TermEdit.h"
#include "serialco.h"

#define PAGE_SIZE	64


class CProgPage : public CObject {
public :
	CProgPage(UINT startAddress ) { m_startAddress = startAddress; }
protected:
	CProgPage();

public:
	UINT			m_startAddress;
	unsigned char	m_data[PAGE_SIZE];		//One page of the EEPROM
};

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
	CButton	m_download2;
	CProgressCtrl	m_mainProgress;
	CButton	m_sendZero;
	CButton	m_sendESC;
	CButton	m_send;
	CButton	m_conTest;
	CButton	m_download;
	CButton	m_upload;
	CButton	m_program;
	CProgressCtrl	m_progress;
	int		m_comPort;
	CString	m_filename;
	CString	m_status;
	BOOL	m_activeTerm;
	int		m_decSend;
	CString	m_charSend;
	CString	m_hexSend;
	BOOL	m_hexMonitor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProgrammerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CMemMap			m_memMap;
	CTermEdit		m_termEdit;
	CSerialCom		m_serialCom;
	CString			m_fileNPath;

	bool			m_timeout;
	bool			m_serSetup;

	HICON			m_hIcon;

	CTypedPtrList<CObList, CProgPage*>	m_pageList;

	int				ReadIntelFile(FILE* fp);
	void			CreatePageList( void );
	void			InsertDataRow(UINT offset, int dataLength, char* rowData);
	void			InsertSinglePageData(UINT offset, int dataLength, char* rowData);
	CProgPage*		FindPage(UINT offset);
	int				DownloadPages( void );
	int				DownloadOnePage(CProgPage* page);
	unsigned char	ReadNextByte(FILE *hexfile);
	CString			SelectFileDestination( void );

public:
	void			CloseSerialPort( void );



	// Generated message map functions
	//{{AFX_MSG(CSuperProgrammerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnProgram();
	afx_msg void OnBrowse();
	afx_msg void OnConntest();
	afx_msg void OnDownload();
	afx_msg void OnSelchangeComport();
	afx_msg void OnUpload();
	afx_msg void OnToWav();
	afx_msg void OnToWaw();
	afx_msg void OnActiveterm();
	afx_msg void OnKillfocusCharsend();
	afx_msg void OnKillfocusDecsend();
	afx_msg void OnKillfocusHexsend();
	afx_msg void OnSend();
	afx_msg void OnSndesc();
	afx_msg void OnSndzer();
	afx_msg void OnClearTerm();
	afx_msg void OnHexMonitor();
	afx_msg void OnPort2text();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDownload2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROGRAMMERDLG_H__19C391A7_5FDC_11D3_B648_0000E8732786__INCLUDED_)
