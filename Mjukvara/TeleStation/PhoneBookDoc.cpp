// CListTestDoc.cpp : implementation of the CPhoneBookDoc class
//

#include "stdafx.h"
#include "TeleStation.h"
#include "TSDocument.h"
#include "PhoneBookDoc.h"
#include "EditCategoriesDlg.h"
#include "FileGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc

IMPLEMENT_DYNCREATE(CPhoneBookDoc, CTSDocument)

BEGIN_MESSAGE_MAP(CPhoneBookDoc, CTSDocument)
	//{{AFX_MSG_MAP(CPhoneBookDoc)
	ON_COMMAND(ID_PHONEBOOK_EDIT_CATEGORIES, OnPhonebookEditCategories)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc construction/destruction

CPhoneBookDoc::CPhoneBookDoc()
{
}

CPhoneBookDoc::~CPhoneBookDoc()
{
}

BOOL CPhoneBookDoc::OnNewDocument()
{
	if (!CTSDocument::OnNewDocument())
		return FALSE;


	int	i;

	for (i=0 ; i<256 ; i++) {
		m_categoryList[i] = "";
	}

	return TRUE;
}


BOOL CPhoneBookDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CTSDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_projectDoc = NULL;	//Förutsätt att tillhörande dokument inte är öppet.

	CTeleStationApp* app = (CTeleStationApp*) AfxGetApp();
	CProjectDoc* pDoc = app->GetCurrentProjDoc();

	if (pDoc) {
		CString tmpStr = lpszPathName;
		tmpStr.MakeUpper();
		CString tmpStr2 = pDoc->GetItemPath(3);
		tmpStr2.MakeUpper();
		if (tmpStr == tmpStr2) {
			m_projectDoc = pDoc;
			pDoc->RegisterDocumentOpen(this, 3);
		}
	}
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc serialization

void CPhoneBookDoc::Serialize(CArchive& ar)
{
	int	i;

	m_phoneBookList.Serialize( ar );

	if (ar.IsStoring()) {
		for (i=0 ; i<256 ; i++) {
			ar << m_categoryList[i];
		}
	} else {
		for (i=0 ; i<256 ; i++) {
			ar >> m_categoryList[i];
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc diagnostics

#ifdef _DEBUG
void CPhoneBookDoc::AssertValid() const
{
	CTSDocument::AssertValid();
}

void CPhoneBookDoc::Dump(CDumpContext& dc) const
{
	CTSDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDoc commands

CPBEntry* CPhoneBookDoc::FindEntry(CString number)
{
	POSITION	pos;
	CPBEntry*	tmpEntry;

	pos = m_phoneBookList.GetHeadPosition();

	while (pos) {
		tmpEntry = m_phoneBookList.GetNext(pos);
		if (tmpEntry->m_number == number) return tmpEntry;
	}
	return NULL;
}


BOOL CPhoneBookDoc::DeleteEntry(CPBEntry* deleteMe)
{
	POSITION	pos;

	pos = m_phoneBookList.Find(deleteMe);
	if (pos) {
		m_phoneBookList.RemoveAt(pos);
		delete deleteMe;
		return TRUE;
	}
	return FALSE;
}

void CPhoneBookDoc::OnPhonebookEditCategories() 
{
	CEditCategoriesDlg	catDlg;

	catDlg.m_listMirror = m_categoryList;

	catDlg.DoModal();

	if (catDlg.m_isModified) {
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}

}

void CPhoneBookDoc::OnCloseDocument() 
{
	if (m_projectDoc) m_projectDoc->InvalidatePhoneBookDoc();
	
	CTSDocument::OnCloseDocument();
}


BOOL CPhoneBookDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	BOOL	ans;

	ans = CTSDocument::CanCloseFrame(pFrame);
	
	if (ans) {
		if (m_projectDoc) m_projectDoc->SaveViewPlacement(pFrame, 3);
	
	}

	return ans;
}

void CPhoneBookDoc::GeneratePhoneBook(CFileGenDlg* dlg)
{
	FILE*		fp;
	char		path_buffer[_MAX_PATH];
	char		drive[_MAX_DRIVE];
	char		dir[_MAX_DIR];
	char		fname[_MAX_FNAME];
	char		ext[_MAX_EXT];
	CString		cdrive, cdir, cfname;
	CString		pbPath;
	CString		tmpStr;
	CPBEntry*	tmpEntry;
	int			count, i;


	if (m_phoneBookList.IsEmpty()) {
		dlg->InsertString("WARNING: Phone book empty, Phone book not generated");
		return;
	}

	strcpy(path_buffer, GetPathName());

	_splitpath(path_buffer, drive, dir, fname, ext );
	cdrive = drive;
	cdir = dir;
	cfname = fname;
	pbPath = cdrive + cdir + "PB.tpb";

	fp = fopen(pbPath, "wb");

	if (fp) {
		dlg->InsertString("Generating Phone book, please wait...");

		i = m_phoneBookList.GetCount();

		fputc((i >> 8) & 0xFF, fp);
		fputc(i & 0xFF, fp);

		POSITION pos = m_phoneBookList.GetHeadPosition();
		while (pos) {
			tmpEntry = m_phoneBookList.GetNext(pos);
			fprintf(fp, "%s", tmpEntry->m_number);
			fputc(0, fp);
			tmpStr = tmpEntry->m_name.Left(32 - tmpEntry->m_number.GetLength() - 3);
			fprintf(fp, "%s", tmpStr);
			fputc(0, fp);
			fputc(tmpEntry->m_category & 0xFF, fp);
			count = tmpEntry->m_number.GetLength() + tmpStr.GetLength() + 3;
			for (i=count ; i<32 ; i++) {
				fputc(0, fp);
			}

		}
		dlg->InsertString("Phone book succefully generated");
		fclose(fp);
	} else {
		dlg->InsertString("ERROR: Could not open output file");
	}
}


















/////////////////////////////////////////////////////////////////////////////
// Class CPBEntry


IMPLEMENT_SERIAL( CPBEntry, CObject, 1 )


void	CPBEntry::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) ) {
		ar << m_name;
		ar << m_number;
		ar << m_category;
	} else {
		ar >> m_name;
		ar >> m_number;
		ar >> m_category;
	}
}
