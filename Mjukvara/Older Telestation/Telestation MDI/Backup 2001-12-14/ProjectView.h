#if !defined(AFX_PROJECTVIEW_H__C9759E21_37F9_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_PROJECTVIEW_H__C9759E21_37F9_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectView.h : header file
//


#define TSF_NONE			0x01
#define TSF_PROJECT_NAME	0x02
#define TSF_SCRIPT_HEAD		0x03
#define TSF_SCRIPT_FILE		0x04
#define TSF_ANNOUNCE_HEAD	0x05
#define TSF_ANNOUNCE_FILE	0x06
#define TSF_PHONE_BOOK_HEAD	0x07
#define TSF_PHONE_BOOK_FILE	0x08



/////////////////////////////////////////////////////////////////////////////
// CProjectView view

class CProjectDoc;

class CProjectView : public CTreeView
{
protected:
	CProjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProjectView)

// Attributes
public:
	HTREEITEM	m_projectNameItem;
	HTREEITEM	m_scriptHeadItem;
	HTREEITEM	m_announcementsHeadItem;
	HTREEITEM	m_phoneBookHeadItem;
	HTREEITEM	m_scriptItem;
	HTREEITEM	m_phoneBookItem;
	int			m_treeSelectionFlag;
//	BOOL		m_announcementsSelected;


// Operations
public:
	CProjectDoc* GetDocument();

	void		SetScriptFile( CString name );
	void		SetPhoneBookFile( CString name );
	HTREEITEM	InsertAnnouncementFile( CString name );
	void		UpdateSelectionFlag( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTVIEW_H__C9759E21_37F9_11D4_9855_0000E873277A__INCLUDED_)
