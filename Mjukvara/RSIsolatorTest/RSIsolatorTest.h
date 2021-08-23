// RSIsolatorTest.h : main header file for the RSISOLATORTEST application
//

#if !defined(AFX_RSISOLATORTEST_H__10222E14_9679_11D3_9274_0060084CBD8A__INCLUDED_)
#define AFX_RSISOLATORTEST_H__10222E14_9679_11D3_9274_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRSIsolatorTestApp:
// See RSIsolatorTest.cpp for the implementation of this class
//

class CRSIsolatorTestApp : public CWinApp
{
public:
	CRSIsolatorTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIsolatorTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRSIsolatorTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSISOLATORTEST_H__10222E14_9679_11D3_9274_0060084CBD8A__INCLUDED_)
