// CListTestDoc.h : interface of the CCListTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTTESTDOC_H__D333A899_7F9D_41A5_814E_B37DC2F273DF__INCLUDED_)
#define AFX_CLISTTESTDOC_H__D333A899_7F9D_41A5_814E_B37DC2F273DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CatView.h"


class CPBEntry : public CObject {
public:
	CPBEntry(CString name, CString number, int category);
	CPBEntry(int category) { m_category = category; }
protected:
	CPBEntry() {m_name = ""; m_number = ""; m_category = 0; }
	DECLARE_SERIAL( CPBEntry )

public:
	CString		m_name;
	CString		m_number;
	int			m_category;

	virtual void Serialize( CArchive& ar );
};



class CCListTestDoc : public CDocument
{
protected: // create from serialization only
	CCListTestDoc();
	DECLARE_DYNCREATE(CCListTestDoc)

// Attributes
public:
	CString								m_categoryList[256];

	CTypedPtrList<CObList, CPBEntry*>	m_phoneBookList;

// Operations
public:
	CPBEntry*		FindEntry(CString number);
	BOOL			DeleteEntry(CPBEntry* deleteMe);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCListTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCListTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCListTestDoc)
	afx_msg void OnViewShowCategories();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLISTTESTDOC_H__D333A899_7F9D_41A5_814E_B37DC2F273DF__INCLUDED_)
