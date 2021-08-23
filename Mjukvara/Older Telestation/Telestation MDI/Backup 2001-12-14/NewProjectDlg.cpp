// NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "NewProjectDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog


CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectDlg)
	m_newProjectScript = _T("Untiteled.srf");
	m_newProjectPhoneBook = _T("");
	m_location = _T("");
	//}}AFX_DATA_INIT
}


void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectDlg)
	DDX_Control(pDX, IDC_PROJECT_NAME, m_projectNameEdit);
	DDX_Text(pDX, IDC_NEW_PROJECT_SCRIPT, m_newProjectScript);
	DDX_Text(pDX, IDC_NEW_PROJECT_PHONE_BOOK, m_newProjectPhoneBook);
	DDX_Text(pDX, IDC_LOCATION, m_location);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CNewProjectDlg)
	ON_BN_CLICKED(IDC_BRWS_SCRIPT, OnBrwsScript)
	ON_BN_CLICKED(IDC_BRWS_PHONE_BOOK, OnBrwsPhoneBook)
	ON_EN_CHANGE(IDC_PROJECT_NAME, OnChangeProjectName)
	ON_BN_CLICKED(IDC_BRWS_LOCATION, OnBrwsLocation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg message handlers

BOOL CNewProjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char	tmpBuf[MAX_PATH];

	_getcwd(tmpBuf, MAX_PATH);

	m_selectedLocation = tmpBuf;
	m_selectedLocation += "\\";

	m_location = m_selectedLocation;

	m_newProjectScript = ".srf";
	m_newProjectPhoneBook = ".tpb";
	UpdateData(FALSE);

	m_scriptPath = "";
	m_phoneBookPath = "";


	return TRUE;  // return TRUE unless you set the focus to a control
}


void CNewProjectDlg::OnBrwsScript() 
{
	CFileDialog	psDlg(TRUE, "SRF", NULL, OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_NOCHANGEDIR |
				OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "Telestation Script Files (*.srf)|*.srf||",
				NULL);

	if (psDlg.DoModal() == IDOK) {
		m_scriptPath = psDlg.GetPathName();
		m_newProjectScript = psDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CNewProjectDlg::OnBrwsPhoneBook() 
{
	CFileDialog	psDlg(TRUE, "TPB", NULL, OFN_HIDEREADONLY | OFN_LONGNAMES |  OFN_NOCHANGEDIR |
				OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "Telestation Phone Book Files (*.tpb)|*.tpb||",
				NULL);

	if (psDlg.DoModal() == IDOK) {
		m_phoneBookPath = psDlg.GetPathName();
		m_newProjectPhoneBook = psDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CNewProjectDlg::OnChangeProjectName() 
{
	m_projectNameEdit.GetWindowText(m_projectTitle);

	m_location = m_selectedLocation + m_projectTitle;
	if (m_scriptPath == "") {
		m_newProjectScript = m_projectTitle + ".srf";
	}
	if (m_phoneBookPath == "") {
		m_newProjectPhoneBook = m_projectTitle + ".tpb";
	}
	UpdateData(FALSE);
}

void CNewProjectDlg::OnBrwsLocation() 
{
    LPMALLOC pMalloc;
    /* Gets the Shell's default allocator */
    if (::SHGetMalloc(&pMalloc) == NOERROR)
    {
        BROWSEINFO bi;
        char pszBuffer[MAX_PATH];
        LPITEMIDLIST pidl;
        // Get help on BROWSEINFO struct - it's got all the bit settings.
        bi.hwndOwner = GetSafeHwnd();
        bi.pidlRoot = NULL;
        bi.pszDisplayName = pszBuffer;
        bi.lpszTitle = _T("Select a Starting Directory");
        bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
        bi.lpfn = NULL;
        bi.lParam = 0;
        // This next call issues the dialog box.
        if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
        {
            if (::SHGetPathFromIDList(pidl, pszBuffer))
            { 
            // At this point pszBuffer contains the selected path */.
				m_selectedLocation = pszBuffer;
				m_selectedLocation += "\\";

				m_location = m_selectedLocation;

				UpdateData(FALSE);
            }
            // Free the PIDL allocated by SHBrowseForFolder.
            pMalloc->Free(pidl);
        }
        // Release the shell's allocator.
        pMalloc->Release();
    }
}

void CNewProjectDlg::OnOK() 
{
	CDialog::OnOK();

	_chdir(m_selectedLocation.GetBuffer(MAX_PATH));
	m_selectedLocation.ReleaseBuffer();
	_mkdir(m_projectTitle);
	_chdir(m_projectTitle);
	m_projectPath = m_location + "\\" + m_projectTitle + ".tsp";

	if (m_scriptPath == "") {
		m_scriptPath = m_location + "\\" + m_projectTitle + ".srf";
		m_scriptTitle = m_projectTitle;
	} else {
		_splitpath(m_scriptPath.GetBuffer(MAX_PATH), NULL, NULL, m_scriptTitle.GetBuffer(_MAX_FNAME), NULL);
		m_scriptPath.ReleaseBuffer();
		m_scriptTitle.ReleaseBuffer();
	}

	if (m_phoneBookPath == "") {
		m_phoneBookPath = m_location + "\\" + m_projectTitle + ".tpb";
		m_phoneBookTitle = m_projectTitle;
	} else {
		_splitpath(m_phoneBookPath.GetBuffer(MAX_PATH), NULL, NULL, m_phoneBookTitle.GetBuffer(_MAX_FNAME), NULL);
		m_phoneBookPath.ReleaseBuffer();
		m_phoneBookTitle.ReleaseBuffer();
	}
}
