// SuperProgrammer.h : main header file for the SUPERPROGRAMMER application
//

#if !defined(AFX_SUPERPROGRAMMER_H__19C391A5_5FDC_11D3_B648_0000E8732786__INCLUDED_)
#define AFX_SUPERPROGRAMMER_H__19C391A5_5FDC_11D3_B648_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSuperProgrammerApp:
// See SuperProgrammer.cpp for the implementation of this class
//

class CSuperProgrammerApp : public CWinApp
{
public:
	CSuperProgrammerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProgrammerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSuperProgrammerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROGRAMMER_H__19C391A5_5FDC_11D3_B648_0000E8732786__INCLUDED_)
