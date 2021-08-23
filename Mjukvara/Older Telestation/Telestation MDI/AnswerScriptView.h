// AnswerScriptView.h : interface of the CAnswerScriptView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AnswerScriptView_H__5CEC804E_21FE_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_AnswerScriptView_H__5CEC804E_21FE_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define DS_NONE					0x00
//#define DS_MOVE_OBJECT			0x01
#define DS_SELECTION			0x02
#define	DS_MOVE_SELECTION		0x03
//#define DS_PLACE_OBJECT			0x04
//#define DS_PRE_MOVE_OBJECT		0x05
#define DS_PRE_MOVE_SELECTION	0x06
#define DS_PLACE_SELECTION		0x07
#define DS_PLACE_TRANSITION		0x08

class CDragRect : public CObject {
public:
	CDragRect(CRect rect, CScriptObject* object) { m_rect = rect; m_object = object; }
protected:
	CDragRect();

public:
	CRect			m_rect;
	CScriptObject*	m_object;
};

class CDragLine : public CObject {
public:
	CDragLine(CPoint startP, BOOL startLock, CPoint endP, BOOL endLock) {
		m_startP = startP;
		m_startLock = startLock;
		m_endP = endP;
		m_endLock = endLock;
	}
protected:
	CDragLine();

public:
	CPoint	m_startP;
	CPoint	m_endP;
	BOOL	m_startLock;
	BOOL	m_endLock;
};






class CAnswerScriptView : public CScrollView
{
protected: // create from serialization only
	CAnswerScriptView();
	DECLARE_DYNCREATE(CAnswerScriptView)

// Attributes
public:
	CPoint			m_preDragPoint;
	CPoint			m_oldTransitionEndPoint;
	BOOL			m_transitionActive;
	CScriptObject*	m_dragObject;
	CRect			m_oldSelectionRect;
	int				m_dragState;
	CPoint			m_selectionStartPoint;
	CTypedPtrList<CObList, CDragRect*>	m_dragRectList;
	CTypedPtrList<CObList, CDragLine*>	m_dragLineList;
	BOOL			m_dragListShown;
	CPoint			m_oldDragPoint;
	BOOL			m_disablePopup;
//	CRect			m_dragRect;


// Operations
public:
	CAnswerScriptDoc* GetDocument();

	void			ClearDragList( void );
	void			AddToDragList( CRect rect, CScriptObject* object);
	void			AddToDragList( CPoint startP, BOOL startLock, CPoint endP, BOOL endLock);
	void			ShowDragList(CPoint point);
	void			HideDragList( void );
	CRect			GetDragListTestRect( CPoint point);
	void			SelectionToDragList(CPoint point);
	int				PointToPointDistance(CPoint p1, CPoint p2);
	void			OnEditPaste( void );
	void			OnObjectNew(int objectType);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnswerScriptView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnswerScriptView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:




	//{{AFX_MSG(CAnswerScriptView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnObjectAddTransition();
	afx_msg void OnUpdateObjectAddTransition(CCmdUI* pCmdUI);
	afx_msg void OnScriptDropPhone();
	afx_msg void OnScriptLiftPhone();
	afx_msg void OnScriptWaitForCall();
	afx_msg void OnScriptPlay();
	afx_msg void OnScriptAskQuestion();
	afx_msg void OnScriptPromptCode();
	afx_msg void OnScriptRecMsg();
	afx_msg void OnScriptEraseMsg();
	afx_msg void OnScriptAnswerOn();
	afx_msg void OnScriptAnswerOff();
	afx_msg void OnScriptPlayAll();
	afx_msg void OnScriptInsertLabel();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCustomCancel();
	afx_msg void OnUpdateCustomCancel(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AnswerScriptView.cpp
inline CAnswerScriptDoc* CAnswerScriptView::GetDocument()
   { return (CAnswerScriptDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AnswerScriptView_H__5CEC804E_21FE_11D4_9855_0000E873277A__INCLUDED_)
