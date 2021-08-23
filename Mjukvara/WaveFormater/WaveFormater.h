// WaveFormater.h : main header file for the WAVEFORMATER application
//

#if !defined(AFX_WAVEFORMATER_H__22D59267_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_)
#define AFX_WAVEFORMATER_H__22D59267_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWaveFormaterApp:
// See WaveFormater.cpp for the implementation of this class
//

class CWaveFormaterApp : public CWinApp
{
public:
	CWaveFormaterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveFormaterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWaveFormaterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEFORMATER_H__22D59267_4D8C_11D4_AD90_0060084CBD8A__INCLUDED_)
