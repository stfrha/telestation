#if !defined(AFX_SCRIPTSIMULATORDLG_H__B544F1A1_3DF3_11D4_AD85_0060084CBD8A__INCLUDED_)
#define AFX_SCRIPTSIMULATORDLG_H__B544F1A1_3DF3_11D4_AD85_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptSimulatorDlg.h : header file
//

#include "MDIBitmapDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CScriptSimulatorDlg dialog

class CScriptSimulatorDlg : public CDialog
{
// Construction
public:
	CScriptSimulatorDlg(CWnd* pParent = NULL);   // standard constructor



	BOOL			IsSimulatorRunning( void ) { return m_simulatorOn; }
	void			SetScriptDoc(CMDIBitmapDoc* script) { m_scriptDoc = script; }
	void			InvalidateScriptDoc( void );
	void			InvalidateProjectDoc( void );
	void			SetProjectDoc(CProjectDoc* project) { m_projectDoc = project; }
	CScriptObject*	GetCurrentSimObject( void ) { return m_currentSimObj; }
	void			EnableOnSwitch(BOOL enable = TRUE);

// Dialog Data
	//{{AFX_DATA(CScriptSimulatorDlg)
	enum { IDD = IDD_SCRIPT_SIMULATOR };
	CButton	m_hangUp;
	CButton	m_makeCall;
	BOOL	m_simulatorOn;
	CString	m_callerNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptSimulatorDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMDIBitmapDoc*	m_scriptDoc;
	CProjectDoc*	m_projectDoc;
	CScriptObject*	m_currentSimObj;

	int				m_DTMFTone;
	int				m_timer;
	BOOL			m_timeout;
	BOOL			m_hungUp;
	int				m_nCallSignals;
	int				m_simState;

	CScriptObject*	FindStartObject( void );
	void			PerformObject( void );
	void			GotoNextObject(void );
	void			TerminateSimulation( void );
	CScriptObject*	FindLastLabel(CString label);
	UINT			GetSoundDuration(CString path);
	
	// Generated message map functions
	//{{AFX_MSG(CScriptSimulatorDlg)
	virtual void OnCancel();
	afx_msg void OnSimulatorOn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMakeCall();
	virtual BOOL OnInitDialog();
	afx_msg void OnHangUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTSIMULATORDLG_H__B544F1A1_3DF3_11D4_AD85_0060084CBD8A__INCLUDED_)
