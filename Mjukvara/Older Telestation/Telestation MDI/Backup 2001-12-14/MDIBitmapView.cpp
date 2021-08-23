// MDIBitmapView.cpp : implementation of the CMDIBitmapView class
//

#include "stdafx.h"
#include "MDIBitmap.h"

#include "MDIBitmapDoc.h"
#include "MDIBitmapView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CTypedPtrList<CObList, CScriptObject*> clipboardObjectList;
extern CTypedPtrList<CObList, CTransition*> clipboardTransitionList;

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView

IMPLEMENT_DYNCREATE(CMDIBitmapView, CScrollView)

BEGIN_MESSAGE_MAP(CMDIBitmapView, CScrollView)
	//{{AFX_MSG_MAP(CMDIBitmapView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OBJECT_ADD_TRANSITION, OnObjectAddTransition)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ADD_TRANSITION, OnUpdateObjectAddTransition)
	ON_COMMAND(ID_SCRIPT_DROP_PHONE, OnScriptDropPhone)
	ON_COMMAND(ID_SCRIPT_LIFT_PHONE, OnScriptLiftPhone)
	ON_COMMAND(ID_SCRIPT_WAIT_FOR_CALL, OnScriptWaitForCall)
	ON_COMMAND(ID_SCRIPT_PLAY, OnScriptPlay)
	ON_COMMAND(ID_SCRIPT_ASK_QUESTION, OnScriptAskQuestion)
	ON_COMMAND(ID_SCRIPT_PROMPT_CODE, OnScriptPromptCode)
	ON_COMMAND(ID_SCRIPT_REC_MSG, OnScriptRecMsg)
	ON_COMMAND(ID_SCRIPT_ERASE_MSG, OnScriptEraseMsg)
	ON_COMMAND(ID_SCRIPT_ANSWER_ON, OnScriptAnswerOn)
	ON_COMMAND(ID_SCRIPT_ANSWER_OFF, OnScriptAnswerOff)
	ON_COMMAND(ID_SCRIPT_PLAY_ALL, OnScriptPlayAll)
	ON_COMMAND(ID_SCRIPT_INSERT_LABEL, OnScriptInsertLabel)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CUSTOM_CANCEL, OnCustomCancel)
	ON_UPDATE_COMMAND_UI(ID_CUSTOM_CANCEL, OnUpdateCustomCancel)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView construction/destruction

CMDIBitmapView::CMDIBitmapView()
{
	// TODO: add construction code here
	m_transitionActive = FALSE;
	m_dragListShown = FALSE;
	m_disablePopup = FALSE;
	m_dragState = DS_NONE;

}

CMDIBitmapView::~CMDIBitmapView()
{
	ClearDragList();
}

BOOL CMDIBitmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView drawing

void CMDIBitmapView::OnDraw(CDC* pDC)
{
	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	pDoc->OnDrawDoc(pDC);

}

void CMDIBitmapView::OnInitialUpdate()
{
	SetScrollSizes( MM_TEXT , GetDocument()->GetDocSize() );

	CScrollView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView printing

BOOL CMDIBitmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMDIBitmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMDIBitmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView diagnostics

#ifdef _DEBUG
void CMDIBitmapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMDIBitmapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMDIBitmapDoc* CMDIBitmapView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIBitmapDoc)));
	return (CMDIBitmapDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapView message handlers

void CMDIBitmapView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);
	
	CPoint	displacedPoint = point + GetScrollPosition();;

	CRect	docRect(CPoint(0, 0), pDoc->GetDocSize());
	CRect	testRect;

	POSITION	pos;
	POSITION	pos2;

	if (m_transitionActive) {

		m_dragObject = pDoc->IsObjectHit(displacedPoint);
		
		if (m_dragObject) {
			ClearDragList();
			AddToDragList(m_dragObject->GetFromHerePoint(), TRUE, CPoint(0, 0), FALSE);

			m_dragState = DS_PLACE_TRANSITION;

			SetCapture();

		} else {
			pDoc->ClearSelection(&dc);
			m_transitionActive = FALSE;
		}
		return;
	}
	
	if (m_dragState == DS_PLACE_SELECTION) {

		testRect = GetDragListTestRect(displacedPoint);

		if (testRect.left < docRect.left) displacedPoint.Offset(CPoint(docRect.left-testRect.left, 0));
		if (testRect.top < docRect.top) displacedPoint.Offset(CPoint(0, docRect.top-testRect.top));
		if (testRect.right > docRect.right) displacedPoint.Offset(CPoint(docRect.right-testRect.right, 0));
		if (testRect.bottom > docRect.bottom) displacedPoint.Offset(CPoint(0, docRect.bottom-testRect.bottom));

		CDragRect*	tmp;

		pos = m_dragRectList.GetHeadPosition();
		while (pos != NULL) {
			tmp = m_dragRectList.GetNext(pos);
			tmp->m_object->MoveObject(displacedPoint + tmp->m_rect.TopLeft(), NULL);
			tmp->m_object->SetInvisability(FALSE);

			pos2 = tmp->m_object->m_fromHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_fromHereList.GetNext(pos2)->SetInvisability(FALSE);
			}

			pos2 = tmp->m_object->m_toHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_toHereList.GetNext(pos2)->SetInvisability(FALSE);
			}

		}

		HideDragList();

		m_dragState = DS_NONE;

		ReleaseCapture();

		pDoc->SetModifiedFlag(TRUE);

		pDoc->UpdateAllViews(NULL);

		pDoc->AskForProperties();

		return;
	}

	if (pDoc->IsSelectionHit(displacedPoint)) {

		m_dragObject = pDoc->IsObjectHit(displacedPoint);

		if (nFlags & MK_SHIFT) {
			pDoc->RemoveFromSelection(m_dragObject, &dc);
		} else {
			m_preDragPoint = displacedPoint;
			m_dragState = DS_PRE_MOVE_SELECTION;
		}
	} else {

		m_dragObject = pDoc->IsObjectHit(displacedPoint);

		if (m_dragObject) {

			if (!(nFlags & MK_SHIFT)) {
				pDoc->ClearSelection(&dc);
				m_dragState = DS_PRE_MOVE_SELECTION;
			}

			pDoc->AddToSelection(m_dragObject, &dc);
			m_preDragPoint = displacedPoint;

			//Om Shift hålls ner skall objektet endast markeras inte flyttas
			
		} else {
			CTransition*	tmpTrans;
			tmpTrans = pDoc->IsTransitionHit(displacedPoint);
			if (tmpTrans) {
				pDoc->ClearSelection(&dc);
				tmpTrans->SetSelected(TRUE, &dc);
				pDoc->m_selectedTransition = tmpTrans;
			} else {
				m_selectionStartPoint = displacedPoint;
				m_oldSelectionRect.SetRectEmpty();
				m_dragState = DS_SELECTION;
			}
		}
	}

	SetCapture();

	return;
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CMDIBitmapView::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if( GetCapture( ) != this )	return;

	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);

	CPoint	displacedPoint;
	displacedPoint = point + GetScrollPosition();

	CRect	selectionRect(displacedPoint.x, displacedPoint.y, m_selectionStartPoint.x, m_selectionStartPoint.y);
	selectionRect.NormalizeRect();

	CRect	docRect(CPoint(0, 0), pDoc->GetDocSize());
	CRect	testRect;

	POSITION	pos;
	POSITION	pos2;

	switch (m_dragState) {
	case DS_MOVE_SELECTION:

		testRect = GetDragListTestRect(displacedPoint);

		if (testRect.left < docRect.left) displacedPoint.Offset(CPoint(docRect.left-testRect.left, 0));
		if (testRect.top < docRect.top) displacedPoint.Offset(CPoint(0, docRect.top-testRect.top));
		if (testRect.right > docRect.right) displacedPoint.Offset(CPoint(docRect.right-testRect.right, 0));
		if (testRect.bottom > docRect.bottom) displacedPoint.Offset(CPoint(0, docRect.bottom-testRect.bottom));


		CDragRect*	tmp;

		pos = m_dragRectList.GetHeadPosition();
		while (pos != NULL) {
			tmp = m_dragRectList.GetNext(pos);
			tmp->m_object->MoveObject(displacedPoint + tmp->m_rect.TopLeft(), NULL);

			tmp->m_object->SetInvisability(FALSE);

			pos2 = tmp->m_object->m_fromHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_fromHereList.GetNext(pos2)->SetInvisability(FALSE);
			}

			pos2 = tmp->m_object->m_toHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_toHereList.GetNext(pos2)->SetInvisability(FALSE);
			}
		}

		HideDragList();

		//UpdateAllViews nedan ritar ut alla objekt

		pDoc->SetModifiedFlag(TRUE);

		break;
		
	case DS_SELECTION:
		if (!m_oldSelectionRect.IsRectNull()) {
			dc.DrawFocusRect(m_oldSelectionRect);
		}

		if (nFlags & MK_SHIFT) {
			pDoc->AddToSelection(selectionRect, &dc);
		} else {
			pDoc->SetSelection(TRUE, selectionRect, &dc);
		}
		break;
	case DS_PRE_MOVE_SELECTION:
		pDoc->ClearSelection(&dc);
		pDoc->AddToSelection(m_dragObject, &dc);
		m_dragObject = NULL;
		break;
	case DS_PLACE_TRANSITION:

		HideDragList();

		CScriptObject*	dstObj = pDoc->IsObjectHit(displacedPoint);

		if (dstObj != m_dragObject) {

			if (dstObj) {
				ReleaseCapture();
				int transType = pDoc->AskForTransitionType(m_dragObject);
				if (transType) {
					CTransition*	tmp;
					tmp = pDoc->NewTransition(m_dragObject, dstObj, transType);
					pDoc->ClearSelection(&dc);
					tmp->SetSelected(TRUE, &dc);
					pDoc->m_selectedTransition = tmp;
					pDoc->SetModifiedFlag(TRUE);
				}
			}
		}

		ReleaseCapture();
		m_dragState = DS_NONE;
	}

	m_dragState = DS_NONE;
	pDoc->UpdateAllViews(NULL);
	ReleaseCapture();

	return;
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CMDIBitmapView::OnMouseMove(UINT nFlags, CPoint point) 
{

	if( GetCapture( ) != this )	return;

	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);

	CPoint	displacedPoint;
	displacedPoint = point + GetScrollPosition();

	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect	drawRect(displacedPoint.x, displacedPoint.y, m_selectionStartPoint.x, m_selectionStartPoint.y);
	drawRect.NormalizeRect();

	CRect	docRect(CPoint(0, 0), pDoc->GetDocSize());
	CRect	testRect;

	switch (m_dragState) {
	case DS_MOVE_SELECTION:
	case DS_PLACE_SELECTION:
		
		testRect = GetDragListTestRect(displacedPoint);

		if (testRect.left < docRect.left) displacedPoint.Offset(CPoint(docRect.left-testRect.left, 0));
		if (testRect.top < docRect.top) displacedPoint.Offset(CPoint(0, docRect.top-testRect.top));
		if (testRect.right > docRect.right) displacedPoint.Offset(CPoint(docRect.right-testRect.right, 0));
		if (testRect.bottom > docRect.bottom) displacedPoint.Offset(CPoint(0, docRect.bottom-testRect.bottom));

		ShowDragList(displacedPoint);

		break;

	case DS_SELECTION:

		if (!m_oldSelectionRect.IsRectNull()) {
			dc.DrawFocusRect(m_oldSelectionRect);
		}

		m_oldSelectionRect = drawRect;

		dc.DrawFocusRect(drawRect);
		break;

	case DS_PRE_MOVE_SELECTION:
		if (PointToPointDistance(displacedPoint, m_preDragPoint) > 10) {

			pDoc->HideSelection(&dc);

			SelectionToDragList(m_preDragPoint);

			ShowDragList(displacedPoint);

			m_dragState = DS_MOVE_SELECTION;
		}
		break;
	case DS_PLACE_TRANSITION:

		ShowDragList(displacedPoint);

		break;
	}

	return;

	CScrollView::OnMouseMove(nFlags, point);
}



void CMDIBitmapView::OnObjectAddTransition() 
{
	if (m_transitionActive) {
		m_transitionActive = FALSE;
	} else {
		m_transitionActive = TRUE;
	}
}

void CMDIBitmapView::OnUpdateObjectAddTransition(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_transitionActive);
}

void CMDIBitmapView::OnEditPaste( void ) 
{
	m_transitionActive = FALSE;

	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);
	
	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ClearSelection(&dc);

	CScriptObject*	tmpObj;
	CScriptObject*	tmpNew;
	CTransition*	tmpTrans;

	CRect	unionRect;

	POSITION	pos;
	
	pos = clipboardObjectList.GetHeadPosition();
	while (pos != NULL) {
		tmpObj = clipboardObjectList.GetNext(pos);
		tmpNew = pDoc->NewObject(tmpObj->GetRect().TopLeft(), tmpObj->GetType());
		tmpNew->DuplicateObject(tmpObj);
		tmpNew->m_clipboardBuddy = tmpObj;
		tmpObj->m_clipboardBuddy = tmpNew;
	}

	pos = clipboardTransitionList.GetHeadPosition();
	while (pos != NULL) {
		tmpTrans = clipboardTransitionList.GetNext(pos);
		pDoc->NewTransition(tmpTrans->m_srcObject->m_clipboardBuddy, tmpTrans->m_dstObject->m_clipboardBuddy, tmpTrans->m_transitionType);
	}

	unionRect.SetRectEmpty();

	pos = clipboardObjectList.GetHeadPosition();
	while (pos != NULL) {
		tmpObj = clipboardObjectList.GetNext(pos);
		pDoc->AddToSelection(tmpObj->m_clipboardBuddy);
		unionRect.UnionRect(&unionRect, tmpObj->GetRect());
	}

//	pDoc->HideSelection(&dc);
	SelectionToDragList(unionRect.TopLeft());
	
	m_dragState = DS_PLACE_SELECTION;
	SetCapture();
}


void CMDIBitmapView::ClearDragList( void )
{
	while( !m_dragRectList.IsEmpty( ) ) {
		delete m_dragRectList.RemoveHead();
	}	

	while( !m_dragLineList.IsEmpty( ) ) {
		delete m_dragLineList.RemoveHead();
	}	
}

void CMDIBitmapView::AddToDragList(CRect rect, CScriptObject* object)
{
	m_dragRectList.AddTail(new CDragRect(rect, object));
}

void CMDIBitmapView::AddToDragList( CPoint startP, BOOL startLock, CPoint endP, BOOL endLock)
{
	m_dragLineList.AddTail(new CDragLine(startP, startLock, endP, endLock));
}

void  CMDIBitmapView::ShowDragList( CPoint point )
{
	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);

	
	if (m_dragListShown) {
		HideDragList();
	}

	m_dragListShown = TRUE;
	m_oldDragPoint = point;

	POSITION	pos;
	CDragRect*	tmpRect;

	pos = m_dragRectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpRect = m_dragRectList.GetNext(pos);
		dc.DrawFocusRect(tmpRect->m_rect + point);
	}

	CDragLine*	tmpLine;

	dc.SetROP2(R2_NOTXORPEN);

	CPen	drawPen(PS_SOLID, 1, (COLORREF) 0x00000000);
	CPen*	oldPen = (CPen*) dc.SelectObject(&drawPen);

	pos = m_dragLineList.GetHeadPosition( );
	while (pos != NULL) {
		tmpLine = m_dragLineList.GetNext(pos);


		if (tmpLine->m_startLock) {
			dc.MoveTo(tmpLine->m_startP);
		} else {
			dc.MoveTo(tmpLine->m_startP + point);
		}
		if (tmpLine->m_endLock) {
			dc.LineTo(tmpLine->m_endP);
		} else {
			dc.LineTo(tmpLine->m_endP + point);
		}
	}

	dc.SelectObject(oldPen);
}


void CMDIBitmapView::HideDragList( void )
{
	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);

	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	m_dragListShown = FALSE;

	POSITION	pos;
	CDragRect*	tmpRect;

	pos = m_dragRectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpRect = m_dragRectList.GetNext(pos);
		dc.DrawFocusRect(tmpRect->m_rect + m_oldDragPoint);
	}

	CDragLine*	tmpLine;

	dc.SetROP2(R2_NOTXORPEN);

	CPen	drawPen(PS_SOLID, 1, (COLORREF) 0x00000000);
	CPen*	oldPen = (CPen*) dc.SelectObject(&drawPen);

	pos = m_dragLineList.GetHeadPosition( );
	while (pos != NULL) {
		tmpLine = m_dragLineList.GetNext(pos);


		if (tmpLine->m_startLock) {
			dc.MoveTo(tmpLine->m_startP);
		} else {
			dc.MoveTo(tmpLine->m_startP + m_oldDragPoint);
		}
		if (tmpLine->m_endLock) {
			dc.LineTo(tmpLine->m_endP);
		} else {
			dc.LineTo(tmpLine->m_endP + m_oldDragPoint);
		}
	}

	dc.SelectObject(oldPen);
}

CRect CMDIBitmapView::GetDragListTestRect( CPoint point)
{
	POSITION	pos;
	CRect		retRect;

	retRect.SetRectEmpty();

	pos = m_dragRectList.GetHeadPosition( );
	while (pos != NULL) {
		retRect.UnionRect(&retRect, m_dragRectList.GetNext(pos)->m_rect + point);
	}

	return retRect;
}


void CMDIBitmapView::SelectionToDragList(CPoint point)
{
	ClearDragList();
	
	CScriptObject*	tmpObj;
	CRect			tmpRect;
	CPoint			rectTopLeft;
	CSize			rectSize;

	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos = pDoc->m_objectList.GetHeadPosition();
	while (pos != NULL) {
		tmpObj = pDoc->m_objectList.GetNext(pos);
		if (tmpObj->IsSelected()) {
			rectSize = tmpObj->GetRect().Size();
			rectTopLeft = tmpObj->GetRect().TopLeft() - point;
			AddToDragList(CRect(rectTopLeft, rectSize), tmpObj);
			tmpObj->SetInvisability(TRUE);
		}
	}

	CTransition*	tmpTrans;
	CPoint			startP;
	CPoint			endP;
	BOOL			lockStart;
	BOOL			lockEnd;

	pos = pDoc->m_transitionList.GetHeadPosition();
	while (pos != NULL) {
		tmpTrans = pDoc->m_transitionList.GetNext(pos);
		if (tmpTrans->IsSelected()) {
			if (tmpTrans->m_srcObject->IsSelected()) {
				lockStart = FALSE;
				startP = tmpTrans->m_srcObject->GetFromHerePoint() - point;
			} else {
				lockStart = TRUE;
				startP = tmpTrans->m_srcObject->GetFromHerePoint();
			}
			if (tmpTrans->m_dstObject->IsSelected()) {
				lockEnd = FALSE;
				endP = tmpTrans->m_dstObject->GetToHerePoint() - point;
			} else {
				lockEnd = TRUE;
				endP = tmpTrans->m_dstObject->GetToHerePoint();
			}

			AddToDragList(startP, lockStart, endP, lockEnd);
			tmpTrans->SetInvisability(TRUE);
		}
	}

}


int CMDIBitmapView::PointToPointDistance(CPoint p1, CPoint p2)
{
	return (int) sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void CMDIBitmapView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (bActivate) {

		pDoc->m_currentView = this;
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMDIBitmapView::OnObjectNew(int objectType)
{
	m_transitionActive = FALSE;

	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);
	
	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CScriptObject* newObject = pDoc->NewObject(CPoint(0, 0), objectType);

	pDoc->ClearSelection(&dc);
	pDoc->AddToSelection(newObject, NULL);

	ClearDragList();
	AddToDragList(newObject->GetRect(), newObject);

	m_dragState = DS_PLACE_SELECTION;
	SetCapture();
}

void CMDIBitmapView::OnScriptDropPhone() 
{
	OnObjectNew(OT_DROP_PHONE);
}

void CMDIBitmapView::OnScriptLiftPhone() 
{
	OnObjectNew(OT_LIFT_PHONE);
}

void CMDIBitmapView::OnScriptWaitForCall() 
{
	OnObjectNew(OT_WAIT_CALL);
}

void CMDIBitmapView::OnScriptPlay() 
{
	OnObjectNew(OT_PLAY_FILE);
}

void CMDIBitmapView::OnScriptAskQuestion() 
{
	OnObjectNew(OT_ASK_QUESTION);
}

void CMDIBitmapView::OnScriptPromptCode() 
{
	OnObjectNew(OT_PROMPT_CODE);
}

void CMDIBitmapView::OnScriptRecMsg() 
{
	OnObjectNew(OT_REC_MSG);
}

void CMDIBitmapView::OnScriptEraseMsg() 
{
	OnObjectNew(OT_ERASE_ALL);
}

void CMDIBitmapView::OnScriptAnswerOn() 
{
	OnObjectNew(OT_ANSWER_ON);
}

void CMDIBitmapView::OnScriptAnswerOff() 
{
	OnObjectNew(OT_ANSWER_OFF);
}

void CMDIBitmapView::OnScriptPlayAll() 
{
	OnObjectNew(OT_PLAY_ALL);
}

void CMDIBitmapView::OnScriptInsertLabel() 
{
	OnObjectNew(OT_LABEL);
}

void CMDIBitmapView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_dragState != DS_NONE) m_disablePopup = TRUE;
	OnCustomCancel();
	//	CScrollView::OnRButtonDown(nFlags, point);
}

void CMDIBitmapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_disablePopup) {
		CMDIBitmapDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		CClientDC dc( this );
		OnPrepareDC(&dc, NULL);

		CPoint	displacedPoint;
		displacedPoint = point + GetScrollPosition();

		if (!pDoc->IsSelectionHit(displacedPoint)) {
			m_dragObject = pDoc->IsObjectHit(displacedPoint);
			if (m_dragObject) {
				pDoc->ClearSelection(&dc);
				pDoc->AddToSelection(m_dragObject, &dc);
			}
		}

		CMenu menu;    
		CMenu *submenu;    
		// Load the menu.
		menu.LoadMenu(IDR_RIGHT_MOUSE);    
		// Get the pop-up menu.
		submenu = menu.GetSubMenu(0);    
		// Convert to screen coordinates.
		ClientToScreen(&point);    
		// Post the menu.
		submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);
	}
	m_disablePopup = FALSE;
}

void CMDIBitmapView::OnCustomCancel() 
{
	CClientDC dc( this );
	OnPrepareDC(&dc, NULL);

	CMDIBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos;
	POSITION pos2;

	CDragRect*	tmp;

	switch (m_dragState) {
	case DS_MOVE_SELECTION:

		pos = m_dragRectList.GetHeadPosition();
		while (pos != NULL) {
			tmp = m_dragRectList.GetNext(pos);

			tmp->m_object->SetInvisability(FALSE);

			pos2 = tmp->m_object->m_fromHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_fromHereList.GetNext(pos2)->SetInvisability(FALSE);
			}

			pos2 = tmp->m_object->m_toHereList.GetHeadPosition();
			while (pos2 != NULL) {
				tmp->m_object->m_toHereList.GetNext(pos2)->SetInvisability(FALSE);
			}
		}

		break;
	
	case DS_PLACE_SELECTION:
		pDoc->ClearSelection();

		pos = m_dragRectList.GetHeadPosition();
		while (pos != NULL) {
			tmp = m_dragRectList.GetNext(pos);
			pDoc->DeleteObject(tmp->m_object);

		}

		break;

	case DS_PLACE_TRANSITION:

		break;
	case DS_SELECTION:

		if (!m_oldSelectionRect.IsRectNull()) {
			dc.DrawFocusRect(m_oldSelectionRect);
		}
		break;
	}
	HideDragList();

	m_dragState = DS_NONE;

	ReleaseCapture();

	pDoc->UpdateAllViews(NULL);
}

void CMDIBitmapView::OnUpdateCustomCancel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_dragState == DS_PLACE_SELECTION);	
}

