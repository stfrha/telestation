// CListTestView.cpp : implementation of the CPhoneBookView class
//

#include "stdafx.h"
#include "CListTest.h"

#include "CListTestDoc.h"
#include "CListTestView.h"
#include "NewPBEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookView

IMPLEMENT_DYNCREATE(CPhoneBookView, CListView)

BEGIN_MESSAGE_MAP(CPhoneBookView, CListView)
	//{{AFX_MSG_MAP(CPhoneBookView)
	ON_COMMAND(ID_EDIT_NEW_ENTRY, OnEditNewEntry)
	ON_COMMAND(ID_EDIT_DELETE_ENTRY, OnEditDeleteEntry)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_ENTRY, OnUpdateEditDeleteEntry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookView construction/destruction

CPhoneBookView::CPhoneBookView()
{
	// TODO: add construction code here

}

CPhoneBookView::~CPhoneBookView()
{
}

BOOL CPhoneBookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookView drawing

void CPhoneBookView::OnDraw(CDC* pDC)
{
	CPhoneBookDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CPhoneBookView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	DWORD dwStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);

	if ((dwStyle  & LVS_TYPEMASK) != LVS_REPORT) SetWindowLong(this->m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT );

	CListCtrl*	myList;
	
	myList = &GetListCtrl();

	char	col0Head[] = "Number";
	char	col1Head[] = "Name";
	char	col2Head[] = "Category";

	myList->InsertColumn(0, col0Head, LVCFMT_LEFT, 100, -1);
	myList->InsertColumn(1, col1Head, LVCFMT_LEFT, 100, -1);
	myList->InsertColumn(2, col2Head, LVCFMT_LEFT, 100, -1);

	myList->SetExtendedStyle(LVS_EX_GRIDLINES);

	OnUpdate(this, 0, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookView diagnostics

#ifdef _DEBUG
void CPhoneBookView::AssertValid() const
{
	CListView::AssertValid();
}

void CPhoneBookView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPhoneBookDoc* CPhoneBookView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneBookDoc)));
	return (CPhoneBookDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookView message handlers

void CPhoneBookView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CPhoneBookDoc*	myDoc = (CPhoneBookDoc*) GetDocument();
	int				i;
	CString			kat;
	POSITION		pos;
	CPBEntry*		tmpEntry;
	CListCtrl*		myList;
	
	myList = &GetListCtrl();

	pos = myDoc->m_phoneBookList.GetHeadPosition();
	i = 0;

	myList->DeleteAllItems();

	while (pos) {
		tmpEntry = myDoc->m_phoneBookList.GetNext(pos);
		myList->InsertItem(i, tmpEntry->m_number);
		myList->SetItemText(i, 1, tmpEntry->m_name);
		if (myDoc->m_categoryList[tmpEntry->m_category] != "") {
			kat = myDoc->m_categoryList[tmpEntry->m_category];
		} else {
			kat.Format("%d", tmpEntry->m_category);
		}
		myList->SetItemText(i, 2, kat);
		i++;
	}

	Invalidate();
}

void CPhoneBookView::OnEditNewEntry() 
{
	CPhoneBookDoc*	myDoc = (CPhoneBookDoc*) GetDocument();
	CPBEntry*		tmpEntry = (CPBEntry*) new CPBEntry(0);
	CNewPBEntry		newDlg;

	newDlg.m_doc = myDoc;

	if (newDlg.DoModal() == IDOK) {

		tmpEntry->m_name = newDlg.m_name;
		tmpEntry->m_number = newDlg.m_number;
		tmpEntry->m_category = newDlg.m_catSpin;
		myDoc->m_phoneBookList.AddTail(tmpEntry);
		myDoc->SetModifiedFlag();
		myDoc->UpdateAllViews(NULL);
	}
}

void CPhoneBookView::OnEditDeleteEntry() 
{
	CPhoneBookDoc*	myDoc = (CPhoneBookDoc*) GetDocument();
	CListCtrl*		myList;
	int				index;
	
	myList = &GetListCtrl();

	do {
		index = myList->GetNextItem(-1, LVNI_SELECTED);
		if (index != -1) {
			if (myDoc->DeleteEntry(myDoc->FindEntry(myList->GetItemText(index, 0)))) {
				myDoc->SetModifiedFlag();
				myDoc->UpdateAllViews(NULL);
			}
			
		}
	} while (index < myList->GetItemCount());
}


void CPhoneBookView::OnUpdateEditDeleteEntry(CCmdUI* pCmdUI) 
{
	CListCtrl*		myList;
	
	myList = &GetListCtrl();

	pCmdUI->Enable(myList->GetSelectedCount() > 0);
}
