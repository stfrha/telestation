#if !defined(AFX_WORKSPACEVIEW_H__8295A276_323B_11D4_AD7C_0060084CBD8A__INCLUDED_)
#define AFX_WORKSPACEVIEW_H__8295A276_323B_11D4_AD7C_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkspaceView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView view

class CWorkspaceView : public CTreeView
{
protected:
	CWorkspaceView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWorkspaceView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWorkspaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEVIEW_H__8295A276_323B_11D4_AD7C_0060084CBD8A__INCLUDED_)
