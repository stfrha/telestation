// TeleStation.h : main header file for the TeleStation application
//

#if !defined(AFX_TeleStation_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_TeleStation_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "ProjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTeleStationApp:
// See TeleStation.cpp for the implementation of this class
//

class CTeleStationApp : public CWinApp
{
public:
	CTeleStationApp();

	CProjectDoc*	GetCurrentProjDoc( void );

	CMultiDocTemplate* m_projectTemplate;
	CMultiDocTemplate* m_scriptTemplate;
	CMultiDocTemplate* m_phoneBookTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeleStationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTeleStationApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TeleStation_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_)
