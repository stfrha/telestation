// AnswerScriptDoc.h : interface of the CAnswerScriptDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "ProjectDoc.h"


#if !defined(AFX_AnswerScriptDoc_H__5CEC804C_21FE_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_AnswerScriptDoc_H__5CEC804C_21FE_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//Script Object Types
#define	OT_HAPPY_SUN		0x01
#define	OT_LIFT_PHONE		0x02
#define	OT_DROP_PHONE		0x03
#define	OT_WAIT_CALL		0x04
#define	OT_PLAY_FILE		0x05
#define	OT_ASK_QUESTION		0x06
#define	OT_PROMPT_CODE		0x07
#define	OT_REC_MSG			0x08
#define	OT_ERASE_ALL		0x09
#define	OT_ANSWER_ON		0x0A
#define	OT_ANSWER_OFF		0x0B
#define	OT_PLAY_ALL			0x0C
#define	OT_LABEL			0x0D




#define NUM_TRANS_TYPE	19

//Transition types
#define TT_DEFAULT		0x01
#define TT_DTMF_0		0x02
#define TT_DTMF_1		0x03
#define TT_DTMF_2		0x04
#define TT_DTMF_3		0x05
#define TT_DTMF_4		0x06
#define TT_DTMF_5		0x07
#define TT_DTMF_6		0x08
#define TT_DTMF_7		0x09
#define TT_DTMF_8		0x0A
#define TT_DTMF_9		0x0B
#define TT_DTMF_STAR	0x0C
#define TT_DTMF_POUND	0x0D
#define TT_OTHER_DTMF	0x0E
#define TT_HANGUP		0x0F
#define TT_TIMEOUT		0x10
#define TT_CORRECT_CODE	0x11
#define TT_WRONG_CODE	0x12
#define TT_OK_SIGNALS	0x13

//Transition Type Index codes
#define TTI_DISABLED	1
#define TTI_ENABLED		2
#define TTI_0			1
#define TTI_1			2
#define TTI_USED		3












class CScriptObject;

class CTransition : public CObject {
public:
	CTransition(CScriptObject* srcObject, CScriptObject* dstObject, int transitionType );
protected:
	CTransition( );
	DECLARE_SERIAL( CTransition )

public:
	CScriptObject*	m_srcObject;
	CScriptObject*	m_dstObject;
	int				m_transitionType;
	int				m_srcIdentity;
	int				m_dstIdentity;

protected:
	BOOL			m_selected;
	CRect			m_textRect;
	BOOL			m_invisable;

public:
	BOOL	DrawTransition( CDC* dc);
	BOOL	HideTransition( CDC* dc);
	BOOL	IsHit(CPoint point);
//	BOOl	IsPartOf(CRect rect);
	BOOL	IsSelected() { return m_selected; }
	void	SetSelected(BOOL selected, CDC* dc = NULL);
	void	SetInvisability(BOOL invisable = TRUE);
	virtual void Serialize( CArchive& ar );

protected:
//	CRect	GetTextRect( void );
};











class CScriptObject : public CObject {
public:
	CScriptObject(CPoint startPoint, int objectType, int identity );

protected:
	CScriptObject( );
	DECLARE_SERIAL( CScriptObject )

public:
	CTypedPtrList<CObList, CTransition*> m_toHereList;
	CTypedPtrList<CObList, CTransition*> m_fromHereList;
	CScriptObject* m_clipboardBuddy;

protected:
	CRect	m_rect;
	BOOL	m_selected;
	int		m_objectType;
	BOOL	m_invisable;
	int		m_identity;
	CString	m_labelName;
	CRect	m_textRect;
	CString	m_announcement;
	CString	m_PINcode;
//	int		m_transitionFlags[NUM_TRANS_TYPE];

public:
	BOOL	DrawObject( CDC* dc );
	void	HideObject( CDC* dc );
	void	MoveObject( CPoint dstPoint, CDC* dc );
	void	OffsetObject(CPoint offsetVector, CDC* dc = NULL);
	BOOL	IsHit(CPoint p);
	void	DuplicateObject(CScriptObject* copyMe);
	CPoint	GetFromHerePoint( void );
	CPoint	GetToHerePoint( void );
	CRect	GetRect( void ) { return m_rect; }
	int		GetType( void ) { return m_objectType; }
	int		GetIdentity( void ) { return m_identity; }
	CString	GetLabel( void ) { return m_labelName; }
	CString	GetAnnouncement( void ) { return m_announcement; }
	CString	GetPINCode( void ) { return m_PINcode; }
//	int*	GetTransitionFlagsPtr( void ) { return m_transitionFlags; }
//	int		GetTransFlagAt(int index) { return m_transitionFlags[index]; }
	BOOL	IsSelected() { return m_selected; }
	BOOL	IsPartOf(CRect rect);
	BOOL	IsTransitionOccupied(int transIndex);
	void	SetSelected(BOOL selected, CDC* dc = NULL);
	void	SetInvisability(BOOL invisable = TRUE) { m_invisable = invisable; }
	void	SetLabel(CString label) { m_labelName = label; }
	void	SetPINCode(CString newCode) { m_PINcode = newCode; }
	void	SetAnnouncement(CString newAnouncement) { m_announcement = newAnouncement; }
//	void	SetTransitionFlags(int* src);
//	void	SetTransFlag(int index, int flag) { m_transitionFlags[index] = flag; }
	virtual void Serialize( CArchive& ar );
};









class CAnswerScriptView;

class CAnswerScriptDoc : public CDocument
{
protected: // create from serialization only
	CAnswerScriptDoc();
	DECLARE_DYNCREATE(CAnswerScriptDoc)

// Attributes
public:
	CTypedPtrList<CObList, CScriptObject*>	m_objectList;
	CTypedPtrList<CObList, CScriptObject*>	m_selectionList;
	CTypedPtrList<CObList, CTransition*>	m_transitionList;
	CScriptObject*							m_dragObject;
	CTransition*							m_selectedTransition;


protected:
	CSize	m_sizeDoc;
	BOOL	m_skipNoProperty;

public:
	CSize GetDocSize() { return m_sizeDoc; }


// Operations
public:
	void	OnDrawDoc(CDC* dc);

	CScriptObject*	NewObject(CPoint startPoint, int objectType);
	void			DeleteObject(CScriptObject *deleteMe);
	CScriptObject*	IsObjectHit(CPoint point);

	CTransition*	NewTransition(CScriptObject* srcObj, CScriptObject* dstObj, int transitionType);
	void			DeleteTransition(CTransition* deleteMe);
	CTransition*	IsTransitionHit(CPoint point);

	CAnswerScriptView*	m_currentView;

	CProjectDoc*	m_projectDoc;

	void	ClearSelection( CDC* dc = NULL );
	void	RemoveFromSelection(CScriptObject* removeMe, CDC* dc = NULL);
	void	SetSelection(BOOL select, CRect selectionRect, CDC* dc = NULL);
	void	AddToSelection(CScriptObject* newSelected, CDC* dc = NULL);
	void	AddToSelection(CRect selectionRect, CDC* dc = NULL);
	BOOL	IsSelectionHit(CPoint point);
	void	MoveSelection(CDC* dc, CPoint dstPoint);
	void	ClearClipboard();
	void	HideSelection(CDC* dc);
	int		GetUniqueIdentity( void );
	BOOL	OccupiedIdentity(int i);
	void	PostSerializeProcessData( void );
	CScriptObject*	FindObject(int identity);
	void	AskForProperties( void );
	int		AskForTransitionType(CScriptObject* srcObj);
	void	SetProjectDoc(CProjectDoc* doc) { m_projectDoc = doc; }
	void	InvalidateProjectDoc( void );


	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnswerScriptDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
//	CScriptObject* m_oneObject;
	virtual ~CAnswerScriptDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void InitDocument( );
	//{{AFX_MSG(CAnswerScriptDoc)
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditPaste();
	afx_msg void OnObjectProp();
	afx_msg void OnUpdateObjectProp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AnswerScriptDoc_H__5CEC804C_21FE_11D4_9855_0000E873277A__INCLUDED_)
