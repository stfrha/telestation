// WaveFormaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WaveFormater.h"
#include "WaveFormaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CWaveFormaterDlg dialog

CWaveFormaterDlg::CWaveFormaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveFormaterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaveFormaterDlg)
	m_formatTag = 0;
	m_nChannels = 0;
	m_samplePerSec = 0;
	m_byteLen = 0;
	m_timeLen = 0;
	m_formatLen = 0;
	m_bitPerSample = 0;
	m_avgBytePerSec = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWaveFormaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveFormaterDlg)
	DDX_Text(pDX, IDC_FORMAT_TAG, m_formatTag);
	DDX_Text(pDX, IDC_N_CHANNELS, m_nChannels);
	DDX_Text(pDX, IDC_SAMPLE_PER_SEC, m_samplePerSec);
	DDX_Text(pDX, IDC_BYTE_LEN, m_byteLen);
	DDX_Text(pDX, IDC_TIME_LEN, m_timeLen);
	DDX_Text(pDX, IDC_FORMAT_LEN, m_formatLen);
	DDX_Text(pDX, IDC_BIT_PER_SAMPLE, m_bitPerSample);
	DDX_Text(pDX, IDC_AVG_BYTES_PER_SEC, m_avgBytePerSec);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWaveFormaterDlg, CDialog)
	//{{AFX_MSG_MAP(CWaveFormaterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECT_WAVE, OnSelectWave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveFormaterDlg message handlers

BOOL CWaveFormaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWaveFormaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWaveFormaterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWaveFormaterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWaveFormaterDlg::OnSelectWave() 
{
	FILE*		fp;
	char		srstr[5];

	CFileDialog	MyFDlg(TRUE, "WAV", "*.WAV", OFN_HIDEREADONLY, "Standard PCM file (*.WAV) |*.WAV| All Files (*.*) |*.*||", NULL);

	if (MyFDlg.DoModal() == IDOK) {
		fp = fopen(MyFDlg.GetPathName(), "rb");
		if (fp) {

			srstr[0] = fgetc(fp);
			srstr[1] = fgetc(fp);
			srstr[2] = fgetc(fp);
			srstr[3] = fgetc(fp);
			srstr[4] = 0;
			while (strcmp(srstr, "fmt "))  {
				srstr[0] = srstr[1];
				srstr[1] = srstr[2];
				srstr[2] = srstr[3];
				srstr[3] = fgetc(fp);
			}
			
			m_formatLen = fgetc(fp);
			m_formatLen += fgetc(fp) << 8;
			m_formatLen += fgetc(fp) << 16;
			m_formatLen += fgetc(fp) << 24;

			m_formatTag = fgetc(fp);
			m_formatTag += fgetc(fp) << 8;

			m_nChannels = fgetc(fp);
			m_nChannels += fgetc(fp) << 8;

			m_samplePerSec = fgetc(fp);
			m_samplePerSec += fgetc(fp) << 8;
			if (m_formatLen >= 16) {
				m_samplePerSec += fgetc(fp) << 16;
				m_samplePerSec += fgetc(fp) << 24;
			}

			m_avgBytePerSec = fgetc(fp);
			m_avgBytePerSec += fgetc(fp) << 8;
			if (m_formatLen >= 16) {
				m_avgBytePerSec += fgetc(fp) << 16;
				m_avgBytePerSec += fgetc(fp) << 24;
			}


			m_bitPerSample = 8 * m_avgBytePerSec / (m_nChannels * m_samplePerSec);

			srstr[0] = fgetc(fp);
			srstr[1] = fgetc(fp);
			srstr[2] = fgetc(fp);
			srstr[3] = fgetc(fp);
			srstr[4] = 0;
			while (strcmp(srstr, "data"))  {
				srstr[0] = srstr[1];
				srstr[1] = srstr[2];
				srstr[2] = srstr[3];
				srstr[3] = fgetc(fp);
			}
			m_byteLen = fgetc(fp);
			m_byteLen += fgetc(fp) << 8;
			m_byteLen += fgetc(fp) << 16;
			m_byteLen += fgetc(fp) << 24;

			m_timeLen =  1000 * m_byteLen / m_avgBytePerSec;

			UpdateData(FALSE);

			fclose(fp);

		} else {
			MessageBox("Could not open file", "Error");
		}
	}
}
