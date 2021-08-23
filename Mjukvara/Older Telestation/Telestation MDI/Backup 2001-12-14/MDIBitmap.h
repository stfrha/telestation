// MDIBitmap.h : main header file for the MDIBITMAP application
//

#if !defined(AFX_MDIBITMAP_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_MDIBITMAP_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "ProjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapApp:
// See MDIBitmap.cpp for the implementation of this class
//

class CMDIBitmapApp : public CWinApp
{
public:
	CMDIBitmapApp();

	CProjectDoc*	GetCurrentProjDoc( void );

	CMultiDocTemplate* m_projectTemplate;
	CMultiDocTemplate* m_scriptTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIBitmapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMDIBitmapApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIBITMAP_H__5CEC8044_21FE_11D4_9855_0000E873277A__INCLUDED_)
