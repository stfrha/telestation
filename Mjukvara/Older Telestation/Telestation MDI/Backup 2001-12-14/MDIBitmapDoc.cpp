// MDIBitmapDoc.cpp : implementation of the CMDIBitmapDoc class
//

#include "stdafx.h"
#include "MDIBitmap.h"

#include "MDIBitmapDoc.h"
#include "LabelPropDlg.h"
#include "PlayFilePropDlg.h"
#include "EnterCodeDlg.h"
#include "NoPropDlg.h"
#include "SelectTransDlg.h"
#include "ProjectDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MDIBitmapView.h"


/////////////////////////////////////
// Globala object

CTypedPtrList<CObList, CScriptObject*> clipboardObjectList;
CTypedPtrList<CObList, CTransition*> clipboardTransitionList;

extern CBitmap	happySun;
extern CBitmap	waitCall;
extern CBitmap	dropPhone;
extern CBitmap	liftPhone;
extern CBitmap	playFile;
extern CBitmap	askQuestion;
extern CBitmap	promptCode;
extern CBitmap	recMsg;
extern CBitmap	playAll;
extern CBitmap	eraseAll;
extern CBitmap	answerOn;
extern CBitmap	answerOff;
extern CBitmap	label;

extern CStringArray	TransTypeStrings;

//Object Transition List

/*
int GenTransList[] = {	TTI_ENABLED, 
						TTI_ENABLED, 
						TTI_ENABLED, 
						TTI_ENABLED, 
						TTI_ENABLED, 
						TTI_ENABLED, 
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED,
						TTI_ENABLED};

*/

int LiftPhoneTransList[] = {TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int DropPhoneTransList[] = {TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int WaitCallTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_1, TTI_0, TTI_0, TTI_1};

int PlayFileTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AskQuestTransList[] = {	TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, 
							TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, 
							TTI_1, TTI_1, TTI_0, TTI_0, TTI_0};

int PromptCodeTransList[]= {TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_1, TTI_1, TTI_1, TTI_1, TTI_0};

int RecMsgTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_1, 
							TTI_1, TTI_0, TTI_0, TTI_0, TTI_0};

int EraseAllTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AnsOnTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AnsOffTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int PlayAllTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int LabelTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};


/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapDoc

IMPLEMENT_DYNCREATE(CMDIBitmapDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIBitmapDoc, CDocument)
	//{{AFX_MSG_MAP(CMDIBitmapDoc)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_OBJECT_PROP, OnObjectProp)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PROP, OnUpdateObjectProp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapDoc construction/destruction

CMDIBitmapDoc::CMDIBitmapDoc()
{
	m_projectDoc = NULL;
}

CMDIBitmapDoc::~CMDIBitmapDoc()
{
}

BOOL CMDIBitmapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	InitDocument();
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapDoc serialization

void CMDIBitmapDoc::Serialize(CArchive& ar)
{
	m_objectList.Serialize(ar);
	m_transitionList.Serialize(ar);

	if (ar.IsStoring())	{
	} else {
		PostSerializeProcessData();
	}

}

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapDoc diagnostics

#ifdef _DEBUG
void CMDIBitmapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIBitmapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIBitmapDoc commands

void CMDIBitmapDoc::InitDocument()
{
	m_sizeDoc = CSize(800,900);
/*
	CScriptObject*	o1;
	CScriptObject*	o2;

	o1 = NewObject(CPoint(20, 20), OT_HAPPY_SUN);
	o2 = NewObject(CPoint(60, 200), OT_HAPPY_SUN);

	NewTransition(o1, o2, 1);
*/
	m_selectedTransition = NULL;
	m_skipNoProperty = FALSE;
}



BOOL CMDIBitmapDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	InitDocument();
	
	return TRUE;
}

void CMDIBitmapDoc::DeleteContents() 
{
	CScriptObject* tmpObj;

	while( !m_objectList.IsEmpty( ) ) {
		tmpObj = m_objectList.RemoveHead( );
		tmpObj->m_fromHereList.RemoveAll();
		tmpObj->m_toHereList.RemoveAll();
		delete tmpObj;
	}	

	while ( !m_transitionList.IsEmpty( ) ) {
		delete m_transitionList.RemoveHead();
	}

	
	ClearClipboard();  //Skall flyttas till App. avslut, inte dokumentets

	m_selectionList.RemoveAll();

	CDocument::DeleteContents();
}





CScriptObject* CMDIBitmapDoc::NewObject(CPoint startPoint, int objectType)
{
	CScriptObject* newObject = new CScriptObject(startPoint, objectType, GetUniqueIdentity());

	m_objectList.AddTail(newObject);

	return newObject;
}

void CMDIBitmapDoc::DeleteObject(CScriptObject *deleteMe)
{
	POSITION pos = m_objectList.Find(deleteMe);

	m_objectList.RemoveAt(pos);

	while (!deleteMe->m_fromHereList.IsEmpty()) {
		DeleteTransition(deleteMe->m_fromHereList.RemoveHead());
	}

	while (!deleteMe->m_toHereList.IsEmpty()) {
		DeleteTransition(deleteMe->m_toHereList.RemoveHead());
	}

	delete deleteMe;
}

CScriptObject*	CMDIBitmapDoc::IsObjectHit(CPoint point)
{
	CScriptObject*	tmpObj;

	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		if (tmpObj->IsHit(point)) return tmpObj;
	}
	return NULL;
}





CTransition* CMDIBitmapDoc::NewTransition(CScriptObject* srcObj, CScriptObject* dstObj, int transitionType)
{
	CTransition* newObject = new CTransition(srcObj, dstObj, transitionType );

	m_transitionList.AddTail(newObject);

	srcObj->m_fromHereList.AddTail(newObject);
	dstObj->m_toHereList.AddTail(newObject);
	newObject->m_srcObject = srcObj;
	newObject->m_dstObject = dstObj;

	UpdateAllViews(NULL);

	return newObject;
}

void CMDIBitmapDoc::DeleteTransition(CTransition* deleteMe)
{
	if (m_selectedTransition == deleteMe) m_selectedTransition = NULL;

	POSITION pos = deleteMe->m_srcObject->m_fromHereList.Find(deleteMe);
	if (pos) deleteMe->m_srcObject->m_fromHereList.RemoveAt(pos);

	pos = deleteMe->m_dstObject->m_toHereList.Find(deleteMe);
	if (pos) deleteMe->m_dstObject->m_toHereList.RemoveAt(pos);

	pos = m_transitionList.Find(deleteMe);
	if (pos) m_transitionList.RemoveAt(pos);

	delete deleteMe;
}

CTransition*	CMDIBitmapDoc::IsTransitionHit(CPoint point)
{
	CTransition*	tmpTrans;

	POSITION pos = m_transitionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpTrans = m_transitionList.GetNext(pos);
		if (tmpTrans->IsHit(point)) return tmpTrans;
	}
	return NULL;
}



void	CMDIBitmapDoc::ClearSelection( CDC* dc)
{
	while( !m_selectionList.IsEmpty( ) ) {
		m_selectionList.RemoveHead()->SetSelected(FALSE, dc);
	}
	
	if (m_selectedTransition) {
		m_selectedTransition->SetSelected(FALSE, dc);
		m_selectedTransition = NULL;
	}
}

void	CMDIBitmapDoc::RemoveFromSelection(CScriptObject* removeMe, CDC* dc)
{
	CTypedPtrList<CObList, CScriptObject*> tmpList;
	CScriptObject*	tmpObject;
	BOOL	done = FALSE;

	while( !done) {
		if (m_selectionList.IsEmpty( )) done = TRUE;
		tmpObject = m_selectionList.RemoveHead();
		if (tmpObject == removeMe) {
			tmpObject->SetSelected(FALSE, dc);
			done = TRUE;
		} else {
			tmpList.AddTail(tmpObject);
		}
	}	

	while( !tmpList.IsEmpty( ) ) {
		m_selectionList.AddTail(tmpList.RemoveHead());
	}	
}


void	CMDIBitmapDoc::SetSelection(BOOL select, CRect selectionRect, CDC* dc)
{
	ClearSelection(dc);

	if (select) {
		AddToSelection(selectionRect, dc);
	}
}

void	CMDIBitmapDoc::AddToSelection(CScriptObject* newSelected, CDC* dc)
{
	if (!newSelected->IsSelected()) {
		m_selectionList.AddHead(newSelected);
	}
	newSelected->SetSelected(TRUE, dc);
}

void	CMDIBitmapDoc::AddToSelection(CRect selectionRect, CDC* dc)
{
	CScriptObject*	tmpObj;
	
	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		if (tmpObj->IsPartOf(selectionRect)) {
			if (!tmpObj->IsSelected()) {
				m_selectionList.AddHead(tmpObj);
			}
			tmpObj->SetSelected(TRUE, dc);
		}
	}		
}


void	CMDIBitmapDoc::OnDrawDoc(CDC* dc)
{
	CScriptObject*	tmpObj;

		
	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		m_objectList.GetNext(pos)->DrawObject(dc);
	}


	POSITION pos2;

	pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		pos2 = tmpObj->m_fromHereList.GetHeadPosition( );
		while (pos2 != NULL) {
			tmpObj->m_fromHereList.GetNext(pos2)->DrawTransition(dc);
		}
	}

	if (m_projectDoc) {
		if (m_projectDoc->IsSimulatorRunning()) {
			CScriptObject* tmp = m_projectDoc->GetCurrnetSimulatorObject();

			CBitmap*	pbmOld = NULL;
			CBitmap*	redArrow = (CBitmap*) new CBitmap;

			redArrow->LoadBitmap(IDB_SIMULATION_ARROW);

			BITMAP	bms;

			redArrow->GetBitmap(&bms);
		
			CDC dcMem;
			dcMem.CreateCompatibleDC(dc);

			pbmOld = dcMem.SelectObject(redArrow);

			dc->BitBlt(tmp->GetRect().right, tmp->GetRect().top - bms.bmHeight, bms.bmWidth, bms.bmHeight, &dcMem, 0, 0, /*0x00990066*/SRCCOPY);

			dcMem.SelectObject(pbmOld);
			dcMem.DeleteDC();

			delete redArrow;
		}
	}
}

BOOL	CMDIBitmapDoc::IsSelectionHit(CPoint point)
{
	POSITION pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		if (m_selectionList.GetNext(pos)->IsHit(point)) return TRUE;
	}
	return FALSE;
}

void	CMDIBitmapDoc::HideSelection(CDC* dc)
{
	CScriptObject*	tmpObj;
	CTransition*	tmpTrans;
	POSITION		pos;
	POSITION		pos2;


	pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_selectionList.GetNext(pos);
		tmpObj->HideObject(dc);

		pos2 = tmpObj->m_fromHereList.GetHeadPosition( );
		while (pos2 != NULL) {
			tmpTrans = tmpObj->m_fromHereList.GetNext(pos2);
			tmpTrans->HideTransition(dc);
		}

		pos2 = tmpObj->m_toHereList.GetHeadPosition( );
		while (pos2 != NULL) {
			tmpTrans = tmpObj->m_toHereList.GetNext(pos2);
			tmpTrans->HideTransition(dc);
		}
	}
}


void	CMDIBitmapDoc::MoveSelection(CDC* dc, CPoint dstPoint)
{
	CScriptObject*	tmpObj;
	CTransition*	tmpTrans;
	CTypedPtrList<CObList, CTransition*>	transList;

	//Gör en lista med alla CTransitions
	POSITION transPos;

	POSITION objPos = m_selectionList.GetHeadPosition( );

	while (objPos != NULL) {
		tmpObj = m_selectionList.GetNext(objPos);
		transPos = tmpObj->m_fromHereList.GetHeadPosition();
		while (transPos != NULL) {
			tmpTrans = tmpObj->m_fromHereList.GetNext(transPos);
			if (!transList.Find(tmpTrans)) {
				transList.AddTail(tmpTrans);
			}
		}
		transPos = tmpObj->m_toHereList.GetHeadPosition();
		while (transPos != NULL) {
			tmpTrans = tmpObj->m_toHereList.GetNext(transPos);
			if (!transList.Find(tmpTrans)) {
				transList.AddTail(tmpTrans);
			}
		}
	}

	//Göm alla CTransitions i listan
	transPos = transList.GetHeadPosition();
	while (transPos != NULL) {
		transList.GetNext(transPos)->HideTransition(dc);
	}

	//Flytta alla CScriptObject
	objPos = m_selectionList.GetHeadPosition( );
	while (objPos != NULL) {
		m_selectionList.GetNext(objPos)->MoveObject(dstPoint, dc);
	}

	//Rita ut alla CTransitions i listan
	transPos = transList.GetHeadPosition();
	while (transPos != NULL) {
		transList.GetNext(transPos)->DrawTransition(dc);
	}

	transList.RemoveAll();
/*	
	POSITION pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		m_selectionList.GetNext(pos)->MoveObject(dc, dstPoint);
	}
*/
}

void CMDIBitmapDoc::ClearClipboard()
{
	while( !clipboardObjectList.IsEmpty( ) ) {
		delete clipboardObjectList.RemoveHead( );
	}	

	while( !clipboardTransitionList.IsEmpty( ) ) {
		delete clipboardTransitionList.RemoveHead( );
	}	
}

void CMDIBitmapDoc::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_selectionList.IsEmpty());	
}

void CMDIBitmapDoc::OnEditCopy() 
{
	ClearClipboard();

	CScriptObject*	tmpObj;
	CScriptObject*	tmpNew;
	CTransition*	tmpTrans;
	CTransition*	newTrans;
	POSITION		pos;
	POSITION		pos2;

	
	pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_selectionList.GetNext(pos);
		tmpNew = new CScriptObject(CPoint(0, 0), tmpObj->GetType(), GetUniqueIdentity());
		tmpNew->DuplicateObject(tmpObj);
		tmpNew->m_clipboardBuddy = tmpObj;
		tmpObj->m_clipboardBuddy = tmpNew;
		clipboardObjectList.AddTail(tmpNew);
	}

	pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_selectionList.GetNext(pos);
		pos2 = tmpObj->m_fromHereList.GetHeadPosition();
		while (pos2 != NULL) {
			tmpTrans = tmpObj->m_fromHereList.GetNext(pos2);
			if (tmpTrans->m_dstObject->IsSelected() ) {
				newTrans = new CTransition(tmpObj->m_clipboardBuddy, tmpTrans->m_dstObject->m_clipboardBuddy, tmpTrans->m_transitionType);
				clipboardTransitionList.AddTail(newTrans);
			}
		}
	}
}

void CMDIBitmapDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_selectionList.IsEmpty());	
}

void CMDIBitmapDoc::OnEditCut() 
{
	OnEditCopy();
	OnEditDelete();
}


void CMDIBitmapDoc::OnEditDelete() 
{
	CScriptObject*	tmpObj;
	
	while(!m_selectionList.IsEmpty()) {
		tmpObj = m_selectionList.RemoveHead();
		DeleteObject(tmpObj);
		SetModifiedFlag(TRUE);
	}

	if (m_selectedTransition) {
		DeleteTransition(m_selectedTransition);
		m_selectedTransition = NULL;
		SetModifiedFlag(TRUE);
	}

//	if (m_objectList.IsEmpty()) MessageBox(NULL, "It it empty!!!!", "Hello", MB_OK);

	
	UpdateAllViews(NULL);
}

void CMDIBitmapDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((!m_selectionList.IsEmpty()) || (m_selectedTransition != NULL));	
}

void CMDIBitmapDoc::OnEditPaste() 
{
	m_currentView->OnEditPaste();
}

void CMDIBitmapDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!clipboardObjectList.IsEmpty());	
}

void CMDIBitmapDoc::OnEditSelectAll() 
{
	ClearSelection();

	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		AddToSelection(m_objectList.GetNext(pos));
	}
	UpdateAllViews(NULL);
}

int CMDIBitmapDoc::GetUniqueIdentity( void )
{
	int testIdentity = 0;

	while ((OccupiedIdentity(testIdentity)) && (testIdentity < 32768)) {
		testIdentity++;
	}

	return testIdentity;
}

BOOL CMDIBitmapDoc::OccupiedIdentity(int i)
{
	POSITION pos = m_objectList.GetHeadPosition();
	while (pos != NULL) {
		if (m_objectList.GetNext(pos)->GetIdentity() == i) return TRUE;
	}
	return FALSE;
}

void CMDIBitmapDoc::PostSerializeProcessData( void )
{
	CTransition*	tmpTrans;

	POSITION pos = m_transitionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpTrans = m_transitionList.GetNext(pos);
		tmpTrans->m_srcObject = FindObject(tmpTrans->m_srcIdentity);
		if (!tmpTrans->m_srcObject) MessageBox(NULL, "Could not find corresponding object", "Error", MB_OK);
		else {
			tmpTrans->m_srcObject->m_fromHereList.AddTail(tmpTrans);
		}

		tmpTrans->m_dstObject = FindObject(tmpTrans->m_dstIdentity);
		if (!tmpTrans->m_dstObject) MessageBox(NULL, "Could not find corresponding object", "Error", MB_OK);
		else {
			tmpTrans->m_dstObject->m_toHereList.AddTail(tmpTrans);
		}
	}
}

CScriptObject*	CMDIBitmapDoc::FindObject(int identity)
{
	CScriptObject*	tmpObj;
	
	POSITION pos = m_objectList.GetHeadPosition();

	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		if (tmpObj->GetIdentity() == identity) return tmpObj;
	}
	return NULL;
}

void CMDIBitmapDoc::OnObjectProp() 
{
	if (m_selectionList.GetCount() == 1) {

		CLabelPropDlg		labelDlg;
		CPlayFilePropDlg	playDlg;
		CEnterCodeDlg		codeDlg;
		CNoPropDlg			noDlg;
		CString				oldStr1;
		CString				oldStr2;

		playDlg.SetProject(m_projectDoc);
		codeDlg.SetProject(m_projectDoc);

		CScriptObject*		tmpObj = m_selectionList.GetHead();

		
		switch(tmpObj->GetType()) {
		case OT_LABEL:

			labelDlg.m_label = tmpObj->GetLabel();

			oldStr1 = labelDlg.m_label;

			if (labelDlg.DoModal() == IDOK) {
				if (labelDlg.m_label != oldStr1) {
					tmpObj->SetLabel(labelDlg.m_label);
					SetModifiedFlag(TRUE);
					UpdateAllViews(NULL);
				}
			}
			break;
		case OT_PLAY_FILE:
		case OT_ASK_QUESTION:

			playDlg.SetSelection(tmpObj->GetAnnouncement());
			oldStr1 = tmpObj->GetAnnouncement();

			if (playDlg.DoModal() == IDOK) {

				if (playDlg.GetSelection() != oldStr1) {
					tmpObj->SetAnnouncement(playDlg.GetSelection());
					SetModifiedFlag(TRUE);
				}
			}
			break;
		case OT_PROMPT_CODE:
			codeDlg.SetSelection(tmpObj->GetAnnouncement());
			oldStr1 = tmpObj->GetAnnouncement();

			codeDlg.m_code = tmpObj->GetPINCode();
			oldStr2 = codeDlg.m_code;
			
			if (codeDlg.DoModal() == IDOK) {
				if (codeDlg.GetSelection() != oldStr1) {
					tmpObj->SetAnnouncement(codeDlg.GetSelection());
					SetModifiedFlag(TRUE);
				}

				if (codeDlg.m_code != oldStr2) {
					tmpObj->SetPINCode(codeDlg.m_code);
					SetModifiedFlag(TRUE);
				}
			}
			break;
		default:
			if (!m_skipNoProperty) {
				noDlg.DoModal();
			}
			break;
		}
	}
	m_skipNoProperty = FALSE;
}

void CMDIBitmapDoc::OnUpdateObjectProp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selectionList.GetCount() == 1);	
}

void CMDIBitmapDoc::AskForProperties( void )
{
	m_skipNoProperty = TRUE;
	OnObjectProp();
}

int	CMDIBitmapDoc::AskForTransitionType(CScriptObject* srcObj)
{
	CSelectTransDlg		dlg;
	int*				defaultFlag;

	switch(srcObj->GetType()) {
	case OT_WAIT_CALL:
		defaultFlag = WaitCallTransList;
		break;
	case OT_LIFT_PHONE:
		defaultFlag = LiftPhoneTransList;
		break;
	case OT_DROP_PHONE:
		defaultFlag = DropPhoneTransList;
		break;
	case OT_PLAY_FILE:
		defaultFlag = PlayFileTransList;
		break;
	case OT_ASK_QUESTION:
		defaultFlag = AskQuestTransList;
		break;
	case OT_PROMPT_CODE:
		defaultFlag = PromptCodeTransList;
		break;
	case OT_REC_MSG:
		defaultFlag = RecMsgTransList;
		break;
	case OT_PLAY_ALL:
		defaultFlag = PlayAllTransList;
		break;
	case OT_ERASE_ALL:
		defaultFlag = EraseAllTransList;
		break;
	case OT_ANSWER_ON:
		defaultFlag = AnsOnTransList;
		break;
	case OT_ANSWER_OFF:
		defaultFlag = AnsOffTransList;
		break;
	case OT_LABEL:
		defaultFlag = LabelTransList;
		break;
	}

	int		insertedTypes[NUM_TRANS_TYPE];
	int		index = 0;
	int		i;

	CString	tmpStr;

	for (i = 0 ; i<NUM_TRANS_TYPE ; i++)  {
		if (defaultFlag[i] == TTI_ENABLED) {
			if (!srcObj->IsTransitionOccupied(i+1)) {
				tmpStr = TransTypeStrings[i];
				dlg.m_stringList.AddTail(tmpStr);
				insertedTypes[index++] = i+1;
			}
		}
	}
	if (index) {
		if (index == 1) {
			return insertedTypes[0];
		}
		if (dlg.DoModal() == IDOK) {
			int sel = dlg.m_selection;
			if (sel == LB_ERR) return 0;
			return insertedTypes[sel];
		} else {
			return 0;
		}
	} else {
		return 0;
	}

}

void CMDIBitmapDoc::OnCloseDocument() 
{
	if (m_projectDoc) m_projectDoc->InvalidateScriptDoc();
	
	CDocument::OnCloseDocument();
}

/*
BOOL 	CMDIBitmapDoc::IsDocumentValid( void )
{
	//Finns en label med namnet "Start"
	//Finns en label med namnet "End"
	//Hamnar man alltid i "Start" efter "End"
}


*/
void CMDIBitmapDoc::InvalidateProjectDoc( void )
{
	m_projectDoc = NULL;
}
























IMPLEMENT_SERIAL( CScriptObject, CObject, 3 )

CScriptObject::CScriptObject()
{
	m_selected = FALSE;
	m_clipboardBuddy = NULL;
	m_invisable = FALSE;
	m_labelName = "Label Text String";
	m_announcement = "";
	m_PINcode = "";
}

CScriptObject::CScriptObject(CPoint startPoint, int objectType, int identity)
{
//	m_objectType = OT_HAPPY_SUN;

	m_labelName = "Label Text String";
	
	m_objectType = objectType;


	BITMAP	tmpBM;

	
	switch (m_objectType) {
	case OT_HAPPY_SUN:
		happySun.GetBitmap(&tmpBM);
		break;
	case OT_LIFT_PHONE:
		liftPhone.GetBitmap(&tmpBM);
		break;
	case OT_DROP_PHONE:
		dropPhone.GetBitmap(&tmpBM);
		break;
	case OT_WAIT_CALL:
		waitCall.GetBitmap(&tmpBM);
		break;
	case OT_PLAY_FILE:
		playFile.GetBitmap(&tmpBM);
		break;
	case OT_ASK_QUESTION:
		askQuestion.GetBitmap(&tmpBM);
		break;
	case OT_PROMPT_CODE:
		promptCode.GetBitmap(&tmpBM);
		break;
	case OT_REC_MSG:
		recMsg.GetBitmap(&tmpBM);
		break;
	case OT_PLAY_ALL:
		playAll.GetBitmap(&tmpBM);
		break;
	case OT_ERASE_ALL:
		eraseAll.GetBitmap(&tmpBM);
		break;
	case OT_ANSWER_ON:
		answerOn.GetBitmap(&tmpBM);
		break;
	case OT_ANSWER_OFF:
		answerOff.GetBitmap(&tmpBM);
		break;
	case OT_LABEL:
		label.GetBitmap(&tmpBM);
		break;
	default:
		happySun.GetBitmap(&tmpBM);
		break;
	}


	m_rect.SetRect(	startPoint.x, 
					startPoint.y, 
					tmpBM.bmWidth + startPoint.x, 
					tmpBM.bmHeight + startPoint.y );

	m_identity = identity;
	m_selected = FALSE;
	m_clipboardBuddy = NULL;
	m_invisable = FALSE;
	m_announcement = "";
	m_PINcode = "";
}


void	CScriptObject::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) ) {
		ar << m_rect;
		ar << m_objectType;
		ar << m_identity;
		ar << m_labelName;
		ar << m_announcement;
		ar << m_PINcode;
/*		for (i=0 ; i < NUM_TRANS_TYPE ; i++) {
			ar << m_transitionFlags[i];
		}	*/
	} else {
		ar >> m_rect;
		ar >> m_objectType;
		ar >> m_identity;
		ar >> m_labelName;
		ar >> m_announcement;
		ar >> m_PINcode;
/*		for (i=0 ; i < NUM_TRANS_TYPE ; i++) {
			ar >> m_transitionFlags[i];
		}	*/
	}
}

BOOL	CScriptObject::DrawObject( CDC* dc )
{
	CBitmap*	pbmOld = NULL;

	if (!m_invisable) {

		CDC dcMem;
		dcMem.CreateCompatibleDC(dc);

		switch (m_objectType) {
		case OT_HAPPY_SUN:
			pbmOld = dcMem.SelectObject(&happySun);
			break;
		case OT_WAIT_CALL:
			pbmOld = dcMem.SelectObject(&waitCall);
			break;
		case OT_LIFT_PHONE:
			pbmOld = dcMem.SelectObject(&liftPhone);
			break;
		case OT_DROP_PHONE:
			pbmOld = dcMem.SelectObject(&dropPhone);
			break;
		case OT_PLAY_FILE:
			pbmOld = dcMem.SelectObject(&playFile);
			break;
		case OT_ASK_QUESTION:
			pbmOld = dcMem.SelectObject(&askQuestion);
			break;
		case OT_PROMPT_CODE:
			pbmOld = dcMem.SelectObject(&promptCode);
			break;
		case OT_REC_MSG:
			pbmOld = dcMem.SelectObject(&recMsg);
			break;
		case OT_PLAY_ALL:
			pbmOld = dcMem.SelectObject(&playAll);
			break;
		case OT_ERASE_ALL:
			pbmOld = dcMem.SelectObject(&eraseAll);
			break;
		case OT_ANSWER_ON:
			pbmOld = dcMem.SelectObject(&answerOn);
			break;
		case OT_ANSWER_OFF:
			pbmOld = dcMem.SelectObject(&answerOff);
			break;
		case OT_LABEL:
			pbmOld = dcMem.SelectObject(&label);
			break;
		default:
			pbmOld = dcMem.SelectObject(&happySun);
			break;
		}

		dc->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &dcMem, 0, 0, /*0x00990066*/SRCCOPY);

		dcMem.SelectObject(pbmOld);
		dcMem.DeleteDC();
		
		if (m_objectType == OT_LABEL) {

			CPoint	textCenter = m_rect.CenterPoint();

			textCenter.Offset(0, 4);

			dc->MoveTo(textCenter);

			dc->SetTextAlign(TA_CENTER | TA_BASELINE | TA_UPDATECP);

			CFont*		fntOld = NULL;
			CFont		tmpFont;

			tmpFont.CreatePointFont(90, "Arial", dc);

			fntOld = dc->SelectObject(&tmpFont);

			dc->SetBkMode(TRANSPARENT);

			dc->TextOut(0, 0, m_labelName);

			dc->SelectObject(fntOld);

			CSize	textSize = dc->GetOutputTextExtent(m_labelName);

			m_textRect.left = textCenter.x - textSize.cx / 2;
			m_textRect.right = m_textRect.left + textSize.cx;
			m_textRect.top = textCenter.y - textSize.cy / 2;
			m_textRect.bottom = m_textRect.top + textSize.cy;

			m_textRect.InflateRect(0, 2, 0, 0);
		}

		if (m_selected) {
			CBrush	tmpBrush((COLORREF) 0x000000FF);

			dc->FrameRect(&m_rect, &tmpBrush);
		}
	}
	return TRUE;
}

void	CScriptObject::HideObject( CDC* dc)
{
//	dc->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), NULL, 0, 0, WHITENESS);

	dc->FillSolidRect(m_rect, 0x00FFFFFF);
	dc->FillSolidRect(m_textRect, 0x00FFFFFF);
}

void	CScriptObject::MoveObject( CPoint dstPoint, CDC* dc )
{
	if (dc) HideObject(dc);
	m_rect.OffsetRect(dstPoint - m_rect.TopLeft());
	if (dc) DrawObject(dc);
}

void	CScriptObject::OffsetObject(CPoint offsetVector, CDC* dc)
{
	if (dc) HideObject(dc);
	m_rect.OffsetRect(offsetVector);
	if (dc) DrawObject(dc);
}


BOOL	CScriptObject::IsHit(CPoint p)
{
	if (m_rect.PtInRect(p)) return TRUE;

	return FALSE;
}


void CScriptObject::SetSelected(BOOL selected, CDC* dc)
{
	m_selected = selected;
	if (dc != NULL) {
		DrawObject(dc);
	}

	POSITION pos = m_fromHereList.GetHeadPosition( );
	while (pos != NULL) {
		m_fromHereList.GetNext(pos)->SetSelected(selected, dc);
	}

	pos = m_toHereList.GetHeadPosition( );
	while (pos != NULL) {
		m_toHereList.GetNext(pos)->SetSelected(selected, dc);
	}
}

BOOL CScriptObject::IsPartOf(CRect rect)
{
	CRect	tmpRect;

	return tmpRect.IntersectRect(m_rect, rect);
}

void CScriptObject::DuplicateObject(CScriptObject* copyMe)
{
	m_objectType = copyMe->m_objectType;
	m_rect = copyMe->m_rect;
	m_labelName = copyMe->GetLabel();
	m_announcement = copyMe->GetAnnouncement();
	m_PINcode = copyMe->GetPINCode();

}

CPoint	CScriptObject::GetFromHerePoint( void )
{
	return CPoint(m_rect.left + (m_rect.Width() / 2), m_rect.bottom);
}

CPoint	CScriptObject::GetToHerePoint( void )
{
	return CPoint(m_rect.left + (m_rect.Width() / 2), m_rect.top);
}

BOOL CScriptObject::IsTransitionOccupied(int transIndex)
{
	POSITION pos = m_fromHereList.GetHeadPosition( );
	while (pos != NULL) {
		if (m_fromHereList.GetNext(pos)->m_transitionType == transIndex) return TRUE;
	}
	return FALSE;
}



























IMPLEMENT_SERIAL( CTransition, CObject, 1 )


CTransition::CTransition()
{
	m_textRect.SetRectEmpty();
	m_selected = FALSE;
	m_invisable = FALSE;
}

CTransition::CTransition(CScriptObject* srcObject, CScriptObject* dstObject, int transitionType)
{
	m_srcObject = srcObject;
	m_dstObject = dstObject;
	m_transitionType = transitionType;
	m_textRect.SetRectEmpty();
	m_selected = FALSE;
	m_invisable = FALSE;
}

void	CTransition::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) ) {
		ar << m_transitionType;
		ar << m_srcObject->GetIdentity();
		ar << m_dstObject->GetIdentity();
	} else {
		ar >> m_transitionType;
		ar >> m_srcIdentity;
		ar >> m_dstIdentity;
	}
}


BOOL	CTransition::DrawTransition(CDC* dc)
{
	if (!m_invisable) {
		CPoint	startP = m_srcObject->GetFromHerePoint();
		CPoint	endP = m_dstObject->GetToHerePoint();

		CPen	normPen(PS_SOLID, 1, (COLORREF) 0x00000000);
		CPen	selPen(PS_SOLID, 1, (COLORREF) 0x000000FF);

		CPen*	oldPen;
		
		if (m_selected) {
			oldPen = (CPen*) dc->SelectObject(&selPen);
			dc->SetTextColor(0x000000FF);
		} else {
			oldPen = (CPen*) dc->SelectObject(&normPen);
			dc->SetTextColor(0x00000000);
		}

		dc->MoveTo(startP);
		dc->LineTo(endP);

		CRect diagRect( startP.x, startP.y, endP.x, endP.y);

		diagRect.NormalizeRect();

		CFont*		fntOld = NULL;
		CFont		tmpFont;

		tmpFont.CreatePointFont(90, "Arial", dc);

		fntOld = dc->SelectObject(&tmpFont);

		dc->SetBkMode(OPAQUE);

		CPoint	textCenter = diagRect.CenterPoint();

		dc->MoveTo(textCenter);

		dc->SetTextAlign(TA_CENTER | TA_BASELINE | TA_UPDATECP);

		dc->TextOut(0, 0, TransTypeStrings[m_transitionType-1]);

		dc->SelectObject(oldPen);
		dc->SelectObject(fntOld);

		CSize	textSize = dc->GetOutputTextExtent(TransTypeStrings[m_transitionType-1]);

		m_textRect.left = textCenter.x - textSize.cx / 2;
		m_textRect.right = m_textRect.left + textSize.cx;
		m_textRect.top = textCenter.y - textSize.cy / 2;
		m_textRect.bottom = m_textRect.top + textSize.cy;

		m_textRect.InflateRect(0, 2, 0, 0);
	}
	return TRUE;
}


BOOL	CTransition::HideTransition( CDC* dc)
{
	CPoint	startP = m_srcObject->GetFromHerePoint();
	CPoint	endP = m_dstObject->GetToHerePoint();

	CPen*	oldPen;

	CPen	tmpPen(PS_SOLID, 1, (COLORREF) 0x00FFFFFF);
	oldPen = (CPen*) dc->SelectObject(&tmpPen);

	dc->MoveTo(startP);
	dc->LineTo(endP);

	dc->FillSolidRect(m_textRect, 0x00FFFFFF);

	dc->SelectObject(oldPen);

	return TRUE;

}

BOOL	CTransition::IsHit(CPoint point)
{
	return m_textRect.PtInRect(point);
}
/*
BOOL	CTransition::IsPartOf(CRect rect)
{
	return CRect::IntersectRect(GetTextRect(), rect);
}
*/
void	CTransition::SetSelected(BOOL selected, CDC* dc)
{
	m_selected = selected;
	if (dc != NULL) {
		DrawTransition(dc);
	}
}

void CTransition::SetInvisability(BOOL invisable)
{
	m_invisable = invisable;
}

/*
CRect	CTransition::GetTextRect( void )
{
	CPoint	startP = m_srcObject->GetFromHerePoint();
	CPoint	endP = m_dstObject->GetToHerePoint();

	CRect diagRect( startP.x, startP.y, endP.x, endP.y);

	diagRect.NormalizeRect();

	CPoint	textCenter = diagRect.CenterPoint();

	CSize	textSize = dc->GetOutputTextExtent("Hello");

	CRect	hideRect;

	hideRect.left = textCenter.x - textSize.cx / 2;
	hideRect.right = hideRect.left + textSize.cx;
	hideRect.top = textCenter.y - textSize.cy / 2;
	hideRect.bottom = hideRect.top + textSize.cy;

	hideRect.InflateRect(0, 2, 0, 0);

	return hideRect;
}
*/



