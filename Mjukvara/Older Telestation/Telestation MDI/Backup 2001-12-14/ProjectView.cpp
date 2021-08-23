// ProjectView.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "ProjectView.h"
#include "ProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CTreeView)

CProjectView::CProjectView()
{
}

CProjectView::~CProjectView()
{
}


BEGIN_MESSAGE_MAP(CProjectView, CTreeView)
	//{{AFX_MSG_MAP(CProjectView)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectView drawing

void CProjectView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CProjectView diagnostics

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CProjectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CProjectDoc* CProjectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectView message handlers

void CProjectView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();

	CProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	SetWindowLong(m_hWnd, GWL_STYLE,	GetWindowLong(m_hWnd, GWL_STYLE) |
										TVS_DISABLEDRAGDROP | TVS_HASBUTTONS |
										TVS_HASLINES | TVS_LINESATROOT);

	CTreeCtrl& tc = GetTreeCtrl();
	
	TV_INSERTSTRUCT TreeCtrlItem;

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = "";
	TreeCtrlItem.item.lParam = TSF_PROJECT_NAME;
	m_projectNameItem = tc.InsertItem(&TreeCtrlItem);
	tc.SetItemText(m_projectNameItem, pDoc->GetTitle());

	TreeCtrlItem.hParent = m_projectNameItem;
	TreeCtrlItem.item.pszText = "Script File";
	TreeCtrlItem.item.lParam = TSF_SCRIPT_HEAD;
	m_scriptHeadItem = tc.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = m_scriptHeadItem;
	TreeCtrlItem.item.pszText = "None";
	TreeCtrlItem.item.lParam = TSF_SCRIPT_FILE;
	m_scriptItem = tc.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = m_projectNameItem;
	TreeCtrlItem.item.pszText = "Announcements";
	TreeCtrlItem.item.lParam = TSF_ANNOUNCE_HEAD;
	m_announcementsHeadItem = tc.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Phone Book File";
	TreeCtrlItem.item.lParam = TSF_PHONE_BOOK_HEAD;
	m_phoneBookHeadItem = tc.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = m_phoneBookHeadItem;
	TreeCtrlItem.item.pszText = "None";
	TreeCtrlItem.item.lParam = TSF_PHONE_BOOK_FILE;
	m_phoneBookItem = tc.InsertItem(&TreeCtrlItem);

	pDoc->SetCurrentView(this);

	pDoc->PostCreationProcessData();




}

void CProjectView::SetScriptFile(CString name)
{
	CTreeCtrl& tc = GetTreeCtrl();

	tc.SetItemText(m_scriptItem, name);
/*

	//Ta bort den gammla filvalet!!!!!!!

	TV_INSERTSTRUCT TreeCtrlItem;

	TreeCtrlItem.hParent = m_scriptHeadItem;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT;
	TreeCtrlItem.item.pszText = name.GetBuffer(1);
	return tc.InsertItem(&TreeCtrlItem);
	*/
}

void CProjectView::SetPhoneBookFile(CString name)
{
	CTreeCtrl& tc = GetTreeCtrl();

	tc.SetItemText(m_phoneBookItem, name);
/*
	TV_INSERTSTRUCT TreeCtrlItem;

	//Ta bort den gammla filvalet!!!!!!!

	TreeCtrlItem.hParent = m_phoneBookHeadItem;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT;
	TreeCtrlItem.item.pszText = name.GetBuffer(1);
	return tc.InsertItem(&TreeCtrlItem);
	*/
}

HTREEITEM CProjectView::InsertAnnouncementFile(CString name)
{
	CTreeCtrl& tc = GetTreeCtrl();

	TV_INSERTSTRUCT TreeCtrlItem;

	TreeCtrlItem.hParent = m_announcementsHeadItem;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = name.GetBuffer(1);
	TreeCtrlItem.item.lParam = TSF_ANNOUNCE_FILE;
	return tc.InsertItem(&TreeCtrlItem);
}


void CProjectView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl& tc = GetTreeCtrl();

	unsigned int	flags;
	HTREEITEM		foundItem = tc.HitTest(point, &flags);

	if (foundItem) {
		tc.SelectItem(foundItem);

		UpdateSelectionFlag();
	}
}

void CProjectView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMenu menu;    
	CMenu *submenu;    

	// Load the menu.

	switch (m_treeSelectionFlag) {
	case TSF_SCRIPT_HEAD:
	case TSF_SCRIPT_FILE:
		menu.LoadMenu(IDR_SCRIPT_POPUP);
		submenu = menu.GetSubMenu(0);    
		ClientToScreen(&point);    
		submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);
		break;
	case TSF_ANNOUNCE_HEAD:
	case TSF_ANNOUNCE_FILE:
		menu.LoadMenu(IDR_ANNOUNCE_POPUP);
		submenu = menu.GetSubMenu(0);    
		ClientToScreen(&point);    
		submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);
		break;
	case TSF_PHONE_BOOK_HEAD:
	case TSF_PHONE_BOOK_FILE:
		menu.LoadMenu(IDR_PHONE_BOOK_POPUP);
		submenu = menu.GetSubMenu(0);    
		ClientToScreen(&point);    
		submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);
		break;
	}

}


void CProjectView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnLButtonDown(nFlags, point);

	UpdateSelectionFlag();
}

void CProjectView::UpdateSelectionFlag( void )
{
	CTreeCtrl& tc = GetTreeCtrl();

	m_treeSelectionFlag = tc.GetItemData(tc.GetSelectedItem());
}

