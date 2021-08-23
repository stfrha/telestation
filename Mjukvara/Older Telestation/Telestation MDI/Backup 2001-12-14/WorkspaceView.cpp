// WorkspaceView.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "WorkspaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView

IMPLEMENT_DYNCREATE(CWorkspaceView, CTreeView)

CWorkspaceView::CWorkspaceView()
{
}

CWorkspaceView::~CWorkspaceView()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceView, CTreeView)
	//{{AFX_MSG_MAP(CWorkspaceView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView drawing

void CWorkspaceView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView diagnostics

#ifdef _DEBUG
void CWorkspaceView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CWorkspaceView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView message handlers
