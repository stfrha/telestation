// PlayFilePropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "PlayFilePropDlg.h"
#include "ProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayFilePropDlg dialog


CPlayFilePropDlg::CPlayFilePropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayFilePropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayFilePropDlg)
	//}}AFX_DATA_INIT

	m_projectDoc = NULL;
	m_selectedTitle = "";
}


void CPlayFilePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayFilePropDlg)
	DDX_Control(pDX, IDC_ANNOUNCEMENT, m_announcementsBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayFilePropDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayFilePropDlg)
	ON_BN_CLICKED(IDC_ADD_FILE, OnAddFile)
	ON_BN_CLICKED(IDC_PLAY_FILE, OnPlayFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayFilePropDlg message handlers

BOOL CPlayFilePropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_projectDoc) {
		
		POSITION pos = m_projectDoc->m_announcementList.GetHeadPosition();
		while (pos) {
			m_announcementsBox.AddString(m_projectDoc->m_announcementList.GetNext(pos)->GetItemTitle());
		}

	}

	if (!m_selectedTitle.IsEmpty()) {
		m_announcementsBox.SelectString(0, m_selectedTitle);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPlayFilePropDlg::OnAddFile() 
{
	if (m_projectDoc) {
		m_projectDoc->AddAnnouncement();
	}

	m_announcementsBox.ResetContent();
	
	POSITION pos = m_projectDoc->m_announcementList.GetHeadPosition();
	while (pos) {
		m_announcementsBox.AddString(m_projectDoc->m_announcementList.GetNext(pos)->GetItemTitle());
	}
}


void CPlayFilePropDlg::OnOK() 
{
	if (m_announcementsBox.GetCurSel() != CB_ERR) {
		m_announcementsBox.GetLBText(m_announcementsBox.GetCurSel(), m_selectedTitle);
	} else {
		m_selectedTitle = "";
	}
	
	CDialog::OnOK();
}

void CPlayFilePropDlg::OnPlayFile() 
{
	if (m_projectDoc) {

		CString	tmp;

		if (m_announcementsBox.GetCurSel() != CB_ERR) {

			m_announcementsBox.GetLBText(m_announcementsBox.GetCurSel(), tmp);

			m_projectDoc->PlayAnnouncement(tmp);
		}
	}
}
