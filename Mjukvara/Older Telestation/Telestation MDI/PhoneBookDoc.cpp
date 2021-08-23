// CListTestDoc.cpp : implementation of the CPhoneBookDoc class
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
// CPhoneBookDoc

IMPLEMENT_DYNCREATE(CPhoneBookDoc, CDocument)

BEGIN_MESSAGE_MAP(CPhoneBookDoc, CDocument)
	//{{AFX_MSG_MAP(CPhoneBookDoc)
	ON_COMMAND(ID_VIEW_SHOW_CATEGORIES, OnViewShowCategories)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc construction/destruction

CPhoneBookDoc::CPhoneBookDoc()
{
}

CPhoneBookDoc::~CPhoneBookDoc()
{
}

BOOL CPhoneBookDoc::OnNewDocument()
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
// CPhoneBookDoc serialization

void CPhoneBookDoc::Serialize(CArchive& ar)
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
// CPhoneBookDoc diagnostics

#ifdef _DEBUG
void CPhoneBookDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhoneBookDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc commands

CPBEntry* CPhoneBookDoc::FindEntry(CString number)
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


BOOL CPhoneBookDoc::DeleteEntry(CPBEntry* deleteMe)
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

extern void ActivateCatView(CPhoneBookDoc* doc);


void CPhoneBookDoc::OnViewShowCategories() 
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








