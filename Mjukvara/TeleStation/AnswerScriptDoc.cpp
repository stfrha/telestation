 // AnswerScriptDoc.cpp : implementation of the CAnswerScriptDoc class
//

#include "stdafx.h"
#include "TeleStation.h"
#include "TSDocument.h"

#include "AnswerScriptDoc.h"
#include "LabelPropDlg.h"
#include "PlayFilePropDlg.h"
#include "EnterCodeDlg.h"
#include "NoPropDlg.h"
#include "SelectTransDlg.h"
#include "BranchPropDlg.h"
#include "ProjectDoc.h"
#include "FileGenDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "AnswerScriptView.h"


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
extern CBitmap	categoryBranch;
extern CBitmap	playMemo;

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
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int DropPhoneTransList[] = {TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int WaitCallTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_1, TTI_0, TTI_0, TTI_1, TTI_0};

int PlayFileTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AskQuestTransList[] = {	TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, 
							TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, TTI_1, 
							TTI_1, TTI_1, TTI_0, TTI_0, TTI_0, TTI_0};
	
int PromptCodeTransList[]= {TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_1, TTI_1, TTI_1, TTI_1, TTI_0, TTI_0};

int RecMsgTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_1, 
							TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int EraseAllTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AnsOnTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int AnsOffTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int PlayAllTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int LabelTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};

int BranchTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_1};

int PlayMemoTransList[] = {	TTI_1, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, 
							TTI_0, TTI_0, TTI_0, TTI_0, TTI_0, TTI_0};


/////////////////////////////////////////////////////////////////////////////
// CAnswerScriptDoc

IMPLEMENT_DYNCREATE(CAnswerScriptDoc, CTSDocument)

BEGIN_MESSAGE_MAP(CAnswerScriptDoc, CTSDocument)
	//{{AFX_MSG_MAP(CAnswerScriptDoc)
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
// CAnswerScriptDoc construction/destruction

CAnswerScriptDoc::CAnswerScriptDoc()
{
	m_startObject = NULL;
}

CAnswerScriptDoc::~CAnswerScriptDoc()
{
}

BOOL CAnswerScriptDoc::OnNewDocument()
{
	if (!CTSDocument::OnNewDocument())
		return FALSE;

	InitDocument();

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAnswerScriptDoc serialization

void CAnswerScriptDoc::Serialize(CArchive& ar)
{
	m_objectList.Serialize(ar);
	m_transitionList.Serialize(ar);

	if (ar.IsStoring())	{
	} else {
		PostSerializeProcessData();
	}

}

/////////////////////////////////////////////////////////////////////////////
// CAnswerScriptDoc diagnostics

#ifdef _DEBUG
void CAnswerScriptDoc::AssertValid() const
{
	CTSDocument::AssertValid();
}

void CAnswerScriptDoc::Dump(CDumpContext& dc) const
{
	CTSDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnswerScriptDoc commands

void CAnswerScriptDoc::InitDocument()
{
	m_sizeDoc = CSize(8000,9000);
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



BOOL CAnswerScriptDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CTSDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	InitDocument();

	m_projectDoc = NULL;	//Förutsätt att tillhörande dokument inte är öppet.

	CTeleStationApp* app = (CTeleStationApp*) AfxGetApp();
	CProjectDoc* pDoc = app->GetCurrentProjDoc();

	if (pDoc) {
		CString tmpStr = lpszPathName;
		tmpStr.MakeUpper();
		CString tmpStr2 = pDoc->GetItemPath(2);
		tmpStr2.MakeUpper();
		if (tmpStr == tmpStr2) {
			m_projectDoc = pDoc;
			pDoc->RegisterDocumentOpen(this, 2);
		}
	}
	
	return TRUE;
}

void CAnswerScriptDoc::DeleteContents() 
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

	CTSDocument::DeleteContents();
}





CScriptObject* CAnswerScriptDoc::NewObject(CPoint startPoint, int objectType)
{
	CScriptObject* newObject = new CScriptObject(startPoint, objectType, GetUniqueIdentity());

	m_objectList.AddTail(newObject);

	return newObject;
}

void CAnswerScriptDoc::DeleteObject(CScriptObject *deleteMe)
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

CScriptObject*	CAnswerScriptDoc::IsObjectHit(CPoint point)
{
	CScriptObject*	tmpObj;

	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		if (tmpObj->IsHit(point)) return tmpObj;
	}
	return NULL;
}





CTransition* CAnswerScriptDoc::NewTransition(CScriptObject* srcObj, CScriptObject* dstObj, int transitionType)
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

void CAnswerScriptDoc::DeleteTransition(CTransition* deleteMe)
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

CTransition*	CAnswerScriptDoc::IsTransitionHit(CPoint point)
{
	CTransition*	tmpTrans;

	POSITION pos = m_transitionList.GetHeadPosition( );
	while (pos != NULL) {
		tmpTrans = m_transitionList.GetNext(pos);
		if (tmpTrans->IsHit(point)) return tmpTrans;
	}
	return NULL;
}



void	CAnswerScriptDoc::ClearSelection( CDC* dc)
{
	while( !m_selectionList.IsEmpty( ) ) {
		m_selectionList.RemoveHead()->SetSelected(FALSE, dc);
	}
	
	if (m_selectedTransition) {
		m_selectedTransition->SetSelected(FALSE, dc);
		m_selectedTransition = NULL;
	}
}

void	CAnswerScriptDoc::RemoveFromSelection(CScriptObject* removeMe, CDC* dc)
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


void	CAnswerScriptDoc::SetSelection(BOOL select, CRect selectionRect, CDC* dc)
{
	ClearSelection(dc);

	if (select) {
		AddToSelection(selectionRect, dc);
	}
}

void	CAnswerScriptDoc::AddToSelection(CScriptObject* newSelected, CDC* dc)
{
	if (!newSelected->IsSelected()) {
		m_selectionList.AddHead(newSelected);
	}
	newSelected->SetSelected(TRUE, dc);
}

void	CAnswerScriptDoc::AddToSelection(CRect selectionRect, CDC* dc)
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


void	CAnswerScriptDoc::OnDrawDoc(CDC* dc)
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

BOOL	CAnswerScriptDoc::IsSelectionHit(CPoint point)
{
	POSITION pos = m_selectionList.GetHeadPosition( );
	while (pos != NULL) {
		if (m_selectionList.GetNext(pos)->IsHit(point)) return TRUE;
	}
	return FALSE;
}

void	CAnswerScriptDoc::HideSelection(CDC* dc)
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


void	CAnswerScriptDoc::MoveSelection(CDC* dc, CPoint dstPoint)
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

void CAnswerScriptDoc::ClearClipboard()
{
	while( !clipboardObjectList.IsEmpty( ) ) {
		delete clipboardObjectList.RemoveHead( );
	}	

	while( !clipboardTransitionList.IsEmpty( ) ) {
		delete clipboardTransitionList.RemoveHead( );
	}	
}

void CAnswerScriptDoc::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_selectionList.IsEmpty());	
}

void CAnswerScriptDoc::OnEditCopy() 
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

void CAnswerScriptDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_selectionList.IsEmpty());	
}

void CAnswerScriptDoc::OnEditCut() 
{
	OnEditCopy();
	OnEditDelete();
}


void CAnswerScriptDoc::OnEditDelete() 
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

void CAnswerScriptDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((!m_selectionList.IsEmpty()) || (m_selectedTransition != NULL));	
}

void CAnswerScriptDoc::OnEditPaste() 
{
	m_currentView->OnEditPaste();
}

void CAnswerScriptDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!clipboardObjectList.IsEmpty());	
}

void CAnswerScriptDoc::OnEditSelectAll() 
{
	ClearSelection();

	POSITION pos = m_objectList.GetHeadPosition( );
	while (pos != NULL) {
		AddToSelection(m_objectList.GetNext(pos));
	}
	UpdateAllViews(NULL);
}

int CAnswerScriptDoc::GetUniqueIdentity( void )
{
	int testIdentity = 0;

	while ((OccupiedIdentity(testIdentity)) && (testIdentity < 32768)) {
		testIdentity++;
	}

	return testIdentity;
}

BOOL CAnswerScriptDoc::OccupiedIdentity(int i)
{
	POSITION pos = m_objectList.GetHeadPosition();
	while (pos != NULL) {
		if (m_objectList.GetNext(pos)->GetIdentity() == i) return TRUE;
	}
	return FALSE;
}

void CAnswerScriptDoc::PostSerializeProcessData( void )
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

CScriptObject*	CAnswerScriptDoc::FindObject(int identity)
{
	CScriptObject*	tmpObj;
	
	POSITION pos = m_objectList.GetHeadPosition();

	while (pos != NULL) {
		tmpObj = m_objectList.GetNext(pos);
		if (tmpObj->GetIdentity() == identity) return tmpObj;
	}
	return NULL;
}

void CAnswerScriptDoc::OnObjectProp() 
{
	if (m_selectionList.GetCount() == 1) {

		CLabelPropDlg		labelDlg;
		CPlayFilePropDlg	playDlg;
		CEnterCodeDlg		codeDlg;
		CNoPropDlg			noDlg;
		CBranchPropDlg		branchDlg;
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
		case OT_BRANCH:
			if (m_projectDoc) {
				branchDlg.m_catList = m_projectDoc->GetPhoneBookList();
				CString	tmpStr = tmpObj->GetAnnouncement();
				branchDlg.m_source = &tmpStr;
				if (branchDlg.m_catList) {
					if (branchDlg.DoModal() == IDOK) {
						if (branchDlg.m_catSelEdit != tmpObj->GetAnnouncement()) {
							tmpObj->SetAnnouncement(branchDlg.m_catSelEdit);
							SetModifiedFlag(TRUE);
						}
					}
					break;
				}
			}
			MessageBox(NULL, "No phone book associated", "Categories not available", MB_OK);
			break;
		case OT_PLAY_MEMO:
		default:
			if (!m_skipNoProperty) {
				noDlg.DoModal();
			}
			break;
		}
	}
	m_skipNoProperty = FALSE;
}

void CAnswerScriptDoc::OnUpdateObjectProp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selectionList.GetCount() == 1);	
}

void CAnswerScriptDoc::AskForProperties( void )
{
	m_skipNoProperty = TRUE;
	OnObjectProp();
}

int	CAnswerScriptDoc::AskForTransitionType(CScriptObject* srcObj)
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
	case OT_BRANCH:
		defaultFlag = BranchTransList;
		break;
	case OT_PLAY_MEMO:
		defaultFlag = PlayMemoTransList;
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

void CAnswerScriptDoc::OnCloseDocument() 
{
	if (m_projectDoc) m_projectDoc->InvalidateScriptDoc();
	
	CTSDocument::OnCloseDocument();
}

/*
BOOL 	CAnswerScriptDoc::IsScriptValid( void )
{
	//Finns en label med namnet "Start"
	//Finns en label med namnet "End"
	//Hamnar man alltid i "Start" efter "End"
}


*/

BOOL CAnswerScriptDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	BOOL	ans;

	ans = CTSDocument::CanCloseFrame(pFrame);
	
	if (ans) {
		if (m_projectDoc) m_projectDoc->SaveViewPlacement(pFrame, 2);
	
	}

	return ans;
}


void CAnswerScriptDoc::InvokeObjectProperty( void )
{
	OnObjectProp();
}

void CAnswerScriptDoc::GenerateScript(CFileGenDlg* dlg)
{
}

void CAnswerScriptDoc::GenResetObjects( void )
{
	CScriptObject*	tmpObj;
	POSITION pos = m_objectList.GetHeadPosition();
	while (pos) {
		tmpObj = m_objectList.GetNext(pos);
		tmpObj->m_processed = FALSE;
		tmpObj->m_writtenToFile = FALSE;
		tmpObj->m_labelIndependent = FALSE;
		tmpObj->m_numCalledBy = 0;
		tmpObj->m_postGoto = "";
		tmpObj->m_genLabel = "";
	}

	m_startObject = NULL;
}

BOOL CAnswerScriptDoc::DRC(CFileGenDlg* dlg)
/*--------------------------------------------------

Fatala regler:
*	Om det finns två eller fler label med samma namn
	får endast en ha en transition från sig

*	Det måste finnas en label som heter "start" och som
	har en transition från sig

Varningsregler:
*	Skriptet skall vara cykliskt, dvs alla möjliga vägar 
	skall gå till start labeln

*	Skriptet skall innehålla ett WaitRing-, LiftPhone-
	och DropPhone objekt


Retunerar TRUE om succe
----------------------------------------------------*/
{
	CTypedPtrList<CObList, CScriptObject*>	labelList;
	CTypedPtrList<CObList, CScriptObject*>	tmpList;
	CScriptObject*	tmpObject;
	CString			labelName;
	CString			msg;
	POSITION		pos1, pos2,pos3;
	BOOL			transFrom;
	BOOL			startExists;

	CWaitCursor		cwc;

	startExists = FALSE;

	//Checking label integrity
	dlg->InsertString("Performing script DRC:");
	dlg->InsertString("Checking label integrity...");
	pos1 = m_objectList.GetHeadPosition();
	while (pos1) {
		tmpObject = m_objectList.GetNext(pos1);
		if (tmpObject->GetType() == OT_LABEL) {
			if (!FindDepartureLabel(tmpObject->GetLabel())) {
				msg.Format("ERROR: Label -%s- has no corresponding label with departure", tmpObject->GetLabel());
				dlg->InsertString(msg);
				return FALSE;
			}
			labelList.AddHead(tmpObject);
			if (tmpObject->GetLabel() == "start") {
				startExists = TRUE;
				if (!tmpObject->m_fromHereList.IsEmpty()) {
					m_startObject = tmpObject;
				}
			}
		}
	}
	if (!startExists) {
		dlg->InsertString("ERROR: No label named -start- exists");
		return FALSE;
	}
	if (!m_startObject) {
		dlg->InsertString("ERROR: Start-label exists but has no departure");
		return FALSE;
	}


	while (labelList.GetCount() > 0) {
		pos1 = labelList.GetHeadPosition();
		pos2 = pos1;
		tmpObject = labelList.GetNext(pos1);
		labelName = tmpObject->GetLabel();
		tmpList.AddHead(tmpObject);
		labelList.RemoveAt(pos2);
		while (pos1) {
			pos2 = pos1;
			tmpObject = labelList.GetNext(pos1);
			if (tmpObject->GetLabel() == labelName) {
				tmpList.AddHead(tmpObject);
				labelList.RemoveAt(pos2);
			}
		}
		transFrom = FALSE;
		if (tmpList.GetCount() > 0) {
			pos3 = tmpList.GetHeadPosition();
			while (pos3) {
				tmpObject = tmpList.GetNext(pos3);
				if (tmpObject->m_fromHereList.GetCount() > 0) {
					if (transFrom) {
						msg.Format("ERROR: Duplicate label -%s- with departuring transition", tmpObject->GetLabel());
						dlg->InsertString(msg);
						return FALSE;
					} else {
						transFrom = TRUE;
					}
				}
			}
			tmpList.RemoveAll();
		}
	}

	dlg->InsertString("Checking script flow...");

	pos1 = m_objectList.GetHeadPosition();
	while (pos1) {
		tmpObject = m_objectList.GetNext(pos1);
		if ((tmpObject->GetType() != OT_LABEL) && (tmpObject->m_toHereList.IsEmpty())) {
			dlg->InsertString("WARNING: Unreachable objects");
		}
	}

/*
	if (!LeadsToStart(m_startObject, m_startObject)) {
		dlg->InsertString("WARNING: Not all paths leads to start-label");
	}
*/
	dlg->InsertString("Script DRC Finished");
	dlg->InsertString("Processing script objects:");
	dlg->InsertString("Building labels...");
	m_genLabelSeed = 0;
	pos1 = m_objectList.GetHeadPosition();
	while (pos1) {
		tmpObject = m_objectList.GetNext(pos1);
		if (tmpObject->GetType() != OT_LABEL) {
			tmpObject->m_genLabel = GenerateGenLabelName(tmpObject);
		}
	}
	return TRUE;
}


void CAnswerScriptDoc::GenerateScriptFile(CFileGenDlg* dlg)
//DRC måste vara körd innan detta körs
{
	FILE*	fp;
	BOOL	dummy;
	char		path_buffer[_MAX_PATH];
	char		drive[_MAX_DRIVE];
	char		dir[_MAX_DIR];
	char		fname[_MAX_FNAME];
	char		ext[_MAX_EXT];
	CString		cdrive, cdir, cfname;
	CString		sPath;
	CString		tmpStr;


	strcpy(path_buffer, GetPathName());

	_splitpath(path_buffer, drive, dir, fname, ext );
	cdrive = drive;
	cdir = dir;
	cfname = fname;
	sPath = cdrive + cdir + "Ans.bat";

	dlg->InsertString("Script file generation:");
	fp = fopen(sPath, "wt");
	if (fp) {
		dlg->InsertString("Creating instructions...");
		CreateInstruction(FindDstObject(m_startObject->m_fromHereList.GetHead()), m_startObject->m_fromHereList.GetHead()->m_transitionType, &dummy);
		dlg->InsertString("Writing instructions...");
		WriteInstruction(fp, FindDstObject(m_startObject->m_fromHereList.GetHead()));
		fclose(fp);
		dlg->InsertString("Script file -Ans.bat- succesfully generated");
	} else {
		dlg->InsertString("ERROR: Could not open output script file");
	}
}

BOOL CAnswerScriptDoc::LeadsToStart(CScriptObject* obj, CScriptObject* startObj)
{
	CTransition*	tmpTrans;

	if ((obj->GetType() == OT_LABEL) && (obj->m_fromHereList.IsEmpty())) {	//Är detta en "mottagande" label?
		obj = FindDepartureLabel(obj->GetLabel());									//JA, kolla motsv avgående label
		if (obj == NULL) {													//Bör aldrig kunna hända
			MessageBox(NULL, "Didn't find departure label", "Strange error", MB_OK);
			return FALSE;
		}
																			
		if (obj == startObj) return TRUE;									//det kan vara startlabeln
	}

	POSITION pos = obj->m_fromHereList.GetHeadPosition();
	while (pos) {
		tmpTrans = obj->m_fromHereList.GetNext(pos);
		if (tmpTrans->m_dstObject == startObj) return TRUE;
		if (LeadsToStart(tmpTrans->m_dstObject, startObj)) return TRUE;
	}
	return FALSE;
}

CScriptObject*	CAnswerScriptDoc::FindDepartureLabel(CString label)
{
	POSITION pos = m_objectList.GetHeadPosition();
	CScriptObject*	tmpObj;

	while (pos) {
		tmpObj = m_objectList.GetNext(pos);
		if ((tmpObj->GetType() == OT_LABEL) && (tmpObj->GetLabel() == label) && 
			(!tmpObj->m_fromHereList.IsEmpty())) return tmpObj;
	}
	return NULL;
}

CString	CAnswerScriptDoc::GenerateGenLabelName(CScriptObject* obj)
{
	CString			tmp;
	CTransition*	tmpTrans;

	POSITION pos = obj->m_toHereList.GetHeadPosition();
	while (pos) {
		tmpTrans = obj->m_toHereList.GetNext(pos);
		if (tmpTrans->m_srcObject->GetType() == OT_LABEL) {
//			tmp = obj->m_toHereList.GetHead()->m_srcObject->GetLabel();
			tmp = tmpTrans->m_srcObject->GetLabel();
			if (!DoesGenLabelExist(tmp)) {
				return tmp;
			}
		}
	}

	CString genLabel;

	do {
		genLabel.Format("%s%d", "Label", m_genLabelSeed ++);
		
	} while (DoesGenLabelExist(genLabel));
	return genLabel;
}

BOOL	CAnswerScriptDoc::DoesGenLabelExist(CString label)
{
	CString	tmp;

	POSITION pos = m_objectList.GetHeadPosition();
	while (pos) {
		//tmp = m_objectList.GetNext(pos)->GetLabel();
		tmp = m_objectList.GetNext(pos)->m_genLabel;
		if (tmp == label) {
			return TRUE;
		}
	}
	return FALSE;
}

CString CAnswerScriptDoc::CreateInstruction(CScriptObject* obj, int transType, BOOL* processed)
{
	POSITION		pos;
	CTransition*	tmpTrans;
	CString			tmpStr;
	BOOL			didProcess;

	*processed = FALSE;	

	obj->m_numCalledBy++;
	if ((transType == TT_DEFAULT) && (obj->m_numCalledBy == 1)) {
		obj->m_labelIndependent = TRUE;
	} else  {
		obj->m_labelIndependent = FALSE;
	}

	if (!obj->m_processed) {
		*processed = TRUE;
		obj->m_processed = TRUE;
		obj->m_instructionStr = GetInstruction(obj);
		pos = obj->m_fromHereList.GetHeadPosition();
		//TT_DEFAULT har ingen switch utan utförs omedelbart efter instruktionen
		while (pos) {
			tmpTrans = obj->m_fromHereList.GetNext(pos);
			if (tmpTrans->m_transitionType == TT_DEFAULT) {
				tmpStr = CreateInstruction(FindDstObject(tmpTrans), tmpTrans->m_transitionType, &didProcess);
				if (!didProcess) {
					obj->m_postGoto = "GOTO " + tmpStr;
				}
			}
		}
		pos = obj->m_fromHereList.GetHeadPosition();
		while (pos) {
			tmpTrans = obj->m_fromHereList.GetNext(pos);
			if (tmpTrans->m_transitionType != TT_DEFAULT) {
				tmpTrans->m_genSwitch = GetSwitch(tmpTrans) + CreateInstruction(FindDstObject(tmpTrans), tmpTrans->m_transitionType, &didProcess);
			}
		}
	}
	return obj->m_genLabel;
}

void	CAnswerScriptDoc::WriteInstruction(FILE* fp, CScriptObject* obj)
{
	POSITION		pos;
	CTransition*	tmpTrans;

	if (!obj->m_writtenToFile) {
		obj->m_writtenToFile = TRUE;
		if (!obj->m_labelIndependent) {
			fprintf(fp, "%s%s\n", ":", obj->m_genLabel);
		}
		fprintf(fp, "%s ", obj->m_instructionStr);

		pos = obj->m_fromHereList.GetHeadPosition();
		while (pos) {
			tmpTrans = obj->m_fromHereList.GetNext(pos);
			if (tmpTrans->m_transitionType != TT_DEFAULT) {
				fprintf(fp, "%s ", tmpTrans->m_genSwitch);
			}
		}
		fprintf(fp, "\n");
		if (!obj->m_postGoto.IsEmpty()) {
			fprintf(fp, "%s\n", obj->m_postGoto);
		}

		pos = obj->m_fromHereList.GetHeadPosition();
		//TT_DEFAULT har ingen switch utan utförs omedelbart efter instruktionen
		while (pos) {
			tmpTrans = obj->m_fromHereList.GetNext(pos);
			if (tmpTrans->m_transitionType == TT_DEFAULT) {
				WriteInstruction(fp, FindDstObject(tmpTrans));
			}
		}
		pos = obj->m_fromHereList.GetHeadPosition();
		while (pos) {
			tmpTrans = obj->m_fromHereList.GetNext(pos);
			if (tmpTrans->m_transitionType != TT_DEFAULT) {
				WriteInstruction(fp, FindDstObject(tmpTrans));
			}
		}
	}
}

CScriptObject*	CAnswerScriptDoc::FindDstObject(CTransition* trans)
{
	CScriptObject* tmpObj = NULL;

	while (trans->m_dstObject->GetType() == OT_LABEL) {
		trans = (FindDepartureLabel(trans->m_dstObject->GetLabel()))->m_fromHereList.GetHead();
	}
	return trans->m_dstObject;
}

CString CAnswerScriptDoc::GetInstruction(CScriptObject* obj)
{
	switch(obj->GetType()) {
	case OT_LIFT_PHONE:
		return "PU";
	case OT_DROP_PHONE:
		return "PD";
	case OT_WAIT_CALL:
		return "WAITCALL ";
	case OT_PLAY_FILE:
		return "PLAY " + obj->GetAnnouncement() + ".waw ";
	case OT_ASK_QUESTION:
		return "QUESTION " + obj->GetAnnouncement() + ".waw ";
	case OT_PROMPT_CODE:
		return "ENTERCODE " + obj->GetAnnouncement() + ".waw " + obj->GetPINCode() + " ";
	case OT_REC_MSG:
		return "RECMSG";
	case OT_ERASE_ALL:
		return "ERASEDYNAMIC";
	case OT_ANSWER_ON:
		return "ANSON";
	case OT_ANSWER_OFF:
		return "ANSOFF";
	case OT_PLAY_ALL:
		return "PLAYNEW";
	case OT_BRANCH:
		return "BRACATEQU "  + obj->GetAnnouncement();
	case OT_PLAY_MEMO:
		return "PLAYMEMO";
	}
	return "";
}

CString	CAnswerScriptDoc::GetSwitch(CTransition* trans)
{
	switch(trans->m_transitionType) {
	case TT_DTMF_0:
		return "L0=";
	case TT_DTMF_1:
		return "L1=";
	case TT_DTMF_2:
		return "L2=";
	case TT_DTMF_3:
		return "L3=";
	case TT_DTMF_4:
		return "L4=";
	case TT_DTMF_5:
		return "L5=";
	case TT_DTMF_6:
		return "L6=";
	case TT_DTMF_7:
		return "L7=";
	case TT_DTMF_8:
		return "L8=";
	case TT_DTMF_9:
		return "L9=";
	case TT_DTMF_STAR:
		return "LS=";
	case TT_DTMF_POUND:
		return "LP=";
	case TT_OTHER_DTMF:
		return "LD=";
	case TT_HANGUP:
		return "LH=";
	case TT_TIMEOUT:
		return "LT=";
	case TT_CORRECT_CODE:
		return "LC=";
	case TT_WRONG_CODE:
		return "LI=";
	case TT_OK_SIGNALS:
		return "LS=";
	case TT_CATEGORY_HIT:
		return "";
	}
	return "";
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
	m_processed = FALSE;
	m_writtenToFile = FALSE;
	m_labelIndependent = FALSE;
	m_numCalledBy = 0;
	m_postGoto = "";
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
	case OT_BRANCH:
		categoryBranch.GetBitmap(&tmpBM);
		break;
	case OT_PLAY_MEMO:
		playMemo.GetBitmap(&tmpBM);
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
	m_processed = FALSE;
	m_writtenToFile = FALSE;
	m_labelIndependent = FALSE;
	m_numCalledBy = 0;
	m_postGoto = "";
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
		case OT_BRANCH:
			pbmOld = dcMem.SelectObject(&categoryBranch);
			break;
		case OT_PLAY_MEMO:
			pbmOld = dcMem.SelectObject(&playMemo);
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
	m_genSwitch = "";
}

CTransition::CTransition(CScriptObject* srcObject, CScriptObject* dstObject, int transitionType)
{
	m_srcObject = srcObject;
	m_dstObject = dstObject;
	m_transitionType = transitionType;
	m_textRect.SetRectEmpty();
	m_selected = FALSE;
	m_invisable = FALSE;
	m_genSwitch = "";
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




