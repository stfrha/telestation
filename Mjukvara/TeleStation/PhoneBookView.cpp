// PhoneBookView.cpp : implementation of the CPhoneBookView class
//

#include "stdafx.h"
#include "TeleStation.h"

#include "PhoneBookDoc.h"
#include "PhoneBookView.h"
#include "NewPBEntryDlg.h"

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
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PB_ENTRY_PROPERTIES, OnPbEntryProperties)
	ON_UPDATE_COMMAND_UI(ID_PB_ENTRY_PROPERTIES, OnUpdatePbEntryProperties)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
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

	if ((dwStyle  & LVS_TYPEMASK) != LVS_REPORT) SetWindowLong(this->m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

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

	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();
	CProjectDoc*	projDoc = myApp->GetCurrentProjDoc();

	CPhoneBookDoc* pDoc = GetDocument();
	if (pDoc) {
		if (pDoc->m_projectDoc) {
			pDoc->m_projectDoc->LoadViewPlacement(this->GetParentFrame(), 3);
		}
	}
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
	CPBEntry*		tmpEntry;
	CNewPBEntryDlg	newDlg;

	newDlg.PopulateList(myDoc);

	if (newDlg.DoModal() == IDOK) {
		if (!newDlg.m_number.IsEmpty()) {
			if (tmpEntry = myDoc->FindEntry(newDlg.m_number)) {
				tmpEntry->m_name = newDlg.m_name;
				tmpEntry->m_category = newDlg.m_catList;
			} else {
				tmpEntry = (CPBEntry*) new CPBEntry(0);
				tmpEntry->m_name = newDlg.m_name;
				tmpEntry->m_number = newDlg.m_number;
				tmpEntry->m_category = newDlg.m_catList;
				myDoc->m_phoneBookList.AddTail(tmpEntry);
			}
			myDoc->SetModifiedFlag();
			myDoc->UpdateAllViews(NULL);
		}
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
	} while ((index < myList->GetItemCount()) && (index != -1));
}


void CPhoneBookView::OnUpdateEditDeleteEntry(CCmdUI* pCmdUI) 
{
	CListCtrl*		myList;
	
	myList = &GetListCtrl();

	pCmdUI->Enable(myList->GetSelectedCount() > 0);
}


void CPhoneBookView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EditEntry();

	*pResult = 0;
}


void CPhoneBookView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMenu menu;    
	CMenu *submenu;    
	// Load the menu.
	menu.LoadMenu(IDR_EDIT_PB_POPUP);    
	// Get the pop-up menu.
	submenu = menu.GetSubMenu(0);    
	// Convert to screen coordinates.
	ClientToScreen(&point);    
	// Post the menu.
	submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);

//	CListView::OnRButtonUp(nFlags, point);
}

void CPhoneBookView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl*		myList;
	int				hitIndex, i;

	myList = &GetListCtrl();

	if (!(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT)) {
		for (i=0 ; i<myList->GetItemCount() ; i++) {
			myList->SetItemState(i, 0, LVIS_SELECTED);
		}
		hitIndex = myList->HitTest(point);

		if (hitIndex > -1) {
			myList->SetItemState(hitIndex, LVIS_SELECTED, LVIS_SELECTED);
			myList->SetItemState(hitIndex, LVIS_FOCUSED, LVIS_FOCUSED);
		}
	}


//	Verkar bara ge ett OnRButtonUp meddelande om man inte hanterar defaultmedtoden för RButtonDown
//	CListView::OnRButtonDown(nFlags, point);
}

void CPhoneBookView::EditEntry( void ) 
{
	CPhoneBookDoc*	myDoc = (CPhoneBookDoc*) GetDocument();
	CListCtrl*		myList;
	CNewPBEntryDlg	entryDlg;
	POSITION		pos;
	int				selIndex;
	CPBEntry*		entry;
	CPBEntry*		tmpEntry;
	
	myList = &GetListCtrl();

	if (myList->GetSelectedCount() == 1) {

		entryDlg.m_windowTitle = "Edit Phone Book Entry";

		pos = myList->GetFirstSelectedItemPosition();
		if (pos) {
			selIndex = myList->GetNextSelectedItem(pos);
			
			entry = myDoc->FindEntry(myList->GetItemText(selIndex, 0));

			entryDlg.PopulateList(myDoc);

			entryDlg.m_name = entry->m_name;
			entryDlg.m_number = entry->m_number;
			entryDlg.m_catList = entry->m_category;

			if (entryDlg.DoModal() == IDOK) {
				tmpEntry = myDoc->FindEntry(entryDlg.m_number);
				tmpEntry->m_name = entryDlg.m_name;
				tmpEntry->m_category = entryDlg.m_catList;
				myDoc->SetModifiedFlag();
				myDoc->UpdateAllViews(NULL);
			}
		}
	} else if (myList->GetSelectedCount() > 1) {
		entryDlg.PopulateList(myDoc);
		entryDlg.SetMultiSel(TRUE);
		entryDlg.m_name = "";
		entryDlg.m_number = "";
		entryDlg.m_catList = 0;
		entryDlg.m_windowTitle = "Change category for selection";
		if (entryDlg.DoModal() == IDOK) {
			pos = myList->GetFirstSelectedItemPosition();
			while (pos) {
				selIndex = myList->GetNextSelectedItem(pos);
				entry = myDoc->FindEntry(myList->GetItemText(selIndex, 0));
				if (entry) entry->m_category = entryDlg.m_catList;
			}
			myDoc->SetModifiedFlag();
			myDoc->UpdateAllViews(NULL);
		}
	}
}

void CPhoneBookView::OnPbEntryProperties() 
{
	EditEntry();
}

void CPhoneBookView::OnUpdatePbEntryProperties(CCmdUI* pCmdUI) 
{
	CListCtrl*		myList;
	
	myList = &GetListCtrl();

	pCmdUI->Enable(myList->GetSelectedCount() > 0);
}

void CPhoneBookView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListView::OnBeginPrinting(pDC, pInfo);
}

BOOL CPhoneBookView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return CListView::OnPreparePrinting(pInfo);
}

void CPhoneBookView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListView::OnEndPrinting(pDC, pInfo);
}
