// WorkspaceDoc.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "WorkspaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc

IMPLEMENT_DYNCREATE(CWorkspaceDoc, CDocument)

CWorkspaceDoc::CWorkspaceDoc()
{
}

BOOL CWorkspaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CWorkspaceDoc::~CWorkspaceDoc()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceDoc, CDocument)
	//{{AFX_MSG_MAP(CWorkspaceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc diagnostics

#ifdef _DEBUG
void CWorkspaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorkspaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc serialization

void CWorkspaceDoc::Serialize(CArchive& ar)
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
// CWorkspaceDoc commands
