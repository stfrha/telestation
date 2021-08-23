// ProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TeleStation.h"
#include "AnswerScriptDoc.h"
#include "PhoneBookDoc.h"
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
	m_phoneBookDoc = NULL;
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
	ON_COMMAND(ID_PROJECT_OPEN_PHONE_BOOK, OnProjectOpenPhoneBook)
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
	
	InitDocument();

	SetPathName(lpszPathName);

	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();

	LoadViewPlacement(NULL, 1);

	if (m_script.m_docVisible) {
		m_scriptDoc = (CAnswerScriptDoc*) myApp->OpenDocumentFile(m_script.GetItemPath());
	}

	if (m_phoneBook.m_docVisible) {
		m_phoneBookDoc = (CPhoneBookDoc*) myApp->OpenDocumentFile(m_phoneBook.GetItemPath());
	}

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
	LoadViewPlacement(m_currentView->GetParentFrame(), 1);
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

	m_script.m_openedByProject = TRUE;

	m_scriptDoc = (CAnswerScriptDoc*) myApp->OpenDocumentFile(m_script.GetItemPath());
	//Om scriptDoc kunde öppnas med raden ovan, så har det hittat detta ProjectDoc och
	//kört RegisterDocumentOpen.
	if (!m_scriptDoc) {
		m_scriptDoc = (CAnswerScriptDoc*) myApp->m_scriptTemplate->OpenDocumentFile(NULL, TRUE);
		if (m_scriptDoc) {
			//Eftersom scriptDoc inte kunde öppnas har ett nytt skapats. Detta har inget namn
			//Därför måste registrering göras "rått" nedan:
			m_scriptDoc->SetPathName(m_script.GetItemPath());
			m_scriptDoc->OnSaveDocument(m_script.GetItemPath());
			m_scriptDoc->SetProjectDoc(this);
			RegisterDocumentOpen(m_scriptDoc, 2);
		} else {
			m_script.m_openedByProject = FALSE;
			return;
		}
	}

//Nedanstående skall utföras av resp Doc.
//	m_scriptDoc->SetProjectDoc(this);

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
	POSITION pos;

	if (m_scriptDoc) {
		pos = m_scriptDoc->GetFirstViewPosition();
		if (pos) {
			if (!m_scriptDoc->CanCloseFrame(m_scriptDoc->GetNextView(pos)->GetParentFrame())) return FALSE;
		}
	}

	if (m_phoneBookDoc) {
		pos = m_phoneBookDoc->GetFirstViewPosition();
		if (pos) {
			if (!m_phoneBookDoc->CanCloseFrame(m_phoneBookDoc->GetNextView(pos)->GetParentFrame())) return FALSE;
		}
	}

	SaveViewPlacement(pFrame, 1);

	if (m_scriptDoc) {
		m_scriptDoc->OnCloseDocument();
	}

	if (m_phoneBookDoc) {
		m_phoneBookDoc->OnCloseDocument();
	}

	if (m_simulator) m_simulator->DestroyWindow();
	m_simulator = NULL;
	
	return CDocument::CanCloseFrame(pFrame);
}

void CProjectDoc::InvalidateScriptDoc( void )
{
	m_scriptDoc = NULL;
	m_script.m_docVisible = FALSE;
	if (m_simulator) {
		m_simulator->InvalidateScriptDoc();
		m_simulator->EnableOnSwitch(FALSE);
	}
}

void CProjectDoc::InvalidatePhoneBookDoc( void )
{
	m_phoneBookDoc = NULL;
	m_phoneBook.m_docVisible = FALSE;
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

	if (m_phoneBookDoc) {
		m_phoneBookDoc->InvalidateProjectDoc();
	}
	m_phoneBookDoc = NULL;
	
	CDocument::OnCloseDocument();
}

void CProjectDoc::OnProjectOpenPhoneBook() 
{
	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();

	m_phoneBook.m_openedByProject = TRUE;
	
	m_phoneBookDoc = (CPhoneBookDoc*) myApp->OpenDocumentFile(m_phoneBook.GetItemPath());
	//Om phoneBookDoc kunde öppnas med raden ovan, så har det hittat detta ProjectDoc och
	//kört RegisterDocumentOpen.
	if (!m_phoneBookDoc) {
		m_phoneBookDoc = (CPhoneBookDoc*) myApp->m_phoneBookTemplate->OpenDocumentFile(NULL, TRUE);
		if (m_phoneBookDoc) {
			//Eftersom phoneBookDoc inte kunde öppnas har ett nytt skapats. Detta har inget namn
			//Därför måste registrering göras "rått" nedan:
			m_phoneBookDoc->SetPathName(m_phoneBook.GetItemPath());
			m_phoneBookDoc->OnSaveDocument(m_phoneBook.GetItemPath());
			m_phoneBookDoc->SetProjectDoc(this);
			RegisterDocumentOpen(m_phoneBookDoc, 3);
		} else {
			m_phoneBook.m_openedByProject = FALSE;
			return;
		}
	}
}

BOOL CProjectDoc::GetDefaultPlacement(CTSDocument* caller, WINDOWPLACEMENT* placement)
{
	return FALSE;
/*
	if (caller == m_scriptDoc) {
		placement = &m_script.m_wndPlacement;
		return m_script.m_wndPlacementValid;
	} else if (caller == m_phoneBookDoc) {
		placement = &m_phoneBook.m_wndPlacement;
		return m_script.m_wndPlacementValid;
	} else return FALSE;
*/
}

void CProjectDoc::SaveViewPlacement(CFrameWnd* frame, int docType)
/*
	docType: 1=CProjectDoc, 2=CAnswerScriptDoc, 3=CPhoneBookDoc
*/
{
	CString			wndplcFile;
	FILE*			fp;
	WINDOWPLACEMENT	wp;

	wp.length = sizeof(WINDOWPLACEMENT);

/*	Nedan skall stå innan kallen till SaveViewPlacement i CProjectDoc
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos)->GetParentFrame()->GetWindowPlacement(&wp);
	*/
	frame->GetWindowPlacement(&wp);
	wndplcFile = GetPathName();
	switch(docType) {
	case 1:
		wndplcFile += ".ppf";
		break;
	case 2:
		wndplcFile += ".spf";
		break;
	case 3:
		wndplcFile += ".bpf";
		break;
	default :
		return;
	}
	fp = fopen(wndplcFile, "wt");
	if (fp) {
		fprintf(fp, "Version2\n");		//OBS Inga mellanslag!!!
		fprintf(fp, "%d\n", wp.flags);
		fprintf(fp, "%d\n", wp.showCmd);
		fprintf(fp, "%d\n", wp.ptMinPosition.x);
		fprintf(fp, "%d\n", wp.ptMinPosition.y);
		fprintf(fp, "%d\n", wp.ptMaxPosition.x);
		fprintf(fp, "%d\n", wp.ptMaxPosition.y);
		fprintf(fp, "%d\n", wp.rcNormalPosition.left);
		fprintf(fp, "%d\n", wp.rcNormalPosition.top);
		fprintf(fp, "%d\n", wp.rcNormalPosition.right);
		fprintf(fp, "%d\n", wp.rcNormalPosition.bottom);
		if (docType == 1) {
			fprintf(fp, "%d\n", m_script.m_docVisible);
			fprintf(fp, "%d\n", m_phoneBook.m_docVisible);
		}
		fclose(fp);
	}
}

void CProjectDoc::LoadViewPlacement(CFrameWnd* frame, int docType)
{
	CString			wndplcFile;
	char			verString[100];
	FILE*			fp;
	WINDOWPLACEMENT	wp;

	wp.length = sizeof(WINDOWPLACEMENT);

	wndplcFile = GetPathName();
	switch(docType) {
	case 1:
		wndplcFile += ".ppf";
		break;
	case 2:
		wndplcFile += ".spf";
		break;
	case 3:
		wndplcFile += ".bpf";
		break;
	default :
		return;
	}
	fp = fopen(wndplcFile, "rt");
	if (fp) {
		fscanf(fp, "%s\n", verString);
		if (!strcmp(verString, "Version2")) {		//OBS Inga mellanslag!!!
			fscanf(fp, "%d\n", &(wp.flags));
			fscanf(fp, "%d\n", &wp.showCmd);
			fscanf(fp, "%d\n", &(wp.ptMinPosition.x));
			fscanf(fp, "%d\n", &(wp.ptMinPosition.y));
			fscanf(fp, "%d\n", &(wp.ptMaxPosition.x));
			fscanf(fp, "%d\n", &(wp.ptMaxPosition.y));
			fscanf(fp, "%d\n", &(wp.rcNormalPosition.left));
			fscanf(fp, "%d\n", &(wp.rcNormalPosition.top));
			fscanf(fp, "%d\n", &(wp.rcNormalPosition.right));
			fscanf(fp, "%d\n", &(wp.rcNormalPosition.bottom));
			if (frame) frame->SetWindowPlacement(&wp);
			if (docType == 1) {
				fscanf(fp, "%d\n", &m_script.m_docVisible);
				fscanf(fp, "%d\n", &m_phoneBook.m_docVisible);
			}
		} else {
			//Skapa default placering
		}
		fclose(fp);
	} else {
		//Skapa default placering
	}
}

void CProjectDoc::RegisterDocumentOpen(CTSDocument* doc, int docType)
{
	switch (docType) {
	case 2 :
		m_scriptDoc = (CAnswerScriptDoc*) doc;
		m_script.m_docVisible = TRUE;
		break;
	case 3:
		m_phoneBookDoc = (CPhoneBookDoc*) doc;
		m_phoneBook.m_docVisible = TRUE;
		break;
	}
}

CString CProjectDoc::GetItemPath(int docType)
{
	switch (docType) {
	case 2 :
		return m_script.GetItemPath();
	case 3:
		return m_phoneBook.GetItemPath();
	}
	return "";
}

CString* CProjectDoc::GetPhoneBookList( void )
{
	if (!m_phoneBookDoc) {
		CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();
		m_phoneBookDoc = (CPhoneBookDoc*) myApp->OpenDocumentFile(m_phoneBook.GetItemPath());
	}
	if (m_phoneBookDoc) {
		return m_phoneBookDoc->m_categoryList;
	} else {
		return NULL;
	}

}

void CProjectDoc::GenerateProjectFiles(CFileGenDlg* dlg)
{
	if (m_scriptDoc) {
		m_scriptDoc->GenResetObjects();
		if (m_scriptDoc->DRC(dlg)) {
			m_scriptDoc->GenerateScriptFile(dlg);
		}
	}

	if (m_phoneBookDoc) {
		m_phoneBookDoc->GeneratePhoneBook(dlg);
	}
	GenerateWaws(dlg);
}

void CProjectDoc::GenerateWaws(CFileGenDlg* dlg)
{
	CString			msg;

	POSITION pos = m_announcementList.GetHeadPosition();
	while (pos != NULL) {
		ConvertToWAW(dlg, m_announcementList.GetNext(pos));
	}
}

void CProjectDoc::ConvertToWAW(CFileGenDlg* dlg, CProjectItem* tmpPI)
{
	CString		drive, path, name, ext, wawPath, msg;
	FILE*		ifp;
	FILE*		ofp;
	char		srstr[5];
	double		fts = 11059200.0/12.0/(256.0-132.0);
	double		npc = fts/(8000.0-fts);
	double		c;
	long		fileLen;
	long		i;

	_splitpath(tmpPI->GetItemPath(), drive.GetBuffer(_MAX_DRIVE), path.GetBuffer(_MAX_DIR), name.GetBuffer(_MAX_FNAME), ext.GetBuffer(_MAX_EXT));
	drive.ReleaseBuffer(-1);
	path.ReleaseBuffer(-1);
	name.ReleaseBuffer(-1);
	ext.ReleaseBuffer(-1);
	wawPath = drive + path + name + ".waw";
	ifp = fopen(wawPath, "rb");
	if (ifp) {
		fclose(ifp);
	} else {
		msg.Format("Converting %s...", tmpPI->GetItemTitle());
		dlg->InsertString(msg);
		ifp = fopen(tmpPI->GetItemPath(), "rb");
		if (!ifp) {
			MessageBox(NULL, "Could not open WAV (input) file", "Error", MB_OK);
			return;
		}
		ofp = fopen(wawPath, "wb");
		if (!ofp) {
			MessageBox(NULL, "Could not open WAW (output) file", "Error", MB_OK);
			return;
		}

		srstr[0] = fgetc(ifp);
		srstr[1] = fgetc(ifp);
		srstr[2] = fgetc(ifp);
		srstr[3] = fgetc(ifp);
		srstr[4] = 0;
		while (strcmp(srstr, "data"))  {
			srstr[0] = srstr[1];
			srstr[1] = srstr[2];
			srstr[2] = srstr[3];
			srstr[3] = fgetc(ifp);
		}

		fileLen = fgetc(ifp);
		fileLen += fgetc(ifp) << 8;
		fileLen += fgetc(ifp) << 16;
		fileLen += fgetc(ifp) << 24;

		c = 0.0;
		for (i=0 ; i<fileLen ; i++) {
			fputc(fgetc(ifp), ofp);
			c++;
			if (c>npc) {
				c = c - npc;
				fgetc(ifp);
			}
		}
		fclose(ofp);
		dlg->InsertString("Conversion done");
	}
}















IMPLEMENT_SERIAL( CProjectItem, CObject, 3)

CProjectItem::CProjectItem()
{
	m_path = "";
	m_treeViewItem = NULL;
//	m_wndPlacement.length = sizeof(WINDOWPLACEMENT);
//	m_wndPlacementValid = 0;
	m_openedByProject = FALSE;
	m_docVisible = FALSE;
	m_docVisibleAtProjectClose = FALSE;
}

CProjectItem::CProjectItem(CString path, CString title)
{
	m_path = path;
	m_title = title;
	m_treeViewItem = NULL;
	m_openedByProject = FALSE;
	m_docVisible = FALSE;
	m_docVisibleAtProjectClose = FALSE;
}

void CProjectItem::Serialize(CArchive &ar)
{
	if( ar.IsStoring( ) ) {
		ar << m_path;
		ar << m_title;
		ar << m_docVisibleAtProjectClose;

	} else {
		ar >> m_path;
		ar >> m_title;
		ar >> m_docVisibleAtProjectClose;
	}
}








