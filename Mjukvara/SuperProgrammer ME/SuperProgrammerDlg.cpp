// SuperProgrammerDlg.cpp : implementation file
//

#include <afxtempl.h>

//#include <stdio.h>
#include "stdafx.h"
#include "SuperProgrammer.h"
#include "SuperProgrammerDlg.h"
#include "serialco.h"
#include "TeleFileSel.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperProgrammerDlg dialog

CSuperProgrammerDlg::CSuperProgrammerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperProgrammerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperProgrammerDlg)
	m_comPort = 0;
	m_filename = _T("");
	m_status = _T("FRHA welcomes you");
	m_activeTerm = FALSE;
	m_decSend = 0;
	m_charSend = _T("");
	m_hexSend = _T("00");
	m_hexMonitor = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSuperProgrammerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperProgrammerDlg)
	DDX_Control(pDX, IDC_DOWNLOAD2, m_download2);
	DDX_Control(pDX, IDC_MAIN_PROGRESS, m_mainProgress);
	DDX_Control(pDX, IDC_SNDZER, m_sendZero);
	DDX_Control(pDX, IDC_SNDESC, m_sendESC);
	DDX_Control(pDX, IDC_SEND, m_send);
	DDX_Control(pDX, IDC_CONNTEST, m_conTest);
	DDX_Control(pDX, IDC_DOWNLOAD, m_download);
	DDX_Control(pDX, IDC_UPLOAD, m_upload);
	DDX_Control(pDX, IDC_PROGRAM, m_program);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_CBIndex(pDX, IDC_COMPORT, m_comPort);
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Check(pDX, IDC_ACTIVETERM, m_activeTerm);
	DDX_Text(pDX, IDC_DECSEND, m_decSend);
	DDV_MinMaxInt(pDX, m_decSend, 0, 255);
	DDX_Text(pDX, IDC_CHARSEND, m_charSend);
	DDV_MaxChars(pDX, m_charSend, 1);
	DDX_Text(pDX, IDC_HEXSEND, m_hexSend);
	DDV_MaxChars(pDX, m_hexSend, 2);
	DDX_Check(pDX, IDC_HEX_MONITOR, m_hexMonitor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSuperProgrammerDlg, CDialog)
	//{{AFX_MSG_MAP(CSuperProgrammerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PROGRAM, OnProgram)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_CONNTEST, OnConntest)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_CBN_SELCHANGE(IDC_COMPORT, OnSelchangeComport)
	ON_BN_CLICKED(IDC_UPLOAD, OnUpload)
	ON_BN_CLICKED(IDC_TO_WAV, OnToWav)
	ON_BN_CLICKED(IDC_TO_WAW, OnToWaw)
	ON_BN_CLICKED(IDC_ACTIVETERM, OnActiveterm)
	ON_EN_KILLFOCUS(IDC_CHARSEND, OnKillfocusCharsend)
	ON_EN_KILLFOCUS(IDC_DECSEND, OnKillfocusDecsend)
	ON_EN_KILLFOCUS(IDC_HEXSEND, OnKillfocusHexsend)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_SNDESC, OnSndesc)
	ON_BN_CLICKED(IDC_SNDZER, OnSndzer)
	ON_BN_CLICKED(IDC_CLEAR_TERM, OnClearTerm)
	ON_BN_CLICKED(IDC_HEX_MONITOR, OnHexMonitor)
	ON_BN_CLICKED(IDC_PORT2TEXT, OnPort2text)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DOWNLOAD2, OnDownload2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperProgrammerDlg message handlers

BOOL CSuperProgrammerDlg::OnInitDialog()
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

	if (!m_serialCom.Open(100, 100)) {
		//Unable to open serial COM
	} else if (!m_serialCom.SetUp()) {
		m_serSetup = false;
		//Unable to setup serial COM
	} else {
		m_serialCom.Flush();
		m_serSetup = true;
	}

	m_comPort = m_serialCom.GetComNr();

	m_memMap.SubclassDlgItem(IDC_MEMMAP, this);
	m_termEdit.SubclassDlgItem(IDC_TERMINAL, this);

	m_termEdit.SetActive(&m_activeTerm);
	m_termEdit.SetHexMonitor(&m_hexMonitor);
	m_termEdit.SetSerialCom(&m_serialCom);

	m_termEdit.EnableWindow(FALSE);
	m_send.EnableWindow(FALSE);
	m_sendZero.EnableWindow(FALSE);
	m_sendESC.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSuperProgrammerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSuperProgrammerDlg::OnPaint() 
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
HCURSOR CSuperProgrammerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSuperProgrammerDlg::OnProgram() 
{
	unsigned char	recType=0;
	bool			proceed = true;
	bool			serSetup = true;
	CString			msg;

	if (m_serSetup) m_serialCom.Flush();
	else {
		m_status = "COM-port not active";
		UpdateData(FALSE);
		return;
	}

	CreatePageList();

//	outfile = fopen("LowMem.txt", "wt");

	UpdateData(TRUE);

	if (m_fileNPath == "") {
		m_status = "Object filename field is empty";
		UpdateData(FALSE);
		return;
	}

	if (DownloadPages()) return;		//DownloadPages rapporterade ett fel


	CProgPage*	tmpPage;
	FILE	*fp;

	fp = fopen("Pages.txt", "wt");
	
	if (fp) {
		POSITION pos = m_pageList.GetHeadPosition();

		while (pos) {
			tmpPage = m_pageList.GetNext(pos);
			fprintf(fp, "%04X\n", tmpPage->m_startAddress);
		}
		fclose(fp);
	}

/*
	m_status = "Backreading...";
	UpdateData(FALSE);

	outfile = fopen("LowPostReadBack.txt", "wt");

	m_progress.SetPos(0);
	m_progress.SetRange32(0, 32767);

	for (n=0 ; n<4096 / 16 ; n++) {
		m_serialCom.Read( (char *) ReadData, 16);
		m_progress.SetPos(n);
		fprintf(outfile, "%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
			n, ReadData[0], ReadData[1], ReadData[2], ReadData[3], ReadData[4], ReadData[5], 
			ReadData[6], ReadData[7], ReadData[8], ReadData[9], ReadData[10], ReadData[11], 
			ReadData[12], ReadData[13], ReadData[14], ReadData[15]);
	}
	
	fclose(outfile);

	m_status = "Backreading done";
	UpdateData(FALSE);

*/

	m_status = "Succefully downloaded";
	UpdateData(FALSE);
}

void CSuperProgrammerDlg::OnBrowse() 
{
	CFileDialog	MyFDlg(TRUE, "A03", "*.A03", OFN_HIDEREADONLY, "Hex files (*.A03) |*.A03|Hex files (*.HEX) |*.HEX| All Files (*.*) |*.*||", NULL); ;
	char		drive[10];
	char		dir[256];
	char		filename[128];
	char		ext[10];
	CString		cs_filename;
	CString		cs_ext;
	
	if (MyFDlg.DoModal() == IDOK) {

		m_fileNPath = MyFDlg.GetPathName();
		_splitpath(m_fileNPath, drive, dir, filename, ext);
		cs_filename = filename;
		cs_ext = ext;
		m_filename = cs_filename + cs_ext;
		m_status = "File selected";
		UpdateData(FALSE);

		CreatePageList();

	}
	Invalidate(TRUE);
	UpdateWindow();
	UpdateData(FALSE);
}


unsigned char CSuperProgrammerDlg::ReadNextByte(FILE *hexfile)
/****************** Funktion som läser nästa byten i hex-filen *******/
/*
	Funktionen läser två tecken vilka representerar en hexadecimal byte och
	gör om dessa till en unsigned char.
*/

{
	unsigned char tmpbyte;
	unsigned char realbyte = 0;

	tmpbyte = fgetc(hexfile);
	if( tmpbyte != ':' ) {
		if( (65 <= tmpbyte) && (tmpbyte <= 70) ) {
			tmpbyte = tmpbyte - 55;
		}
		if( (48 <= tmpbyte) && (tmpbyte <= 57) ) {
			tmpbyte = tmpbyte - 48;
		}
		realbyte = (tmpbyte) * 16;
		tmpbyte = fgetc(hexfile);

		if( (65 <= tmpbyte) && (tmpbyte <= 70) ) {
			tmpbyte = tmpbyte - 55;
		}
		if( (48 <= tmpbyte) && (tmpbyte <= 57) ) {
			tmpbyte = tmpbyte - 48;
		}
		realbyte = realbyte + (tmpbyte);

		return realbyte;

	} else {
		return tmpbyte;
	}

}

void CSuperProgrammerDlg::OnConntest() 
{
	BYTE				WriteData[2];
	unsigned char		ReadData[48];

	if (m_serSetup) m_serialCom.Flush();
	else {
		m_status = "COM-port not active";
		UpdateData(FALSE);
		return;
	}

	m_status = "Saying Hello!";
	UpdateData(FALSE);
	WriteData[0] = 'H';
	m_serialCom.Write((LPCSTR)WriteData,1);
	m_status = "Waiting for response...";
	UpdateData(FALSE);

	m_serialCom.Read( (char *) ReadData, 42);
	ReadData[48] = 0;

	if (ReadData[0] == 0) {
		m_status = "Error: Link respond time-out";
	} else {			
		m_status.Format("Recivied this: '%s'", ReadData);
	}
	UpdateData(FALSE);
}



void CSuperProgrammerDlg::OnDownload() 
{
	char			readData[100];
	char			ans;
	int				cCount;
	char			name[15];
	char			retName[15];
	CTeleFileSel	teleFS;
	int				selIndex;
	char			writeData[5];
	long			fileLen;
	long			i;
	unsigned char	cs = 0;
	FILE*			outfile;
	bool			lerror = false;
	bool			cancel = false;

	int				l1, l2, l3, l4;
	CString			msg;

	if (m_serSetup) m_serialCom.Flush();
	else {
		m_status = "COM-port not active";
		UpdateData(FALSE);
		return;
	}

	teleFS.SetRetPtr(retName);	
	teleFS.SetRetIndex(&selIndex);	

	m_status = "Getting file information...";
	UpdateData(FALSE);

	m_serialCom.Write("\x1B", 1);
	m_serialCom.Write("D", 1);

	m_serialCom.Read(readData, 1);


	if ((m_serialCom.m_timeOut) || (readData[0] != 'O')) {
		lerror = true;
		m_status = "ERROR: Did not receive OK!";
		UpdateData(FALSE);
	} else {
		m_status = "Getting file information...";
		UpdateData(FALSE);
		do {
			m_serialCom.Write("F", 1);
			m_serialCom.Read(readData, 1);
			ans = readData[0];
			if (ans == 'F') {
				cCount = 0;
				do {
					m_serialCom.Read(readData, 1);
					name[cCount++] = readData[0];
					if ((m_serialCom.m_timeOut) || (cCount == 14)) lerror = true;
				} while ((!lerror) && (readData[0] != 0));
				
				name[12] = 0;	

				if (!lerror) {
					teleFS.AddString(name);
				}
			}
		} while ((!lerror) && (ans == 'F'));
		if ((lerror) || (ans != 'D')) {
			lerror = 1;
			m_status = "ERROR: Did not receive File- or Done ack!";
			UpdateData(FALSE);
		} else {
			m_status = "Please select file";
			UpdateData(FALSE);
			if (teleFS.DoModal() == IDOK) {
				CFileDialog	myFileDlg(FALSE, NULL, retName, OFN_OVERWRITEPROMPT | OFN_EXPLORER, NULL, this);

				if (myFileDlg.DoModal() == IDOK) {
					//selIndex skall innehålla filindex
					//retName skall innehålla filnamnet
					writeData[0] = 'S';
					writeData[1] = (selIndex >> 8) & 0xFF;
					writeData[2] = selIndex & 0xFF;
					m_serialCom.Write(writeData, 3);
					m_serialCom.Read(readData, 1);
					if (readData[0] != 'L') {
						m_status = "ERROR: Did not receive length ack!";
						UpdateData(FALSE);
						lerror = true;
					} else {
						m_serialCom.Read(readData, 1);
						if (m_serialCom.m_timeOut) {
							m_status = "ERROR: Length 4 timeout!";
							UpdateData(FALSE);
							lerror = true;
						} else {
							l4 = readData[0];
							fileLen = (readData[0] & 0xFF) * 0x1000000;
							m_serialCom.Read(readData, 1);
							if (m_serialCom.m_timeOut) {
								m_status = "ERROR: Length 3 timeout!";
								UpdateData(FALSE);
								lerror = true;
							} else {
								l3 = readData[0];
								fileLen += (readData[0] & 0xFF) * 0x10000;
								m_serialCom.Read(readData, 1);
								if (m_serialCom.m_timeOut) {
									m_status = "ERROR: Length 2 timeout!";
									UpdateData(FALSE);
									lerror = true;
								} else {
									l2 = readData[0];
									fileLen += (readData[0] & 0xFF) * 0x100;
									m_serialCom.Read(readData, 1);
									if (m_serialCom.m_timeOut) {
										m_status = "ERROR: Length 1 timeout!";
										UpdateData(FALSE);
										lerror = true;
									} else {
										l1 = readData[0] & 0xFF;
										fileLen += readData[0] & 0xFF;

										msg.Format("l4: %02X, l3: %02X, l2: %02X, l1: %02X, length %d (0x%04X)", l4, l3, l2, l1, fileLen, fileLen);
										MessageBox(msg, "Hello");

									}
								}
							}
						}
					}
					if (!lerror) {
						outfile = fopen(myFileDlg.GetPathName(), "wb");
						m_progress.SetRange32(0, fileLen);
						m_progress.SetPos(0);
						cs = 0xFF;
						m_status = "Downloading file...";
						UpdateData(FALSE);
						m_serialCom.Write("O", 1);
						for (i=0 ; (i<fileLen) && (!lerror) ; i++) {
							m_serialCom.Read(readData, 1);
							if (m_serialCom.m_timeOut) {
								m_status = "ERROR: Data timeout!";
								UpdateData(FALSE);
								lerror = true;
							} else {
								fputc(readData[0], outfile);
								cs ^= readData[0];
							}
							m_progress.SetPos(i);
						}
						fclose(outfile);
						if (!lerror) {
							m_serialCom.Read(readData, 1);
							if (m_serialCom.m_timeOut) {
								m_status = "ERROR: Checksum timeout!";
								UpdateData(FALSE);
								lerror = true;
							} else {
								cs ^= readData[0];
								m_serialCom.Read(readData, 1);
								if (readData[0] != 'D') {
									m_status = "ERROR: Did not receive total done ack!";
									UpdateData(FALSE);
									lerror = true;
								} else {
								}
							}
						}
					}
				} else {
					m_serialCom.Write("C", 1);
					m_serialCom.Read(readData, 1);
					if (readData[0] != 'C') {
						m_status = "ERROR: Did not receive cancel ack!";
						UpdateData(FALSE);
						lerror = true;
					} else {
						cancel = true;
						m_status = "Download canceled";
						UpdateData(FALSE);
					}
				}
			} else {
				m_serialCom.Write("C", 1);
				m_serialCom.Read(readData, 1);
				if (readData[0] != 'C') {
					m_status = "ERROR: Did not receive cancel ack!";
					UpdateData(FALSE);
					lerror = true;
				} else {
					cancel = true;
					m_status = "Download canceled";
					UpdateData(FALSE);
				}
			}
		}
	}
	if (lerror) {
		m_serialCom.Write("E", 1);
	} else {
		if (!cancel) {
			if (cs != 0) {
				m_status = "ERROR: Checksum error!";
				UpdateData(FALSE);
			} else {
				m_status = "File succesfully downloaded";
				UpdateData(FALSE);
			}
		}
	}
}

void CSuperProgrammerDlg::OnSelchangeComport() 
{
	int	oldSel = m_comPort;

	UpdateData(TRUE);
	
	if (m_serialCom.SetComNr(m_comPort, TRUE)) {
		m_serialCom.SetUp();
		m_serialCom.Flush();
	} else {
		m_comPort = oldSel;
		UpdateData(FALSE);
	}
}



	
void CSuperProgrammerDlg::OnUpload() 
{
	CFileDialog		MyFDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST, "All Files (*.*) |*.*||", this); ;
	char			nameBuffer[1024];
	char			drive[10];
	char			dir[256];
	char			filename[128];
	char			ext[10];
	char			newFile[20];
	CString			fileNPath;
	bool			lerror;
	long			len;
	int				retrans;
	char			readData[270];
	unsigned char	sl;
	unsigned char	cs;
	char			c;
	FILE*			srcfile;
	long			reminder;
	long			blockStart;
	int				i	;
	CString			msg;
	POSITION		pos;
	UINT			myTimer;
	MSG				myMsg;
	int				nSelected;
	int				curIter;
	
	if (m_serSetup) m_serialCom.Flush();
	else {
		m_status = "COM-port not active";
		UpdateData(FALSE);
		return;
	}

	m_status = "Select file";
	UpdateData(FALSE);

	MyFDlg.m_ofn.lpstrFile = nameBuffer;
	MyFDlg.m_ofn.nMaxFile = 1024;
	MyFDlg.m_ofn.lpstrFile[0] = 0;

	if (MyFDlg.DoModal() == IDOK) {


		nSelected = 0;
		pos = MyFDlg.GetStartPosition();
		while (pos) {
			nSelected++;
			MyFDlg.GetNextPathName(pos);
		}

		m_mainProgress.SetPos(0);
		m_mainProgress.SetRange(0, nSelected);
		curIter = 0;

		pos = MyFDlg.GetStartPosition();
		while (pos) {

			lerror = false;
			retrans = 0;

			fileNPath = MyFDlg.GetNextPathName(pos);

//			MessageBox("Next File", "Hello");

			_splitpath(fileNPath, drive, dir, filename, ext);
			filename[8] = 0;
			strcpy(newFile, filename);
			strcat(newFile, ext);

			srcfile = fopen(fileNPath, "rb");

			if (srcfile) {

				m_status = "Preparing file...";
				UpdateData(FALSE);

				myTimer = SetTimer(1, 1000, NULL);

				while (!m_timeout) {
					if (GetMessage(&myMsg, NULL, 0, 0) > 0) {
						TranslateMessage(&myMsg);
						DispatchMessage(&myMsg);
					}
				}

				m_status = "Contacting TeleStation...";
				UpdateData(FALSE);

				m_serialCom.Write("\x1B", 1);
				m_serialCom.Write("U", 1);
				
				m_serialCom.Read(readData, 1);

				if (readData[0] == 'O') {
					m_serialCom.Write("F", 1);
					m_serialCom.Write(newFile, strlen(newFile));
					readData[0] = 0x00;
					m_serialCom.Write(readData, 1);
					m_serialCom.Read(readData, 1);

					if (readData[0] == 'O') {
						fseek(srcfile, 0, SEEK_END);
						len = ftell(srcfile);
						rewind(srcfile);
			
						m_progress.SetRange32(0, len);
						m_progress.SetPos(0);

						m_status = "Uploading file... (Retransmitt: 0)";
						UpdateData(FALSE);
	
						reminder = len - ftell(srcfile);
						blockStart = 0;

						while ((reminder > 0) && (!lerror)) {
							if (reminder >= 256) {
								sl = 0xFF;
							} else {
								sl = reminder - 1;
							}

							m_serialCom.Write("L", 1);
							m_serialCom.Write((char*) &sl, 1);
							m_serialCom.Read(readData, 1);
							if (readData[0] != 'O') {
								lerror = true;
								m_status = "ERROR: Didn't receive blocklength ack!";
								UpdateData(FALSE);
							} else {

								cs = 0xFF;

								for (i=0 ; i<(sl+1) ; i++) {
									c = fgetc(srcfile);
									m_serialCom.Write(&c, 1);
									cs ^= c;
								}

								m_serialCom.Write((char*) &cs, 1);

								m_serialCom.Read(readData, 1);					//Hämtar svar
								if (readData[0] != 'O') {
									if (readData[0] == 'R') {
										retrans++;
										msg.Format("Uploading file... (Retransmitt: %d)", retrans);
										UpdateData(FALSE);
										fseek(srcfile, blockStart, SEEK_SET);
									} else {
										lerror = true;
										m_status = "ERROR: Illegal block ack!";
										UpdateData(FALSE);
									}
								}
								blockStart = ftell(srcfile);
								reminder = len - blockStart;
								m_progress.SetPos(blockStart);
							}
						}

						m_serialCom.Write("D", 1);

					} else if (readData[0] == 'I') {
						m_status = "ERROR: TeleStation could not open file!";
						UpdateData(FALSE);
					} else {
						lerror = true;
						m_status = "ERROR: Didn't receive filename ack!";
						UpdateData(FALSE);
					}
				} else {
					lerror = true;
					m_status.Format("ERROR: Didn't receive command ack!, -%c-", readData[0]);
					UpdateData(FALSE);
				} 
				if (lerror) {
					m_serialCom.Write("E", 1);
				} else {
					m_status = "Upload completed succesfully";
					UpdateData(FALSE);
				}
				fclose(srcfile);
			} else {
				m_status = "Could not open file";
				UpdateData(FALSE);
			}

			m_timeout = false;

			curIter++;
			m_mainProgress.SetPos(curIter);

		}
	} else {
		m_status = "Upload canceled";
		UpdateData(FALSE);
	}
}
	
	
	
	
	
	
	

void CSuperProgrammerDlg::OnToWav() 
{
	CFileDialog	MyFDlg(TRUE, "WAW", "*.WAW", OFN_HIDEREADONLY, "Telestation sound files (*.WAW) |*.WAW| All Files (*.*) |*.*||", NULL); ;
	char		drive[10];
	char		dir[256];
	char		filename[128];
	char		ext[10];
	char		newFile[20];
	CString		fileNPath;
	CString		fileTitle;
	FILE*		inf;
	FILE*		outf;
	long		fileLen;
	long		i;
	
	if (MyFDlg.DoModal() == IDOK) {
		fileNPath = MyFDlg.GetPathName();
		_splitpath(fileNPath, drive, dir, filename, ext);
		strcpy(newFile, filename);
		fileTitle = newFile;
		strcat(newFile, ext);
		
		inf = fopen(newFile, "rb");

		if (inf) {

			CFileDialog	MyFDlg2(FALSE, "WAV", fileTitle + ".WAV", OFN_HIDEREADONLY, "Standard PCM file (*.WAV) |*.WAV| All Files (*.*) |*.*||", NULL); ;

			if (MyFDlg2.DoModal() == IDOK) {
				fileNPath = MyFDlg2.GetPathName();
				_splitpath(fileNPath, drive, dir, filename, ext);
				strcpy(newFile, filename);
				strcat(newFile, ext);

				outf = fopen(newFile, "wb");

				if (outf) {
					fseek(inf, 0, SEEK_END);
					fileLen = ftell(inf);
					rewind(inf);

					m_status = "Converting file...";
					UpdateData(FALSE);

					m_progress.SetRange32(0, fileLen);
					m_progress.SetPos(0);
					
					fprintf(outf, "RIFF");
					fputc((fileLen+36) & 0xFF, outf);
					fputc(((fileLen+36)>>8) & 0xFF, outf);
					fputc(((fileLen+36)>>16) & 0xFF, outf);
					fputc(((fileLen+36)>>24) & 0xFF, outf);
					fprintf(outf, "WAVE");
					fprintf(outf, "fmt ");
					fputc(16, outf);
					fputc(0, outf);
					fputc(0, outf);
					fputc(0, outf);
					fputc(1, outf);			//PCM - format
					fputc(0, outf);
					fputc(1, outf);			//1 = Mono, 2 = Stereo
					fputc(0, outf);
					fputc(64, outf);		//8000 = 0x1F40
					fputc(31, outf);		
					fputc(0, outf);
					fputc(0, outf);
					fputc(64, outf);		//1 chan * 8000 Hz * 8 Bits/s / 8 = 0x1F40
					fputc(31, outf);		
					fputc(0, outf);
					fputc(0, outf);
					fputc(1, outf);			//1 byte i taget
					fputc(0, outf);
					fputc(8, outf);			//Format specific???????
					fputc(0, outf);
					fprintf(outf, "data");
					fputc((fileLen) & 0xFF, outf);
					fputc(((fileLen)>>8) & 0xFF, outf);
					fputc(((fileLen)>>16) & 0xFF, outf);
					fputc(((fileLen)>>24) & 0xFF, outf);
					for (i=0 ; i<fileLen ; i++) {
						fputc(fgetc(inf), outf);
						m_progress.SetPos(i);
					}
					fclose(outf);
					m_status = "Conversion done";
					UpdateData(FALSE);
				} else {
					m_status = "Could not open output file";
					UpdateData(FALSE);
				}
			} else {
				m_status = "File conversion canceled";
				UpdateData(FALSE);
			}

			fclose(inf);
		} else {
			m_status = "Could not open input file";
			UpdateData(FALSE);
		}
	} else {
		m_status = "File conversion canceled";
		UpdateData(FALSE);
	}
}

void CSuperProgrammerDlg::OnToWaw() 
{
	CFileDialog	MyFDlg(TRUE, "WAV", "*.WAV", OFN_HIDEREADONLY, "Standard PCM file (*.WAV) |*.WAV| All Files (*.*) |*.*||", NULL); ;
	char		drive[10];
	char		dir[256];
	char		filename[128];
	char		ext[10];
	char		newFile[20];
	CString		fileNPath;
	CString		fileTitle;
	FILE*		inf;
	FILE*		outf;
	long		fileLen;
	long		i;
	char		srstr[5];
	
	if (MyFDlg.DoModal() == IDOK) {
		fileNPath = MyFDlg.GetPathName();
		_splitpath(fileNPath, drive, dir, filename, ext);
		strcpy(newFile, filename);
		fileTitle = newFile;
		strcat(newFile, ext);
		
		inf = fopen(newFile, "rb");

		if (inf) {

			CFileDialog	MyFDlg2(FALSE, "WAW", fileTitle + ".WAW", OFN_HIDEREADONLY, "Telestation sound files (*.WAW) |*.WAW| All Files (*.*) |*.*||", NULL);

			if (MyFDlg2.DoModal() == IDOK) {
				fileNPath = MyFDlg2.GetPathName();
				_splitpath(fileNPath, drive, dir, filename, ext);
				strcpy(newFile, filename);
				strcat(newFile, ext);

				outf = fopen(newFile, "wb");

				if (outf) {
					srstr[0] = fgetc(inf);
					srstr[1] = fgetc(inf);
					srstr[2] = fgetc(inf);
					srstr[3] = fgetc(inf);
					srstr[4] = 0;
					while (strcmp(srstr, "data"))  {
						srstr[0] = srstr[1];
						srstr[1] = srstr[2];
						srstr[2] = srstr[3];
						srstr[3] = fgetc(inf);
					}

					fileLen = fgetc(inf);
					fileLen += fgetc(inf) << 8;
					fileLen += fgetc(inf) << 16;
					fileLen += fgetc(inf) << 24;

					m_status = "Converting file...";
					UpdateData(FALSE);

					m_progress.SetRange32(0, fileLen);
					m_progress.SetPos(0);
					
					for (i=0 ; i<fileLen ; i++) {
						fputc(fgetc(inf), outf);
						m_progress.SetPos(i);
					}
					fclose(outf);
					m_status = "Conversion done";
					UpdateData(FALSE);
				} else {
					m_status = "Could not open output file";
					UpdateData(FALSE);
				}
			} else {
				m_status = "File conversion canceled";
				UpdateData(FALSE);
			}

			fclose(inf);
		} else {
			m_status = "Could not open input file";
			UpdateData(FALSE);
		}
	} else {
		m_status = "File conversion canceled";
		UpdateData(FALSE);
	}
}

void CSuperProgrammerDlg::OnActiveterm() 
{
	UpdateData(TRUE);

	if (m_activeTerm) {
		if (m_serSetup)  {
			m_serialCom.Flush();
			m_program.EnableWindow(FALSE);
			m_conTest.EnableWindow(FALSE);
			m_upload.EnableWindow(FALSE);
			m_download.EnableWindow(FALSE);
			m_download2.EnableWindow(FALSE);
			m_termEdit.EnableWindow(TRUE);
			m_send.EnableWindow(TRUE);
			m_sendZero.EnableWindow(TRUE);
			m_sendESC.EnableWindow(TRUE);
			m_termEdit.StartTimer();
			m_termEdit.SetFocus();
		} else {
			m_status = "COM-port not active";
			m_activeTerm = false;
			UpdateData(FALSE);
		}
	} else {
		m_termEdit.StopTimer();
		m_program.EnableWindow(TRUE);
		m_conTest.EnableWindow(TRUE);
		m_upload.EnableWindow(TRUE);
		m_download.EnableWindow(TRUE);
		m_download2.EnableWindow(TRUE);
		m_termEdit.EnableWindow(FALSE);
		m_send.EnableWindow(FALSE);
		m_sendZero.EnableWindow(FALSE);
		m_sendESC.EnableWindow(FALSE);
	}
}

void CSuperProgrammerDlg::OnKillfocusCharsend() 
{
	CString	old = m_charSend;

	UpdateData(TRUE);

	if (old != m_charSend) {
		m_decSend = m_charSend[0];
		m_hexSend.Format("%02X", m_charSend[0]);
		UpdateData(FALSE);
		m_send.SetFocus();
	}
}

void CSuperProgrammerDlg::OnKillfocusDecsend() 
{
	int	old = m_decSend;

	UpdateData(TRUE);

	if (old != m_decSend) {
		m_charSend.Format("%c", m_decSend);
		m_hexSend.Format("%02X", m_decSend);
		UpdateData(FALSE);
		m_send.SetFocus();
	}
}

void CSuperProgrammerDlg::OnKillfocusHexsend() 
{
	CString	old = m_hexSend;
	int		conv;

	UpdateData(TRUE);

	if (old != m_hexSend) {
		sscanf(m_hexSend, "%02X", &conv);
		m_decSend = conv;
		m_charSend.Format("%c", conv);
		UpdateData(FALSE);
		m_send.SetFocus();
	}
}

void CSuperProgrammerDlg::OnSend() 
{
	char	buff[2];

	UpdateData(TRUE);

	if (m_activeTerm) {
		buff[0] = m_decSend;
		m_serialCom.Write(buff, 1);
		m_termEdit.SetFocus();
	}
}

void CSuperProgrammerDlg::OnSndesc() 
{
	char	buff[2];

	UpdateData(TRUE);

	if (m_activeTerm) {
		buff[0] = 0x1B;
		m_serialCom.Write(buff, 1);
		m_termEdit.SetFocus();
	}
}

void CSuperProgrammerDlg::OnSndzer() 
{
	char	buff[2];

	UpdateData(TRUE);

	if (m_activeTerm) {
		buff[0] = 0x00;
		m_serialCom.Write(buff, 1);
		m_termEdit.SetFocus();
	}
}

void CSuperProgrammerDlg::OnClearTerm() 
{
	m_termEdit.SetSel(0, -1);
	m_termEdit.Clear();
	m_termEdit.SetFocus();
}

void CSuperProgrammerDlg::OnHexMonitor() 
{
	UpdateData(TRUE);
	m_termEdit.SetFocus();
}

void CSuperProgrammerDlg::OnPort2text() 
{
	CFileDialog	MyFDlg(TRUE, "BIN", "*.BIN", OFN_HIDEREADONLY, "Telestation port files (*.BIN) |*.BIN| All Files (*.*) |*.*||", NULL); ;
	char		drive[10];
	char		dir[256];
	char		filename[128];
	char		ext[10];
	char		newFile[20];
	CString		fileNPath;
	CString		fileTitle;
	FILE*		inf;
	FILE*		outf;
	long		fileLen;
	long		i;
	char		c;
	
	if (MyFDlg.DoModal() == IDOK) {
		fileNPath = MyFDlg.GetPathName();
		_splitpath(fileNPath, drive, dir, filename, ext);
		strcpy(newFile, filename);
		fileTitle = newFile;
		strcat(newFile, ext);
		
		inf = fopen(newFile, "rb");

		if (inf) {

			CFileDialog	MyFDlg2(FALSE, "TXT", fileTitle + ".TXT", OFN_HIDEREADONLY, "Standard text file (*.TXT) |*.TXT| All Files (*.*) |*.*||", NULL); ;

			if (MyFDlg2.DoModal() == IDOK) {
				fileNPath = MyFDlg2.GetPathName();
				_splitpath(fileNPath, drive, dir, filename, ext);
				strcpy(newFile, filename);
				strcat(newFile, ext);

				outf = fopen(newFile, "wb");

				if (outf) {
					fseek(inf, 0, SEEK_END);
					fileLen = ftell(inf);
					rewind(inf);

					m_status = "Converting file...";
					UpdateData(FALSE);

					m_progress.SetRange32(0, fileLen);
					m_progress.SetPos(0);
					
					for (i=0 ; i<fileLen/4 ; i++) {
						c = fgetc(inf);
						c = fgetc(inf);
						c = fgetc(inf);
						c = fgetc(inf);
						if (c & 0x80) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x40) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x20) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x10) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x08) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x04) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x02) fprintf(outf, "1\t");
						else fprintf(outf, "0\t");
						if (c & 0x01) fprintf(outf, "1\n");
						else fprintf(outf, "0\n");
						m_progress.SetPos(i);
					}
					fclose(outf);
					m_status = "Conversion done";
					UpdateData(FALSE);
				} else {
					m_status = "Could not open output file";
					UpdateData(FALSE);
				}
			} else {
				m_status = "File conversion canceled";
				UpdateData(FALSE);
			}

			fclose(inf);
		} else {
			m_status = "Could not open input file";
			UpdateData(FALSE);
		}
	} else {
		m_status = "File conversion canceled";
		UpdateData(FALSE);
	}
}

void CSuperProgrammerDlg::OnTimer(UINT nIDEvent) 
{
	m_timeout = true;
	
	CDialog::OnTimer(nIDEvent);
}

void CSuperProgrammerDlg::OnDownload2() 
{
	char			readData[100];
	char			ans;
	int				cCount;
	char			name[15];
	char			retName[15];
	CTeleFileSel	teleFS;
	int				selIndex;
	
	int				SelIndexArray[1024];
	int				nSelected;
	int				j;
	CString			tmpPath;
	CString			destPath;

	char			writeData[5];
	long			fileLen;
	long			i;
	unsigned char	cs = 0;
	FILE*			outfile;
	bool			lerror = false;
	bool			cancel = false;

	int				l1, l2, l3, l4;
	CString			msg;

	UINT			myTimer;
	MSG				myMsg;

	if (m_serSetup) m_serialCom.Flush();
	else {
		m_status = "COM-port not active";
		UpdateData(FALSE);
		return;
	}

	teleFS.SetRetPtr(retName);	
	teleFS.SetRetIndex(&selIndex);	

	teleFS.SetMaxSelected(1024);
	teleFS.SetnSelected(&nSelected);
	teleFS.SetSelIndexArray(SelIndexArray);


	m_serialCom.Flush();

	m_status = "Getting file information...";
	UpdateData(FALSE);

	m_serialCom.Write("\x1B", 1);
	m_serialCom.Write("D", 1);

	m_serialCom.Read(readData, 1);


	if ((m_serialCom.m_timeOut) || (readData[0] != 'O')) {
	
		lerror = true;
		m_status = "ERROR: Did not receive OK!";
		UpdateData(FALSE);
	} else {
		m_status = "Getting file information...";
		UpdateData(FALSE);
		do {
			m_serialCom.Write("F", 1);
			m_serialCom.Read(readData, 1);
			ans = readData[0];
			if (ans == 'F') {
				cCount = 0;
				do {
					m_serialCom.Read(readData, 1);
					name[cCount++] = readData[0];
					if ((m_serialCom.m_timeOut) || (cCount == 14)) lerror = true;
				} while ((!lerror) && (readData[0] != 0));
				
				name[12] = 0;	

				if (!lerror) {
					teleFS.AddString(name);
				}
			}
		} while ((!lerror) && (ans == 'F'));
		if ((lerror) || (ans != 'D')) {
			lerror = 1;
			m_status = "ERROR: Did not receive File- or Done ack!";
			UpdateData(FALSE);
		} else {

			writeData[0] = 'C';						//Tvingar Download till fel för att börja om
			m_serialCom.Write(writeData, 1);

			m_serialCom.Read(readData, 1);
			if (readData[0] != 'C') {
				m_status = "ERROR: Did not receive cancel ack!";
				UpdateData(FALSE);
				lerror = true;
			}

			m_status = "Please select file";
			UpdateData(FALSE);

			nSelected = 0;

			if ((teleFS.DoModal() == IDOK) && (nSelected > 0)) {

				destPath = SelectFileDestination();

				if (destPath != "") {

					m_mainProgress.SetPos(0);
					m_mainProgress.SetRange(0, nSelected);

					for (j=0 ; (j<nSelected) && (!lerror) ; j++) {

						m_mainProgress.SetPos(j);

						m_timeout = false;

						myTimer = SetTimer(1, 1000, NULL);

						m_status = "Preparing next file...";
						UpdateData(FALSE);

						while (!m_timeout) {
							if (GetMessage(&myMsg, NULL, 0, 0) > 0) {
								TranslateMessage(&myMsg);
								DispatchMessage(&myMsg);
							}

						}

						do {
							m_serialCom.Read(readData, 1);
						} while (readData[0] != 0);

						m_serialCom.Flush();

						m_serialCom.Write("\x1B", 1);
						m_serialCom.Write("D", 1);
						m_serialCom.Read(readData, 1);
						if ((m_serialCom.m_timeOut) || (readData[0] != 'O')) {
							lerror = true;
							m_status = "ERROR: Did not receive OK!";
							UpdateData(FALSE);
						} else {
							m_status = "Getting file information...";
							UpdateData(FALSE);
							do {
								m_serialCom.Write("F", 1);
								m_serialCom.Read(readData, 1);
								ans = readData[0];
								if (ans == 'F') {
									cCount = 0;
									do {
										m_serialCom.Read(readData, 1);
										if ((m_serialCom.m_timeOut) || (cCount == 14)) lerror = true;
									} while ((!lerror) && (readData[0] != 0));
								}
							} while ((!lerror) && (ans == 'F'));
							if ((lerror) || (ans != 'D')) {
								lerror = 1;
								m_status = "ERROR: Did not receive File- or Done ack!";
								UpdateData(FALSE);
							} else {
								writeData[0] = 'S';
								writeData[1] = (SelIndexArray[j] >> 8) & 0xFF;
								writeData[2] = SelIndexArray[j] & 0xFF;
								m_serialCom.Write(writeData, 3);
								m_serialCom.Read(readData, 1);
								if (readData[0] != 'L') {
									m_status = "ERROR: Did not receive length ack!";
									UpdateData(FALSE);
									lerror = true;
								} else {
									m_serialCom.Read(readData, 1);
									if (m_serialCom.m_timeOut) {
										m_status = "ERROR: Length 4 timeout!";
										UpdateData(FALSE);
										lerror = true;
									} else {
										l4 = readData[0];
										fileLen = (readData[0] & 0xFF) * 0x1000000;
										m_serialCom.Read(readData, 1);
										if (m_serialCom.m_timeOut) {
											m_status = "ERROR: Length 3 timeout!";
											UpdateData(FALSE);
											lerror = true;
										} else {
											l3 = readData[0];
											fileLen += (readData[0] & 0xFF) * 0x10000;
											m_serialCom.Read(readData, 1);
											if (m_serialCom.m_timeOut) {
												m_status = "ERROR: Length 2 timeout!";
												UpdateData(FALSE);
												lerror = true;
											} else {
												l2 = readData[0];
												fileLen += (readData[0] & 0xFF) * 0x100;
												m_serialCom.Read(readData, 1);
												if (m_serialCom.m_timeOut) {
													m_status = "ERROR: Length 1 timeout!";
													UpdateData(FALSE);
													lerror = true;
												} else {
													l1 = readData[0] & 0xFF;
													fileLen += readData[0] & 0xFF;
												}
											}
										}
									}
								}
								if (!lerror) {
									tmpPath = destPath + teleFS.GetString(SelIndexArray[j]);
									outfile = fopen(tmpPath, "wb");
									m_progress.SetRange32(0, fileLen);
									m_progress.SetPos(0);
									cs = 0xFF;
									m_status = "Downloading file...";
									UpdateData(FALSE);
									m_serialCom.Write("O", 1);
									for (i=0 ; (i<fileLen) && (!lerror) ; i++) {
										m_serialCom.Read(readData, 1);
										if (m_serialCom.m_timeOut) {
											m_status = "ERROR: Data timeout!";
											UpdateData(FALSE);
											lerror = true;
										} else {
											fputc(readData[0], outfile);
											cs ^= readData[0];
										}
										m_progress.SetPos(i);
									}
									fclose(outfile);
									if (!lerror) {
										m_serialCom.Read(readData, 1);
										if (m_serialCom.m_timeOut) {
											m_status = "ERROR: Checksum timeout!";
											UpdateData(FALSE);
											lerror = true;
										} else {
											cs ^= readData[0];
											m_serialCom.Read(readData, 1);
											if (readData[0] != 'D') {
												m_status = "ERROR: Did not receive total done ack!";
												UpdateData(FALSE);
												lerror = true;
											} else {
											}
										}
									} else {
										m_serialCom.Write("C", 1);
										m_serialCom.Read(readData, 1);
										if (readData[0] != 'C') {
											m_status = "ERROR: Did not receive cancel ack!";
											UpdateData(FALSE);
											lerror = true;
										} else {
											cancel = true;
											m_status = "Download canceled";
											UpdateData(FALSE);
										}
									}
								} else {
									m_serialCom.Write("C", 1);
									m_serialCom.Read(readData, 1);
									if (readData[0] != 'C') {
										m_status = "ERROR: Did not receive cancel ack!";
										UpdateData(FALSE);
										lerror = true;
									} else {
										cancel = true;
										m_status = "Download canceled";
										UpdateData(FALSE);
									}
								}
							}
						}
						if (lerror) {
							m_serialCom.Write("E", 1);
						} else {
							if (!cancel) {
								if (cs != 0) {
									m_status = "ERROR: Checksum error!";
									UpdateData(FALSE);
								} else {
									m_status = "File succesfully downloaded";
									UpdateData(FALSE);
								}
							}
						}
					}
				}
			}
		}
	}
	m_mainProgress.SetPos(nSelected);
}


CString CSuperProgrammerDlg::SelectFileDestination( void )
{
	CString	selectedLocation = "";

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
				selectedLocation = pszBuffer;
				selectedLocation += "\\";
            }
            // Free the PIDL allocated by SHBrowseForFolder.
            pMalloc->Free(pidl);
        }
        // Release the shell's allocator.
        pMalloc->Release();
    }
	return selectedLocation;
}


int CSuperProgrammerDlg::ReadIntelFile(FILE* hexfile)
{
	unsigned char	recType=0;
	bool			serSetup = true;
	CString			msg;

	int				dataLength;
	int				i;
	UINT			offset;
	char			rowData[16];
	bool			proceed = true;

	if( (ReadNextByte(hexfile)) != ':' ) {
		m_status = "Error reading HEX-file. No ':' found";
			recType = 1;
	} else {
		m_status = "Reading Hex-file...";
	}
	UpdateData(FALSE);

	rewind(hexfile);

	m_memMap.MarkEmpty(0, 65535);
	m_memMap.Invalidate(FALSE);
	m_memMap.UpdateWindow();;

	while( !m_pageList.IsEmpty( ) ) {
		delete m_pageList.RemoveHead( );
	}	


	while((recType == 0 ) && (proceed)) {

		while((ReadNextByte(hexfile) != ':'));

		dataLength = ReadNextByte(hexfile);
		offset = ReadNextByte(hexfile) * 256;
		offset += ReadNextByte(hexfile);

		recType = ReadNextByte(hexfile);

		if (recType == 0) {

			for (i=0 ; i<dataLength ; i++) {
				rowData[i] = ReadNextByte(hexfile);
			}

			ReadNextByte(hexfile);			//Eat CS

			InsertDataRow(offset, dataLength, rowData);

		} else {
			if( recType == 1 ) {
				if( (ReadNextByte(hexfile)) != 0xFF ) {	
					m_status = "Error in last record. Last byte was not FF";
					UpdateData(FALSE);
					proceed = false;
				}
			} else {
				m_status = "Error reading HEX-file. Bad file format";
				UpdateData(FALSE);
				proceed = false;
			}
		}
	}

	if (proceed) return 0;
	return 0;
}



void	CSuperProgrammerDlg::CreatePageList( void )
{
	FILE			*hexfile;
	unsigned char	recType=0;

	if (m_filename != "") {

		hexfile = fopen(m_filename, "rb");
		if (hexfile != NULL) {

			if (ReadIntelFile(hexfile) == 0) {
				CProgPage*	tmpPage;

				POSITION pos = m_pageList.GetHeadPosition();

				while (pos) {
					tmpPage = m_pageList.GetNext(pos);
					m_memMap.MarkOccupied(tmpPage->m_startAddress, tmpPage->m_startAddress);
				}

				m_memMap.Invalidate(FALSE);
				m_memMap.UpdateWindow();

//					while( !m_pageList.IsEmpty( ) ) {
//						delete m_pageList.RemoveHead( );
//					}
				m_status = "File OK";
			}

			fclose(hexfile);

		} else {
			m_status = "Could not open file";
		}
		fclose(hexfile);
	} else {
		m_status = "Path and file name field is empty";
	}
	Invalidate(TRUE);
	UpdateWindow();
	UpdateData(FALSE);
}







void CSuperProgrammerDlg::InsertDataRow(UINT offset, int dataLength, char* rowData)
{
	UINT	endAdr = offset + dataLength;

	//Defalut assignments
	int		preSplitLength = dataLength;

	if (((endAdr - 1) / PAGE_SIZE) != (offset / PAGE_SIZE)) {
		//Two pages
		UINT	postSplitOffset = ((offset / PAGE_SIZE) + 1) * PAGE_SIZE;
		preSplitLength = postSplitOffset - offset;
		int		postSplitLength = dataLength - preSplitLength;
		InsertSinglePageData(postSplitOffset, postSplitLength, (char*) (rowData + preSplitLength));
	}
	InsertSinglePageData(offset, preSplitLength, rowData);
}


void CSuperProgrammerDlg::InsertSinglePageData(UINT offset, int dataLength, char* rowData)
{	
	int		i;
	UINT	pageStart = (offset / PAGE_SIZE) * PAGE_SIZE;

	CProgPage*	tmpPage = FindPage(pageStart);

	if (!tmpPage) {
		//Create new CProgPage
		tmpPage = new CProgPage(pageStart);
		m_pageList.AddTail(tmpPage);
		for (i=0 ; i<64 ; i++) tmpPage->m_data[i] = 0xAA;
	}
	for (i=0 ; i<dataLength ; i++) {
		tmpPage->m_data[i+offset-pageStart] = rowData[i];
	}
	tmpPage->m_startAddress = pageStart;
}

CProgPage* CSuperProgrammerDlg::FindPage(UINT offset)
{
	CProgPage*	tmpPage;

	POSITION pos = m_pageList.GetHeadPosition();

	while (pos) {
		tmpPage = m_pageList.GetNext(pos);
		if (tmpPage->m_startAddress == (offset / PAGE_SIZE) * PAGE_SIZE)  return tmpPage;
	}

	return NULL;
}


int CSuperProgrammerDlg::DownloadPages( void )
//Ret 0 = OK, Nonzero = error
{
	char	ans;
	char	ansMsg[100];
	CString	msg;
	bool	proceed;
	int		pageCount = 0;
	
	m_progress.SetRange(0, m_pageList.GetCount());
	m_progress.SetPos(0);

	CProgPage*	tmpPage;

	m_serialCom.Write("0",1);
	m_serialCom.Read(ansMsg, 16);
	ansMsg[14] = 0;
	msg = ansMsg;
	if (msg != "ROM 0 selected") {
		m_status = "Illegal response, tried to select rom 0000-7FFF";
		UpdateData(FALSE);
		proceed = false;
	}

	if (proceed) {
		m_serialCom.Write("P",1);
		m_serialCom.Read(&ans, 1);
		if (ans != 'P') {
			proceed = false;
			if (ans == 0) {
				m_status = "Error: Link respond time-out";
				UpdateData(FALSE);
			} else {
				m_status.Format("Recivied this: '%c'", ans);
				UpdateData(FALSE);
			}
		}
	}

	m_status = "Downloading higher half...";
	UpdateData(FALSE);

	POSITION pos = m_pageList.GetHeadPosition();

	while ((pos) && (proceed)) {
		tmpPage = m_pageList.GetNext(pos);
//		if ((tmpPage->m_startAddress + PAGE_SIZE) >= 0x8000) {
		if ((tmpPage->m_startAddress) >= 0x8000) {
			if (DownloadOnePage(tmpPage)) {
				proceed = false;
			} else {
				m_memMap.MarkProgrammed(tmpPage->m_startAddress, tmpPage->m_startAddress);
				m_memMap.Invalidate(FALSE);
				m_memMap.UpdateWindow();;
				m_progress.SetPos(++pageCount);
			}
		}
	}

	if (proceed) {
		m_serialCom.Write("D",1);
		m_serialCom.Read(ansMsg, 4);
		msg = ansMsg;
		if (msg != "D1D2") {
			m_status = "Illegal response, finished with rom 0000-7FFF";
			UpdateData(FALSE);
			proceed = false;
		}
	}

	if (proceed) {
		m_serialCom.Write("1",1);
		m_serialCom.Read(ansMsg, 16);
		ansMsg[14] = 0;
		msg = ansMsg;
		if (msg != "ROM 1 selected") {
			m_status = "Illegal response, tried to select rom 8000-FFFF";
			UpdateData(FALSE);
			proceed = false;
		}
	}

	if (proceed) {
		m_serialCom.Write("P",1);
		m_serialCom.Read(&ans, 1);
		if (ans != 'P') {
			proceed = false;
			if (ans == 0) {
				m_status = "Error: Link respond time-out";
				UpdateData(FALSE);
			} else {
				m_status.Format("Recivied this: '%c'", ans);
				UpdateData(FALSE);
			}
		}
	}

	m_status = "Downloading lower half...";
	UpdateData(FALSE);

	pos = m_pageList.GetHeadPosition();

	while ((pos) && (proceed)) {
		tmpPage = m_pageList.GetNext(pos);
//		if ((tmpPage->m_startAddress + PAGE_SIZE) < 0x8000) {
		if ((tmpPage->m_startAddress) < 0x8000) {
			if (DownloadOnePage(tmpPage)) {
				proceed = false;
			} else {
				m_memMap.MarkProgrammed(tmpPage->m_startAddress, tmpPage->m_startAddress);
				m_memMap.Invalidate(FALSE);
				m_memMap.UpdateWindow();;
				m_progress.SetPos(++pageCount);
			}
		}
	}

	if (proceed) {
		m_serialCom.Write("D",1);
		m_serialCom.Read(ansMsg, 4);
		msg = ansMsg;
		if (msg != "D1D2") {
			m_status = "Illegal response, finished with rom 8000-FFFF";
			UpdateData(FALSE);
			proceed = false;
		}
	}
	if (proceed) return 0;
	return 1;
}

int CSuperProgrammerDlg::DownloadOnePage(CProgPage* page)
//Ret 0 = OK, Nonzero = error
{
	BYTE	writeData[5];
	UINT	startAddress;
	char	ans;
	unsigned char	cs;
	unsigned char	c;
	int		i = 0;

	if (page->m_startAddress < 0x8000) {
		startAddress = page->m_startAddress + 0x8000; // All programmering sker i över halvan av minnesutrymmet
	} else {
		startAddress = page->m_startAddress;
	}

	do {
		writeData[0] = 'F';
		m_serialCom.Write((LPCSTR)writeData, 1);
		writeData[0] = (startAddress >> 8) & 0xFF;
		m_serialCom.Write((LPCSTR)writeData, 1);
		writeData[0] = startAddress & 0xFF;
		m_serialCom.Write((LPCSTR)writeData, 1);
		writeData[0] = PAGE_SIZE;
		m_serialCom.Write((LPCSTR)writeData, 1);


/*

		while (!m_serialCom.CharacterPending()) {
			m_serialCom.Write(writeData, 1);
			Sleep(5);
			if (m_serialCom.CharacterPending() == 0) {
				m_status.Format("Test no: %d, nothing yet", i++);
				UpdateData(FALSE);
			} else {
				MessageBox("Now we cooking", "Hello");
				m_status.Format("Test ended at: %d", i);
				UpdateData(FALSE);
			}
		}

		return 1;
*/



		cs = 255;

		for (i=0 ; i<PAGE_SIZE ; i++) {
			c = page->m_data[i] & 0xFF;
			cs ^= c;
			writeData[0] = c;
			m_serialCom.Write((LPCSTR)writeData, 1);
//			m_status.Format("Writing byte: %02X at %d", writeData[0], i);
//			UpdateData(FALSE);
//			Sleep(100);
		}
		writeData[0] = cs;
		m_serialCom.Write((LPCSTR)writeData, 1);

		i = 0;
/*
		while (!m_serialCom.CharacterPending()) {
			m_serialCom.Write(writeData, 1);
			Sleep(10);
			if (m_serialCom.CharacterPending() == 0) {
				m_status.Format("Test no: %d, nothing yet", i++);
				UpdateData(FALSE);
			} else {
				MessageBox("Now we cooking", "Hello");
			}
		}
*/	
		m_serialCom.Read(&ans, 1);
		if (m_serialCom.m_timeOut) MessageBox("Link reported timeout", "ERROR");
	} while (ans == 'R');

	if (ans != 'F') {
		m_status = "Didnt get 'F' character after block";
		UpdateData(FALSE);
		return 1;
	}
	return 0;
}

void CSuperProgrammerDlg::PostNcDestroy() 
{
	while( !m_pageList.IsEmpty( ) ) {
		delete m_pageList.RemoveHead( );
	}	

	CDialog::PostNcDestroy();
}

void	CSuperProgrammerDlg::CloseSerialPort( void )
{
	if (m_serSetup) m_serialCom.Close();
}
