#if !defined(AFX_CATVIEW_H__A37014E5_03FC_48BA_BBE2_EAF6777940BD__INCLUDED_)
#define AFX_CATVIEW_H__A37014E5_03FC_48BA_BBE2_EAF6777940BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCatView view

class CCatView : public CListView
{
protected:
	CCatView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCatView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCatView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATVIEW_H__A37014E5_03FC_48BA_BBE2_EAF6777940BD__INCLUDED_)
