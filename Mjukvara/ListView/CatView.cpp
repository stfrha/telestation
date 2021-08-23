// CatView.cpp : implementation file
//

#include "stdafx.h"
#include "CListTest.h"
#include "CatView.h"
#include "CListTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatView

IMPLEMENT_DYNCREATE(CCatView, CListView)

CCatView::CCatView()
{
}

CCatView::~CCatView()
{
}


BEGIN_MESSAGE_MAP(CCatView, CListView)
	//{{AFX_MSG_MAP(CCatView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatView drawing

void CCatView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCatView diagnostics

#ifdef _DEBUG
void CCatView::AssertValid() const
{
	CListView::AssertValid();
}

void CCatView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCatView message handlers

void CCatView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();	
	
	DWORD dwStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);

	if ((dwStyle  & LVS_TYPEMASK) != LVS_REPORT) SetWindowLong(this->m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT );

	CListCtrl*	myList;
	
	myList = &GetListCtrl();

	char	col0Head[] = "Category";
	char	col1Head[] = "Explenation";

	myList->InsertColumn(0, col0Head, LVCFMT_LEFT, 100, -1);
	myList->InsertColumn(1, col1Head, LVCFMT_LEFT, 100, -1);

	myList->SetExtendedStyle(LVS_EX_GRIDLINES);

	OnUpdate(this, 0, NULL);
}



void CCatView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CCListTestDoc*	myDoc = (CCListTestDoc*) GetDocument();
	int				j, i;
	CString			cat;
	CListCtrl*		myList;
	
	myList = &GetListCtrl();

	i = 0;

	myList->DeleteAllItems();

	for (j=0 ; j<256 ; j++) {
		if (myDoc->m_categoryList[j] != "") {
			cat.Format("%d", j);
			myList->InsertItem(i, cat);
			myList->SetItemText(i, 1, myDoc->m_categoryList[j]);
			i++;
		}
	}
	
	Invalidate();
	
}

