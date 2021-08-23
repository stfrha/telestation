// TeleStation.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TeleStation.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "AnswerScriptDoc.h"
#include "AnswerScriptView.h"

#include "ProjectDoc.h"
#include "ProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBitmap	happySun;
CBitmap	waitCall;
CBitmap	liftPhone;
CBitmap	dropPhone;
CBitmap	playFile;
CBitmap	askQuestion;
CBitmap	promptCode;
CBitmap	recMsg;
CBitmap	playAll;
CBitmap	eraseAll;
CBitmap	answerOn;
CBitmap	answerOff;
CBitmap	label;

CStringArray	TransTypeStrings;



/////////////////////////////////////////////////////////////////////////////
// CTeleStationApp

BEGIN_MESSAGE_MAP(CTeleStationApp, CWinApp)
	//{{AFX_MSG_MAP(CTeleStationApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeleStationApp construction

CTeleStationApp::CTeleStationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTeleStationApp object

CTeleStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTeleStationApp initialization

BOOL CTeleStationApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	happySun.LoadBitmap(IDB_HAPPY_SUN_BM);
	dropPhone.LoadBitmap(IDB_SCRIPT_DROP_PHONE);
	liftPhone.LoadBitmap(IDB_SCRIPT_LIFT_PHONE);
	waitCall.LoadBitmap(IDB_SCRIPT_WAIT_RING);
	playFile.LoadBitmap(IDB_SCRIPT_PLAY_FILE);
	askQuestion.LoadBitmap(IDB_SCRIPT_ASK_QUESTION);
	promptCode.LoadBitmap(IDB_SCRIPT_PROMPT_CODE);
	recMsg.LoadBitmap(IDB_SCRIPT_REC_MSG);
	playAll.LoadBitmap(IDB_SCRIPT_PLAY_ALL);
	eraseAll.LoadBitmap(IDB_SCRIPT_ERASE_ALL);
	answerOn.LoadBitmap(IDB_SCRIPT_ANS_ON);
	answerOff.LoadBitmap(IDB_SCRIPT_ANS_OFF);
	label.LoadBitmap(IDB_SCRIPT_LABEL);

//	TransTypeStrings.SetSize(NUM_TRANS_TYPE);
	TransTypeStrings.Add("Done");
	TransTypeStrings.Add("DTMF 0");
	TransTypeStrings.Add("DTMF 1");
	TransTypeStrings.Add("DTMF 2");
	TransTypeStrings.Add("DTMF 3");
	TransTypeStrings.Add("DTMF 4");
	TransTypeStrings.Add("DTMF 5");
	TransTypeStrings.Add("DTMF 6");
	TransTypeStrings.Add("DTMF 7");
	TransTypeStrings.Add("DTMF 8");
	TransTypeStrings.Add("DTMF 9");
	TransTypeStrings.Add("DTMF Star");
	TransTypeStrings.Add("DTMF Pound");
	TransTypeStrings.Add("Other DTMF");
	TransTypeStrings.Add("Hangup");
	TransTypeStrings.Add("Timeout");
	TransTypeStrings.Add("Correct Code");
	TransTypeStrings.Add("Wrong Code");
	TransTypeStrings.Add("OK signals");

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.


//	CMultiDocTemplate* pDocTemplate;

	m_projectTemplate = new CMultiDocTemplate(
		IDR_PROJECTTYPE,
		RUNTIME_CLASS(CProjectDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CProjectView));
	AddDocTemplate(m_projectTemplate);

	m_scriptTemplate = new CMultiDocTemplate(
		IDR_ANSWERSCRIPTTYPE,
		RUNTIME_CLASS(CAnswerScriptDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAnswerScriptView));
	AddDocTemplate(m_scriptTemplate);


	//Någonstans måste m_scriptTemplate och m_projectTemplate delete:as!!!!!!!!!!!!!
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//Nedan skall göra så att det inte skapas något nytt dokument
	//vid uppstart
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTeleStationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTeleStationApp message handlers

CProjectDoc*	CTeleStationApp::GetCurrentProjDoc( void )
{
	POSITION pos = m_projectTemplate->GetFirstDocPosition();
	if (pos) {
		return (CProjectDoc*) m_projectTemplate->GetNextDoc(pos);
	}
	return NULL;
}
