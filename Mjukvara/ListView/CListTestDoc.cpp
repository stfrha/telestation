// CListTestDoc.cpp : implementation of the CCListTestDoc class
//

#include "stdafx.h"
#include "CListTest.h"

#include "CListTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCListTestDoc

IMPLEMENT_DYNCREATE(CCListTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CCListTestDoc, CDocument)
	//{{AFX_MSG_MAP(CCListTestDoc)
	ON_COMMAND(ID_VIEW_SHOW_CATEGORIES, OnViewShowCategories)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCListTestDoc construction/destruction

CCListTestDoc::CCListTestDoc()
{
}

CCListTestDoc::~CCListTestDoc()
{
}

BOOL CCListTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	int	i;

	for (i=0 ; i<256 ; i++) {
		m_categoryList[i] = "";
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCListTestDoc serialization

void CCListTestDoc::Serialize(CArchive& ar)
{
	int	i;

	m_phoneBookList.Serialize( ar );

	if (ar.IsStoring()) {
		for (i=0 ; i<256 ; i++) {
			ar << m_categoryList[i];
		}
	} else {
		for (i=0 ; i<256 ; i++) {
			ar >> m_categoryList[i];
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCListTestDoc diagnostics

#ifdef _DEBUG
void CCListTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCListTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCListTestDoc commands

CPBEntry* CCListTestDoc::FindEntry(CString number)
{
	POSITION	pos;
	CPBEntry*	tmpEntry;

	pos = m_phoneBookList.GetHeadPosition();

	while (pos) {
		tmpEntry = m_phoneBookList.GetNext(pos);
		if (tmpEntry->m_number == number) return tmpEntry;
	}
	return NULL;
}


BOOL CCListTestDoc::DeleteEntry(CPBEntry* deleteMe)
{
	POSITION	pos;

	pos = m_phoneBookList.Find(deleteMe);
	if (pos) {
		m_phoneBookList.RemoveAt(pos);
		delete deleteMe;
		return TRUE;
	}
	return FALSE;
}

extern void ActivateCatView(CCListTestDoc* doc);


void CCListTestDoc::OnViewShowCategories() 
{
	CView*	tmpView;

	POSITION pos;

	pos = GetFirstViewPosition();

	int i = 0;

	while (pos) {
		i++;
		tmpView = GetNextView(pos);
		if (!tmpView->IsWindowVisible()) tmpView->ShowWindow(SW_SHOW);
	}
	CString msg;

	msg.Format("Number of views: %d", i);
	MessageBox(NULL, msg, "Hello", MB_OK);

	

	ActivateCatView(this);



	pos = GetFirstViewPosition();

	i = 0;

	while (pos) {
		i++;
		tmpView = GetNextView(pos);
		if (!tmpView->IsWindowVisible()) {
			MessageBox(NULL, "Invisable window detected", "Hello", MB_OK);
			tmpView->ShowWindow(SW_SHOWNORMAL);
		}
	}

	msg.Format("Number of views: %d", i);
	MessageBox(NULL, msg, "Hello", MB_OK);

	pos = GetFirstViewPosition();

	i = 0;

	while (pos) {
		i++;
		tmpView = GetNextView(pos);
		if (!tmpView->IsWindowVisible()) {
			MessageBox(NULL, "Invisable window detected", "Hello", MB_OK);
			tmpView->ShowWindow(SW_SHOWNORMAL);
		}
	}

	msg.Format("Number of views: %d", i);
	MessageBox(NULL, msg, "Hello", MB_OK);


}






















/////////////////////////////////////////////////////////////////////////////
// Class CPBEntry


IMPLEMENT_SERIAL( CPBEntry, CObject, 1 )


void	CPBEntry::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) ) {
		ar << m_name;
		ar << m_number;
		ar << m_category;
	} else {
		ar >> m_name;
		ar >> m_number;
		ar >> m_category;
	}
}








