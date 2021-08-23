#if !defined(AFX_TERMEDIT_H__1D9665D3_99E4_11D3_B688_0000E8732786__INCLUDED_)
#define AFX_TERMEDIT_H__1D9665D3_99E4_11D3_B688_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TermEdit.h : header file
//

#include "SERIALCO.h"


#define MAX_CHARS 10240
//10240
#define MAX_LINES 256

/////////////////////////////////////////////////////////////////////////////
// CTermEdit window

class CTermEdit : public CEdit
{
// Construction
public:
	CTermEdit();

// Attributes
public:
	CSerialCom*		m_serialCom;
	BOOL*			m_active;
	int				m_timer;
	BOOL*			m_hexMonitor;


// Operations
public:
	void			SetSerialCom(CSerialCom* serCom);
	void			SetActive(BOOL* active);
	void			SetHexMonitor(BOOL* hexMonitorOn);
	void			StartTimer(void);
	void			StopTimer(void);
	void			PutChar(char c);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTermEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTermEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTermEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnErrspace();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMEDIT_H__1D9665D3_99E4_11D3_B688_0000E8732786__INCLUDED_)
