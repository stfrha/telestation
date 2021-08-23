// ProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TeleStation.h"
#include "AnswerScriptDoc.h"
#include "ProjectDoc.h"
#include "NewProjectDlg.h"
#include "ProjItemPropDlg.h"
#include "MainFrm.h"
#include <mmsystem.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc

IMPLEMENT_DYNCREATE(CProjectDoc, CDocument)

CProjectDoc::CProjectDoc()
{
	m_currentView = NULL;
	m_newProject = FALSE;
	m_simulator = NULL;
	m_scriptDoc = NULL;
}

BOOL CProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;

	CNewProjectDlg	myDlg;

	if (myDlg.DoModal() == IDOK) {

		m_tempProjectPath = myDlg.m_projectPath;

		SetPathName(m_tempProjectPath, TRUE);

		m_script.SetItemPath(myDlg.m_scriptPath);
		m_script.SetItemTitle(myDlg.m_scriptTitle);
		m_phoneBook.SetItemPath(myDlg.m_phoneBookPath);
		m_phoneBook.SetItemTitle(myDlg.m_phoneBookTitle);

		m_newProject = TRUE;

	} else {
		return FALSE;
	}

	InitDocument();

	return TRUE;
}

CProjectDoc::~CProjectDoc()
{
}


BEGIN_MESSAGE_MAP(CProjectDoc, CDocument)
	//{{AFX_MSG_MAP(CProjectDoc)
	ON_COMMAND(ID_PROJECT_SELECT_SCRIPT, OnProjectSelectScript)
	ON_COMMAND(ID_PROJECT_SELECT_PHONE_BOOK, OnProjectSelectPhoneBook)
	ON_COMMAND(ID_PROJECT_ADD_ANNOUNCEMENT, OnProjectAddAnnouncement)
	ON_COMMAND(ID_PROJECT_OPEN_SCRIPT, OnProjectOpenScript)
	ON_COMMAND(ID_PROJECT_PLAY_ANNOUNCEMENT, OnProjectPlayAnnouncement)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_PLAY_ANNOUNCEMENT, OnUpdateProjectPlayAnnouncement)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_OPEN_ANNOUNCEMENT, OnUpdateProjectOpenAnnouncement)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_DELETE_ANNOUNCEMENT, OnUpdateProjectDeleteAnnouncement)
	ON_COMMAND(ID_PROJECT_DELETE_ANNOUNCEMENT, OnProjectDeleteAnnouncement)
	ON_COMMAND(ID_PROJECT_OPEN_ANNOUNCEMENT, OnProjectOpenAnnouncement)
	ON_COMMAND(ID_PROJECT_NEW_ANNOUNCEMENT, OnProjectNewAnnouncement)
	ON_COMMAND(ID_PROJECT_PROPERTIES, OnProjectProperties)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_PROPERTIES, OnUpdateProjectProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc diagnostics

#ifdef _DEBUG
void CProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc serialization

void CProjectDoc::Serialize(CArchive& ar)
{
	m_script.Serialize(ar);
	m_phoneBook.Serialize(ar);
	m_announcementList.Serialize(ar);

	if (ar.IsStoring()) {
	} else {
//		PostSerializeProcessData();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc commands

void CProjectDoc::InitDocument()
{
//	DeleteContents();
}

void CProjectDoc::DeleteContents() 
{
/*	if (m_script != NULL) {
		delete m_script;
	}
	m_script = NULL;

	if (m_phoneBook) {
		delete m_phoneBook;
	}
	m_phoneBook = NULL;
*/
	while (!m_announcementList.IsEmpty()) {
		delete m_announcementList.RemoveHead();
	}

	CDocument::DeleteContents();
}

BOOL CProjectDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	InitDocument();

/*
	char	buffer[MAX_PATH];

	_getcwd(buffer, MAX_PATH);

	m_tempProjectPath = buffer;
	m_tempProjectPath += "\\";
	m_tempProjectPath += "dummy";	//GetTitle();
	m_tempProjectPath += ".tsp";
*/
	m_tempProjectPath = lpszPathName;

	return TRUE;
}


void  CProjectDoc::PostCreationProcessData( void )
{
	HTREEITEM	tmp;
	CProjectItem*	tmpPI;

	if (m_currentView == NULL) {
		MessageBox(NULL, "View not initialised", "Error", MB_OK);
		return;
	}

	m_currentView->SetScriptFile(m_script.GetItemTitle());
	m_currentView->SetPhoneBookFile(m_phoneBook.GetItemTitle());

	POSITION pos = m_announcementList.GetHeadPosition();
	while (pos != NULL) {
		tmpPI = m_announcementList.GetNext(pos); 
		tmp = m_currentView->InsertAnnouncementFile(tmpPI->GetItemTitle());
		tmpPI->SetTVItem(tmp);
	}

	if (m_newProject) CDocument::OnSaveDocument(m_tempProjectPath);
	m_newProject = FALSE;

}

void CProjectDoc::OnProjectSelectScript() 
{
	CFileDialog	psDlg(TRUE, "SRF", NULL, OFN_HIDEREADONLY | OFN_LONGNAMES |  OFN_NOCHANGEDIR |
				OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "Telestation Script Files (*.srf)|*.srf||",
				NULL);

	if (psDlg.DoModal() == IDOK) {

//		CString	relPath = CreateRelativePath(m_tempProjectPath, psDlg.GetPathName());

		m_script.SetItemPath(psDlg.GetPathName());
		m_script.SetItemTitle(psDlg.GetFileTitle());

		m_currentView->SetScriptFile(psDlg.GetFileTitle());

		SetModifiedFlag(TRUE);
	}
}

void CProjectDoc::OnProjectSelectPhoneBook() 
{
	MessageBox(NULL, "Function not implemented", "Have patience", MB_OK);
}

void CProjectDoc::OnProjectAddAnnouncement() 
{
	HTREEITEM	tmp;

	CFileDialog	psDlg(TRUE, "WAV", NULL, OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_NOCHANGEDIR |
				OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "Announcement Files (*.wav)|*.wav||",
				NULL);

	if (psDlg.DoModal() == IDOK) {

		POSITION pos = m_announcementList.GetHeadPosition();
		while (pos) {
			if (m_announcementList.GetNext(pos)->GetItemTitle() == psDlg.GetFileTitle()) {
				MessageBox(NULL, "The project already have an announcement with that title\nSelect another or rename file", "Announcement already added", MB_OK);
				return;
			}
		}

//		CString	relPath = CreateRelativePath(m_tempProjectPath, psDlg.GetPathName());

		CProjectItem*	newItem = new CProjectItem(psDlg.GetPathName(), psDlg.GetFileTitle());

		m_announcementList.AddTail(newItem);

		tmp = m_currentView->InsertAnnouncementFile(psDlg.GetFileTitle());
		newItem->SetTVItem(tmp);

		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}

void	CProjectDoc::AddAnnouncement( void )
{
	OnProjectAddAnnouncement();
}

void CProjectDoc::OnProjectOpenScript() 
{
	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();

	m_scriptDoc = (CAnswerScriptDoc*) myApp->OpenDocumentFile(m_script.GetItemPath());
	if (!m_scriptDoc) {
		m_scriptDoc = (CAnswerScriptDoc*) myApp->m_scriptTemplate->OpenDocumentFile(NULL, TRUE);
		if (m_scriptDoc) {
			m_scriptDoc->SetPathName(m_script.GetItemPath());
			m_scriptDoc->OnSaveDocument(m_script.GetItemPath());
		} else {
			return;
		}
	}
	m_scriptDoc->SetProjectDoc(this);
	if (m_simulator) {
		m_simulator->SetScriptDoc(m_scriptDoc);
		m_simulator->EnableOnSwitch(TRUE);
	}
}

void CProjectDoc::OnProjectNewAnnouncement() 
{
	MessageBox(NULL, "Function not implemented", "Have patience", MB_OK);
}

void CProjectDoc::OnProjectPlayAnnouncement() 
{
	CTreeCtrl& tc = m_currentView->GetTreeCtrl();

	PlaySound(GetAnnouncement(tc.GetSelectedItem())->GetItemPath(), NULL, SND_FILENAME | SND_ASYNC);
}

void CProjectDoc::PlayAnnouncement(CString title)
{
	CProjectItem* tmp;

	POSITION pos = m_announcementList.GetHeadPosition();
	while (pos) {
		tmp = m_announcementList.GetNext(pos);
		if (tmp->GetItemTitle() == title) {
			PlaySound(tmp->GetItemPath(), NULL, SND_FILENAME | SND_SYNC);
			return;
		}
	}
}

CString CProjectDoc::GetAnnouncementPath(CString title)
{
	CProjectItem* tmp;

	POSITION pos = m_announcementList.GetHeadPosition();
	while (pos) {
		tmp = m_announcementList.GetNext(pos);
		if (tmp->GetItemTitle() == title) {
			return tmp->GetItemPath();
		}
	}
	return "";
}


void CProjectDoc::OnUpdateProjectPlayAnnouncement(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_currentView->m_treeSelectionFlag == TSF_ANNOUNCE_FILE);
}

void CProjectDoc::OnProjectOpenAnnouncement() 
{
	MessageBox(NULL, "Function not implemented", "Have patience", MB_OK);
}

void CProjectDoc::OnUpdateProjectOpenAnnouncement(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_currentView->m_treeSelectionFlag == TSF_ANNOUNCE_FILE);
}

void CProjectDoc::OnProjectDeleteAnnouncement() 
{
	CTreeCtrl& tc = m_currentView->GetTreeCtrl();

	CProjectItem* tmpItem = GetAnnouncement(tc.GetSelectedItem());

	tc.DeleteItem(tc.GetSelectedItem());

	POSITION pos = m_announcementList.Find(tmpItem);

	if (pos) {
		m_announcementList.RemoveAt(pos);
	}

	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);

}

void CProjectDoc::OnUpdateProjectDeleteAnnouncement(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_currentView->m_treeSelectionFlag == TSF_ANNOUNCE_FILE);
}

CProjectItem* CProjectDoc::GetAnnouncement(HTREEITEM findMe) 
{
	CProjectItem* tmp;

	POSITION pos = m_announcementList.GetHeadPosition();
	while (pos) {
		tmp = m_announcementList.GetNext(pos);
		if (tmp->GetTVItem() == findMe) return tmp;
	}
	return NULL;
}

CString	CProjectDoc::CreateRelativePath(CString basePath, CString relPath)
{
	int		i;
	int		nDirs;
	int		differIndex;
	int		minLen;
	CString	retStr = "";

	CString		baseNoFile = basePath.Left(basePath.ReverseFind('\\') + 1);
	CString		relNoFile = relPath.Left(relPath.ReverseFind('\\') + 1);

	if (baseNoFile.GetLength() < relNoFile.GetLength()) {
		minLen = baseNoFile.GetLength();
	} else {
		minLen = relNoFile.GetLength();
	}

	for (i=0 ; (i<minLen) && (baseNoFile.GetAt(i) == relNoFile.GetAt(i)) ; i++);

	differIndex = i;
	nDirs = 0;

	if (i < baseNoFile.GetLength()) {
		//i = 8
		//Räkna directories i baseNoFile
		while (i != -1) {
			nDirs++;
			i = baseNoFile.Find('\\', i);
			if (i >= 0) {
				i++;
				retStr += "..\\";
			}
		}
	}

	retStr += relPath.Right(relPath.GetLength() - differIndex);

	return retStr;
}

void CProjectDoc::OnProjectProperties() 
{
	CTreeCtrl&		tc = m_currentView->GetTreeCtrl();
	CProjectItem*	tmpItem;

	switch (m_currentView->m_treeSelectionFlag) {
	case TSF_ANNOUNCE_FILE :
		tmpItem = GetAnnouncement(tc.GetSelectedItem());
		break;
	case TSF_SCRIPT_FILE :
		tmpItem = &m_script;
		break;
	case TSF_PHONE_BOOK_FILE :
		tmpItem = &m_phoneBook;
		break;
	}

	CProjItemPropDlg	myDlg;

	myDlg.SetDlgData(tmpItem->GetItemTitle(), tmpItem->GetItemPath());
	myDlg.DoModal();
}

void CProjectDoc::OnUpdateProjectProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_currentView->m_treeSelectionFlag == TSF_ANNOUNCE_FILE) ||
					(m_currentView->m_treeSelectionFlag == TSF_SCRIPT_FILE) ||
					(m_currentView->m_treeSelectionFlag == TSF_PHONE_BOOK_FILE) );
}

void CProjectDoc::OnProjectSimulateProject() 
{
	m_simulator = (CScriptSimulatorDlg*) new CScriptSimulatorDlg();

	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();
	CMainFrame*		myMF = (CMainFrame*) myApp->m_pMainWnd;


	m_simulator->Create(IDD_SCRIPT_SIMULATOR, myApp->m_pMainWnd);
	m_simulator->ShowWindow(SW_SHOW);
	
	CRect	dlgR;
	m_simulator->GetWindowRect(&dlgR);

	CRect	wndR = myMF->GetEmptyClientRect();

	dlgR.OffsetRect(wndR.Width()-dlgR.Width()-4, wndR.top);
	m_simulator->MoveWindow(dlgR, TRUE);

	
	//m_simulator->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);

	if (m_scriptDoc) m_simulator->SetScriptDoc(m_scriptDoc);
	else m_simulator->EnableOnSwitch(FALSE);
	m_simulator->SetProjectDoc(this);
}

BOOL CProjectDoc::IsSimulatorRunning( void )
{
	if (m_simulator) {
		return m_simulator->IsSimulatorRunning();
	}
	return FALSE;
}

CScriptObject*	CProjectDoc::GetCurrnetSimulatorObject( void )
{
	if (m_simulator) {
		return m_simulator->GetCurrentSimObject();
	}
	return NULL;
}

BOOL CProjectDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if (m_scriptDoc) {
		if (!m_scriptDoc->CanCloseFrame(pFrame)) return FALSE;
		m_scriptDoc->OnCloseDocument();
	}

	if (m_simulator) m_simulator->DestroyWindow();
	m_simulator = NULL;
	
	return CDocument::CanCloseFrame(pFrame);
}

void CProjectDoc::InvalidateScriptDoc( void )
{
	m_scriptDoc = NULL;
	if (m_simulator) {
		m_simulator->InvalidateScriptDoc();
		m_simulator->EnableOnSwitch(FALSE);
	}
}

void CProjectDoc::InvalidateSimulatorDoc( void )
{
	m_simulator = NULL;
}

void CProjectDoc::OnCloseDocument() 
{
	if (m_simulator) {
		m_simulator->InvalidateProjectDoc();
	}
	m_simulator = NULL;
	if (m_scriptDoc) {
		m_scriptDoc->InvalidateProjectDoc();
	}
	m_scriptDoc = NULL;

	
	CDocument::OnCloseDocument();
}

































IMPLEMENT_SERIAL( CProjectItem, CObject, 2 )

CProjectItem::CProjectItem()
{
	m_path = "";
	m_treeViewItem = NULL;
}

CProjectItem::CProjectItem(CString path, CString title)
{
	m_path = path;
	m_title = title;
	m_treeViewItem = NULL;
}

void CProjectItem::Serialize(CArchive &ar)
{
	if( ar.IsStoring( ) ) {
		ar << m_path;
		ar << m_title;
	} else {
		ar >> m_path;
		ar >> m_title;
	}
}







