// Dumper3.h : main header file for the DUMPER3 application
//

#if !defined(AFX_DUMPER3_H__40D24D46_CEB6_11D1_918D_0060084CBD8A__INCLUDED_)
#define AFX_DUMPER3_H__40D24D46_CEB6_11D1_918D_0060084CBD8A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDumper3App:
// See Dumper3.cpp for the implementation of this class
//

class CDumper3App : public CWinApp
{
public:
	CDumper3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDumper3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDumper3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMPER3_H__40D24D46_CEB6_11D1_918D_0060084CBD8A__INCLUDED_)
