// TSDocument.cpp : implementation file
//

#include "stdafx.h"
#include "telestation.h"
#include "TSDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTSDocument

IMPLEMENT_DYNCREATE(CTSDocument, CDocument)

CTSDocument::CTSDocument()
{
	m_projectDoc = NULL;
	m_openedByProject = FALSE;
}

BOOL CTSDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTSDocument::~CTSDocument()
{
}


BEGIN_MESSAGE_MAP(CTSDocument, CDocument)
	//{{AFX_MSG_MAP(CTSDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTSDocument diagnostics

#ifdef _DEBUG
void CTSDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTSDocument serialization

void CTSDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTSDocument commands

void CTSDocument::InvalidateProjectDoc( void )
{
	m_projectDoc = NULL;
}

BOOL CTSDocument::GetCurrentWndPlacement(WINDOWPLACEMENT* placement)
{
	CView*		tmpView;
	POSITION	pos;
	
	pos = GetFirstViewPosition();
	tmpView = GetNextView(pos);

	if (tmpView) {
		tmpView->GetParentFrame()->GetWindowPlacement(placement);
		return TRUE;
	} else {
		return FALSE;
	}
}
