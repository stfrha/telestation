// CListTest.h : main header file for the CLISTTEST application
//

#if !defined(AFX_CLISTTEST_H__67BE4BF1_21C2_4B58_A1CE_6016C520839B__INCLUDED_)
#define AFX_CLISTTEST_H__67BE4BF1_21C2_4B58_A1CE_6016C520839B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CListTestDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCListTestApp:
// See CListTest.cpp for the implementation of this class
//

class CCListTestApp : public CWinApp
{
public:
	CCListTestApp();

	CMultiDocTemplate*	m_catListTemplate;

	//void	ActivateCatView(CCListTestDoc* doc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCListTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCListTestApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLISTTEST_H__67BE4BF1_21C2_4B58_A1CE_6016C520839B__INCLUDED_)
