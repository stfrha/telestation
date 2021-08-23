// WaveFormaterDlg.h : header file
//

#if !defined(AFX_WAVEFORMATERDLG_H__22D59269_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_)
#define AFX_WAVEFORMATERDLG_H__22D59269_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWaveFormaterDlg dialog

class CWaveFormaterDlg : public CDialog
{
// Construction
public:
	CWaveFormaterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWaveFormaterDlg)
	enum { IDD = IDD_WAVEFORMATER_DIALOG };
	long	m_formatTag;
	int		m_nChannels;
	int		m_samplePerSec;
	long	m_byteLen;
	long	m_timeLen;
	long	m_formatLen;
	int		m_bitPerSample;
	long	m_avgBytePerSec;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveFormaterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWaveFormaterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelectWave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEFORMATERDLG_H__22D59269_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_)
