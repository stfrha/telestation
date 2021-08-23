// CListTestView.h : interface of the CCListTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTTESTVIEW_H__13E75936_3CE8_4A39_8DA6_7502755DF4D0__INCLUDED_)
#define AFX_CLISTTESTVIEW_H__13E75936_3CE8_4A39_8DA6_7502755DF4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCListTestView : public CListView
{
protected: // create from serialization only
	CCListTestView();
	DECLARE_DYNCREATE(CCListTestView)

// Attributes
public:
	CCListTestDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCListTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCListTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCListTestView)
	afx_msg void OnEditNewEntry();
	afx_msg void OnEditDeleteEntry();
	afx_msg void OnUpdateEditDeleteEntry(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CListTestView.cpp
inline CCListTestDoc* CCListTestView::GetDocument()
   { return (CCListTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLISTTESTVIEW_H__13E75936_3CE8_4A39_8DA6_7502755DF4D0__INCLUDED_)
