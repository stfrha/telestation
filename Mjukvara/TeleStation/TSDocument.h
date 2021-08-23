#if !defined(AFX_TSDOCUMENT_H__5AC2F821_0F99_4D03_ADE6_2F18A13F75D8__INCLUDED_)
#define AFX_TSDOCUMENT_H__5AC2F821_0F99_4D03_ADE6_2F18A13F75D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TSDocument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTSDocument document

class CTSDocument : public CDocument
{
protected:
	CTSDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTSDocument)

// Attributes
public:
	CProjectDoc*	m_projectDoc;
	BOOL			m_openedByProject;

// Operations
public:
	void	SetProjectDoc(CProjectDoc* doc) { m_projectDoc = doc; }
	void	InvalidateProjectDoc( void );
	BOOL	GetCurrentWndPlacement(WINDOWPLACEMENT* placement);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTSDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTSDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTSDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSDOCUMENT_H__5AC2F821_0F99_4D03_ADE6_2F18A13F75D8__INCLUDED_)
