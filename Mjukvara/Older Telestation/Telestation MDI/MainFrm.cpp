// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TeleStation.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_SCRIPT_TOOLBAR, OnViewScriptToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCRIPT_TOOLBAR, OnUpdateViewScriptToolbar)
	ON_COMMAND(ID_PROJECT_SIMULATE_PROJECT, OnProjectSimulateProject)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SIMULATE_PROJECT, OnUpdateProjectSimulateProject)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_scriptToolBox.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_scriptToolBox.LoadToolBar(IDR_SCRIPT_TOOLS))
	{
		TRACE0("Failed to create script toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_scriptToolBox.SetWindowText(_T("Script tools"));


/*	
	int nCount = m_scriptToolBox.GetToolBarCtrl().GetButtonCount();

	for(int i = 0; i < nCount; i++)
	{
		UINT nStyle = m_scriptToolBox.GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % 3) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;

		m_scriptToolBox.SetButtonStyle(i, nStyle);
	}
*/

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_scriptToolBox.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_scriptToolBox);

//	FloatControlBar(&m_scriptToolBox, CPoint(20, 20), CBRS_ALIGN_RIGHT);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnViewScriptToolbar() 
{
	ShowControlBar(&m_scriptToolBox, !m_scriptToolBox.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateViewScriptToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_scriptToolBox.IsWindowVisible());
}

void CMainFrame::OnProjectSimulateProject() 
{
	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();

	CProjectDoc*	curProjDoc = myApp->GetCurrentProjDoc();

	if (curProjDoc) {
		curProjDoc->OnProjectSimulateProject();
	}
}

void CMainFrame::OnUpdateProjectSimulateProject(CCmdUI* pCmdUI) 
{
	CTeleStationApp*	myApp = (CTeleStationApp*) AfxGetApp();

	if (myApp->GetCurrentProjDoc()) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}


void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIFrameWnd::OnClose();
}

CRect CMainFrame::GetEmptyClientRect( void )
{
	CRect tr;

	GetClientRect(&tr);

	CRect toolRect;

	m_wndToolBar.GetWindowRect(&toolRect);

	if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_TOP) {
		tr.DeflateRect(0, toolRect.Height(), 0, 0);
	} else if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_RIGHT) {
		tr.DeflateRect(0, 0, toolRect.Width(), 0);
	} else if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_BOTTOM) {
		tr.DeflateRect(0, 0, 0, toolRect.Height());
	} else if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_LEFT) {
		tr.DeflateRect(toolRect.Width(), 0, 0, 0);
	}

	m_scriptToolBox.GetWindowRect(&toolRect);

	if (m_scriptToolBox.GetBarStyle() & CBRS_ALIGN_TOP) {
		tr.DeflateRect(0, toolRect.Height(), 0, 0);
	} else if (m_scriptToolBox.GetBarStyle() & CBRS_ALIGN_RIGHT) {
		tr.DeflateRect(0, 0, toolRect.Width(), 0);
	} else if (m_scriptToolBox.GetBarStyle() & CBRS_ALIGN_BOTTOM) {
		tr.DeflateRect(0, 0, 0, toolRect.Height());
	} else if (m_scriptToolBox.GetBarStyle() & CBRS_ALIGN_LEFT) {
		tr.DeflateRect(toolRect.Width(), 0, 0, 0);
	}

	return tr;
}
