// SuperProgrammerDlg.cpp : implementation file
//

//#include <stdio.h>
#include "stdafx.h"
#include "SuperProgrammer.h"
#include "SuperProgrammerDlg.h"
#include "serialco.h"

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
// CSuperProgrammerDlg dialog

CSuperProgrammerDlg::CSuperProgrammerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperProgrammerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperProgrammerDlg)
	m_comPort = 0;
	m_filename = _T("");
	m_status = _T("FRHA welcomes you");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSuperProgrammerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperProgrammerDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_CBIndex(pDX, IDC_COMPORT, m_comPort);
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDX_Text(pDX, IDC_STATUS, m_status);
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
	ON_BN_CLICKED(IDC_FINALIZE, OnFinalize)
	ON_BN_CLICKED(IDC_GETHIGH, OnGethigh)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
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

	m_memMap.SubclassDlgItem(IDC_MEMMAP, this);

	
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
	BYTE				WriteData[2];
	unsigned char		ReadData[48];
	FILE			*hexfile;
	FILE			*outfile;
	unsigned char	DataLength, OffsetLowByte, OffsetHighByte;
	unsigned char	RecType=0;
	unsigned char	c;
	unsigned char	cs;
	int				n,Offset,l=0;
	int				blockRoof; 
	bool			proceed = true;
	long			lastBlock;
	long			fileEnd;
	CString			msg;


	outfile = fopen("HighMem.txt", "wt");
	
	if (m_fileNPath == "") {
		proceed = false;
		m_status = "Path and file name field is empty";
	}
	if ((proceed) && ((hexfile = fopen(m_fileNPath, "rb")) == NULL)) {
		proceed = false;
		m_status = "Could not open file";
	}
	if ((proceed) && (!m_serialCom.Open(100, 100))) {
		proceed = false;
		m_status = "The selected COM-port could not initialize";
	}
	if ((proceed) && (!m_serialCom.SetUp())) {
		proceed = false;
		m_status = "Error setting up COM-port";
	}
	if (proceed) {
		
		m_serialCom.Flush();

		m_status = "Programming high memory";
		UpdateData(FALSE);
		
		WriteData[0] = 'P';
		m_serialCom.Write((LPCSTR)WriteData,1);
		m_serialCom.Read( (char *) ReadData, 1);
		if (ReadData[0] != 'P') {
			proceed = false;
			if (ReadData[0] == 0) {
				m_status = "Error: Link respond time-out";
			} else {
				m_status.Format("Recivied this: '%s'", ReadData);
			}
		}
	}
	if (proceed) {
		if( (ReadNextByte(hexfile)) != ':' ) {
			m_status = "Error reading HEX-file. No ':' found";
			RecType = 1;
		} else {
			m_status = "Reading Hex-file...";
		}



		//Programmera höga minnet

		m_status = "Programming high memory";
		UpdateData(FALSE);
		


		fseek(hexfile, 0, SEEK_END);
		fileEnd = ftell(hexfile);

		m_progress.SetRange32(0, fileEnd);

		fseek(hexfile, 0, 0);

		while( RecType == 0 ) {

			lastBlock = ftell(hexfile);

			m_progress.SetPos(lastBlock);

			l++;
			while((ReadNextByte(hexfile) != ':'));

			
			
			DataLength = ReadNextByte(hexfile);
			OffsetHighByte = ReadNextByte(hexfile);
			OffsetLowByte = ReadNextByte(hexfile);

			Offset = ( OffsetHighByte*256 ) + OffsetLowByte;

			if (Offset >= 0x8000) {

				blockRoof = Offset / 1024;
				
				blockRoof = (blockRoof * 1024) + 64;
				if (Offset + DataLength > blockRoof) {
					msg.Format("Attempt to exceed roof\nOffset: %04X, BlockRoof: %04X, Length: %d", Offset, blockRoof, DataLength);
				}

				WriteData[0] = 'F';
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = OffsetHighByte;
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = OffsetLowByte;
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = DataLength;
				m_serialCom.Write((LPCSTR)WriteData,1);

				fprintf(outfile, "%02X%02X ", OffsetHighByte, OffsetLowByte);

			}

			RecType = ReadNextByte(hexfile);

			if( RecType == 0 ) {
				if (Offset >= 0x8000) {
					m_memMap.MarkProgrammed(Offset, Offset+DataLength);
					m_memMap.Invalidate(FALSE);
					m_memMap.UpdateWindow();;
					cs = 0xFF;
					for( n=0; n<DataLength; n++ ) {
						Offset++;
						c = ReadNextByte(hexfile);
						cs ^= c;
						WriteData[0] = c;
						m_serialCom.Write((LPCSTR)WriteData,1);
						fprintf(outfile, "%02X ", c);
					}
					fprintf(outfile, "\n");
					WriteData[0] = cs;
					m_serialCom.Write((LPCSTR)WriteData,1);
				} else {
					for( n=0; n<DataLength; n++ ) {
						ReadNextByte(hexfile);
					}
				}
				ReadNextByte(hexfile); //Läser in chksumma, orsakar fel om 3A (:)2

				if (Offset >= 0x8000) {
					m_serialCom.Read( (char *) ReadData, 1);
					if (ReadData[0] == 'R') {
						fseek(hexfile, lastBlock, SEEK_SET);
						m_status = "Resending!!!!";
						UpdateData(FALSE);
					} else if (ReadData[0] != 'F') {
						m_status = "Illegal response from SuperComputer";
						proceed = false;
						RecType = 0;
					}
				}
			} else {
				if( RecType == 1 ) {
					if( (ReadNextByte(hexfile)) != 255 ) {	
						m_status = "Error in last record. Last byte was not FF";
						proceed = false;
					} else {
						m_status = "High succesfully programmed";
					}
				} else {
					m_status = "Error reading HEX-file. Bad file format";
					proceed = false;
				}
			}
		}
	}

	fclose(outfile);
	
	outfile = fopen("LowMem.txt", "wt");

	if (proceed) {
		
		//Programmera låga minnet

		WriteData[0] = 'L';
		m_serialCom.Write((LPCSTR)WriteData,1);

/*
		do {
			m_serialCom.Read( (char *) ReadData, 1);
		} while (ReadData[0] != 'L');

		proceed = true;
*/		
		
		m_serialCom.Read( (char *) ReadData, 1);
		if (ReadData[0] != 'L') {
			proceed = false;
			if (ReadData[0] == 0) {
				m_status = "Error: Link respond time-out";
			} else {
				m_status.Format("Recivied this: '%s'", ReadData);
			}
		}

  
	}
	if (proceed) {

		m_status = "Programming low memory";
		UpdateData(FALSE);
		
		m_progress.SetPos(0);

		fseek(hexfile, 0, 0);

		RecType = 0;

		while( RecType == 0 ) {

			lastBlock = ftell(hexfile);

			m_progress.SetPos(lastBlock);

			l++;
			while((ReadNextByte(hexfile) != ':'));

			
			
			DataLength = ReadNextByte(hexfile);
			OffsetHighByte = ReadNextByte(hexfile);
			OffsetLowByte = ReadNextByte(hexfile);
			RecType = ReadNextByte(hexfile);

			if( RecType == 0 ) {

				Offset = ( OffsetHighByte*256 ) + OffsetLowByte;

				if (Offset < 0x8000) {

					blockRoof = Offset / 1024;
					
					blockRoof = (blockRoof * 1024) + 64;
					if (Offset + DataLength > blockRoof) {
						msg.Format("Attempt to exceed roof\nOffset: %04X, BlockRoof: %04X, Length: %d", Offset, blockRoof, DataLength);
					}

					WriteData[0] = 'F';
					m_serialCom.Write((LPCSTR)WriteData,1);
					WriteData[0] = OffsetHighByte;
					m_serialCom.Write((LPCSTR)WriteData,1);
					WriteData[0] = OffsetLowByte;
					m_serialCom.Write((LPCSTR)WriteData,1);
					WriteData[0] = DataLength;
					m_serialCom.Write((LPCSTR)WriteData,1);

					fprintf(outfile, "%02X%02X ", OffsetHighByte, OffsetLowByte);

					cs = 0xFF;
				}

				if (Offset < 0x8000) {
					m_memMap.MarkProgrammed(Offset, Offset+DataLength);
					m_memMap.Invalidate(FALSE);
					m_memMap.UpdateWindow();;
					cs = 0xFF;
					for( n=0; n<DataLength; n++ ) {
						c = ReadNextByte(hexfile);
						cs ^= c;
						WriteData[0] = c;
						m_serialCom.Write((LPCSTR)WriteData,1);
						fprintf(outfile, "%02X ", c);
					}
					fprintf(outfile, "\n");
					WriteData[0] = cs;
					m_serialCom.Write((LPCSTR)WriteData,1);

				} else {
					for( n=0; n<DataLength; n++ ) {
						ReadNextByte(hexfile);
					}
				}
				ReadNextByte(hexfile); //Läser in chksumma, orsakar fel om 3A (:)2

				if (Offset < 0x8000) {
					m_serialCom.Read( (char *) ReadData, 1);
					if (ReadData[0] == 'R') {
						fseek(hexfile, lastBlock, SEEK_SET);
						m_status = "Resending!!!!";
						UpdateData(FALSE);
					} else if (ReadData[0] != 'F') {
						m_status = "Illegal response from SuperComputer";
						proceed = false;
						RecType = 0;
					}
				}
			} else {
				if( RecType == 1 ) {
					if( (ReadNextByte(hexfile)) != 255 ) {	
						m_status = "Error in last record. Last byte was not FF";
					} else {
						m_status = "File succesfully programmed";
					}
				} else {
					m_status = "Error reading HEX-file. Bad file format";
					UpdateData(FALSE);
				}
			}
		}
		
		
	}

	WriteData[0] = 'D';
	m_serialCom.Write((LPCSTR)WriteData,1);
	m_serialCom.Read( (char *) ReadData, 4);

	if ((ReadData[0] != 'D') && (ReadData[1] != '1') && (ReadData[2] != 'D') && (ReadData[3] != '2')) 
		MessageBox("Didn't receive D-character", "Something is fishy");

	fclose(hexfile);
	fclose(outfile);

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

	m_serialCom.Close();
	UpdateData(FALSE);
}

void CSuperProgrammerDlg::OnBrowse() 
{
	CFileDialog	MyFDlg(TRUE, "A03", "*.A03", OFN_HIDEREADONLY, "Hex files (*.A03) |*.A03| All Files (*.*) |*.*||", NULL); ;
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

		FILE			*hexfile;
		unsigned char	DataLength, OffsetLowByte, OffsetHighByte;
		unsigned char	RecType=0;
		int				n,Offset,l=0;

		if (m_filename != "") {

			hexfile = fopen(m_filename, "rb");
			if (hexfile != NULL) {
				if( (ReadNextByte(hexfile)) != ':' ) {
					m_status = "Error reading HEX-file. No ':' found";
					RecType = 1;
				} else {
					m_status = "Reading Hex-file...";
				}

				fseek(hexfile, 0, 0);
				m_memMap.MarkEmpty(0, 65535);
				while( RecType == 0 ) {

					l++;
					while((ReadNextByte(hexfile) != ':'));
					DataLength = ReadNextByte(hexfile);
					OffsetHighByte = ReadNextByte(hexfile);
					OffsetLowByte = ReadNextByte(hexfile);
					Offset = ( OffsetHighByte*256 ) + OffsetLowByte;
					RecType = ReadNextByte(hexfile);

					if( RecType == 0 ) {
						m_memMap.MarkOccupied(Offset, Offset+DataLength);
						for( n=0; n<DataLength; n++ ) {
							ReadNextByte(hexfile);
						}
						ReadNextByte(hexfile); //Läser in chksumma, orsakar fel om 3A (:)2
					} else {
						if( RecType == 1 ) {
							if( (ReadNextByte(hexfile)) != 255 ) {	
								m_status = "Error in last record. Last byte was not FF";
							} else {
								m_status = "File succesfully analysed";
							}
						} else {
							m_status = "Error reading HEX-file. Bad file format";
						}
					}
				}
			} else {
				m_status = "Could not open file";
			}
			fclose(hexfile);
		} else {
			m_status = "Path and file name field is empty";
		}
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

	if (!m_serialCom.Open(100, 100)) {
		m_status = "The selected COM-port could not initialize";
	} else {
		if (!m_serialCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		m_serialCom.Flush();

		m_status = "Saying Hello!";
		UpdateData(FALSE);
		WriteData[0] = 'H';
		m_serialCom.Write((LPCSTR)WriteData,1);
		m_status = "Waiting for response...";
		UpdateData(FALSE);

		m_serialCom.Read( (char *) ReadData, 14);
		ReadData[14] = 0;

		if (ReadData[0] == 0) {
			m_status = "Error: Link respond time-out";
		} else {			
			m_status.Format("Recivied this: '%s'", ReadData);
		}
		UpdateData(FALSE);
		m_serialCom.Close();
	}
}

void CSuperProgrammerDlg::OnFinalize() 
{
	BYTE				WriteData[2];

	if (!m_serialCom.Open(100, 100)) {
		m_status = "The selected COM-port could not initialize";
	} else {
		if (!m_serialCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		m_serialCom.Flush();

		m_status = "Finalyzing";
		UpdateData(FALSE);
		WriteData[0] = 'D';
		m_serialCom.Write((LPCSTR)WriteData,1);
		UpdateData(FALSE);
		m_serialCom.Close();
	}
	UpdateData(FALSE);
}

void CSuperProgrammerDlg::OnGethigh() 
{
	BYTE				WriteData[2];
	unsigned char		ReadData[80];
	FILE*				outfile;
	int					i;

	if (!m_serialCom.Open(100, 100)) {
		m_status = "The selected COM-port could not initialize";
	} else {
		if (!m_serialCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		m_serialCom.Flush();

		m_status = "Sending command";
		UpdateData(FALSE);
		WriteData[0] = 'B';
		m_serialCom.Write((LPCSTR)WriteData,1);
		m_status = "Waiting for response...";
		UpdateData(FALSE);

		m_serialCom.Read( (char *) ReadData, 64);
		ReadData[64] = 0;

		outfile = fopen("highBack.txt", "wt");

		for (i=0 ; i < 64 ; i++) {
			fprintf(outfile, "%4d %X\n", i, ReadData[i]);
		}

		fclose(outfile);

		m_status = "64 bytes received";
		UpdateData(FALSE);
		m_serialCom.Close();
	}
}

void CSuperProgrammerDlg::OnButton1() 
{
	BYTE				WriteData[2];
	unsigned char		ReadData[48];
	FILE			*hexfile;
	FILE			*outfile;
	unsigned char	DataLength, OffsetLowByte, OffsetHighByte;
	unsigned char	RecType=0;
	unsigned char	c;
	unsigned char	cs;
	int				n,Offset,l=0;
	bool			proceed = true;
	long			lastBlock;
	long			fileEnd;


	outfile = fopen("HighMem.txt", "wt");
	
	if (m_fileNPath == "") {
		proceed = false;
		m_status = "Path and file name field is empty";
	}
	if ((proceed) && ((hexfile = fopen(m_fileNPath, "rb")) == NULL)) {
		proceed = false;
		m_status = "Could not open file";
	}
	if ((proceed) && (!m_serialCom.Open(100, 100))) {
		proceed = false;
		m_status = "The selected COM-port could not initialize";
	}
	if ((proceed) && (!m_serialCom.SetUp())) {
		proceed = false;
		m_status = "Error setting up COM-port";
	}
	if (proceed) {
		
		m_serialCom.Flush();

		m_status = "Programming high memory";
		UpdateData(FALSE);
		
		WriteData[0] = 'P';
		m_serialCom.Write((LPCSTR)WriteData,1);
		m_serialCom.Read( (char *) ReadData, 1);
		if (ReadData[0] != 'P') {
			proceed = false;
			if (ReadData[0] == 0) {
				m_status = "Error: Link respond time-out";
			} else {
				m_status.Format("Recivied this: '%s'", ReadData);
			}
		}
	}
	if (proceed) {
		if( (ReadNextByte(hexfile)) != ':' ) {
			m_status = "Error reading HEX-file. No ':' found";
			RecType = 1;
		} else {
			m_status = "Reading Hex-file...";
		}



		//Programmera höga minnet

		m_status = "Programming high memory";
		UpdateData(FALSE);
		


		fseek(hexfile, 0, SEEK_END);
		fileEnd = ftell(hexfile);

		m_progress.SetRange32(0, fileEnd);

		fseek(hexfile, 0, 0);

		while( RecType == 0 ) {

			lastBlock = ftell(hexfile);

			m_progress.SetPos(lastBlock);

			l++;
			while((ReadNextByte(hexfile) != ':'));

			
			
			DataLength = ReadNextByte(hexfile);
			OffsetHighByte = ReadNextByte(hexfile);
			OffsetLowByte = ReadNextByte(hexfile);

			Offset = ( OffsetHighByte*256 ) + OffsetLowByte;

			if (Offset >= 0x8000) {

				WriteData[0] = 'F';
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = OffsetHighByte;
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = OffsetLowByte;
				m_serialCom.Write((LPCSTR)WriteData,1);
				WriteData[0] = DataLength;
				m_serialCom.Write((LPCSTR)WriteData,1);

				fprintf(outfile, "%02X%02X ", OffsetHighByte, OffsetLowByte);

			}

			RecType = ReadNextByte(hexfile);

			if( RecType == 0 ) {
				if (Offset >= 0x8000) {
					m_memMap.MarkProgrammed(Offset, Offset+DataLength);
					m_memMap.Invalidate(FALSE);
					m_memMap.UpdateWindow();;
					cs = 0xFF;
					for( n=0; n<DataLength; n++ ) {
						c = ReadNextByte(hexfile);
						cs ^= c;
						WriteData[0] = c;
						m_serialCom.Write((LPCSTR)WriteData,1);
						fprintf(outfile, "%02X ", c);
					}
					fprintf(outfile, "\n");
					WriteData[0] = cs;
					m_serialCom.Write((LPCSTR)WriteData,1);
				} else {
					for( n=0; n<DataLength; n++ ) {
						ReadNextByte(hexfile);
					}
				}
				ReadNextByte(hexfile); //Läser in chksumma, orsakar fel om 3A (:)2

				if (Offset >= 0x8000) {
					m_serialCom.Read( (char *) ReadData, 1);
					if (ReadData[0] == 'R') {
						fseek(hexfile, lastBlock, SEEK_SET);
						m_status = "Resending!!!!";
						UpdateData(FALSE);
					} else if (ReadData[0] != 'F') {
						m_status = "Illegal response from SuperComputer";
						proceed = false;
						RecType = 0;
					}
				}
			} else {
				if( RecType == 1 ) {
					if( (ReadNextByte(hexfile)) != 255 ) {	
						m_status = "Error in last record. Last byte was not FF";
						proceed = false;
					} else {
						m_status = "High succesfully programmed";
					}
				} else {
					m_status = "Error reading HEX-file. Bad file format";
					proceed = false;
				}
			}
		}
	}

	fclose(outfile);
	
	if (proceed) {
		
		//Programmera låga minnet

		WriteData[0] = 'L';
		m_serialCom.Write((LPCSTR)WriteData,1);

/*
		do {
			m_serialCom.Read( (char *) ReadData, 1);
		} while (ReadData[0] != 'L');

		proceed = true;
*/		
		
		m_serialCom.Read( (char *) ReadData, 1);
		if (ReadData[0] != 'L') {
			proceed = false;
			if (ReadData[0] == 0) {
				m_status = "Error: Link respond time-out";
			} else {
				m_status.Format("Recivied this: '%s'", ReadData);
			}
		}

  
	}
	if (proceed) {

		m_status = "Getting test low memory";
		UpdateData(FALSE);
		

		outfile = fopen("LowMemTest.txt", "wt");

		m_progress.SetPos(0);
		m_progress.SetRange32(0, 32767);

		for (n=0 ; n<16384 / 16 ; n++) {
			m_serialCom.Read( (char *) ReadData, 16);
			m_progress.SetPos(n);
			fprintf(outfile, "%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
				n, ReadData[0], ReadData[1], ReadData[2], ReadData[3], ReadData[4], ReadData[5], 
				ReadData[6], ReadData[7], ReadData[8], ReadData[9], ReadData[10], ReadData[11], 
				ReadData[12], ReadData[13], ReadData[14], ReadData[15]);
		}
		
		fclose(outfile);
	}

	m_status = "Backreading done";
	UpdateData(FALSE);

	m_serialCom.Close();
	UpdateData(FALSE);
}
