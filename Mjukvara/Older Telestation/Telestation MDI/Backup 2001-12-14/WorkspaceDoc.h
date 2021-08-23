#if !defined(AFX_WORKSPACEDOC_H__8295A277_323B_11D4_AD7C_0060084CBD8A__INCLUDED_)
#define AFX_WORKSPACEDOC_H__8295A277_323B_11D4_AD7C_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkspaceDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc document

class CWorkspaceDoc : public CDocument
{
protected:
	CWorkspaceDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWorkspaceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEDOC_H__8295A277_323B_11D4_AD7C_0060084CBD8A__INCLUDED_)
