// Dumper3Dlg.cpp : implementation file
//strlen

#include "stdafx.h"
#include "Dumper3.h"
#include "Dumper3Dlg.h"
#include "ComSelect.h"

#include "SERIALCO.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




int BuildVektor(CString, unsigned char [], CStatic*);
unsigned char ReadNextByte(FILE *);


const int maxblocks = 512;

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
// CDumper3Dlg dialog

CDumper3Dlg::CDumper3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDumper3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDumper3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDumper3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDumper3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDumper3Dlg, CDialog)
	//{{AFX_MSG_MAP(CDumper3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_DUMP, OnDump)
	ON_BN_CLICKED(IDC_CONNTEST, OnConntest)
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	ON_BN_CLICKED(IDC_BACKLOAD, OnBackload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDumper3Dlg message handlers

BOOL CDumper3Dlg::OnInitDialog()
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

void CDumper3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDumper3Dlg::OnPaint() 
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
HCURSOR CDumper3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDumper3Dlg::OnBrowse() 
{
	CFileDialog	MyFDlg(TRUE, "A03", "*.A03", OFN_HIDEREADONLY, "Hex files (*.A03) |*.A03| All Files (*.*) |*.*||", NULL); ;
	CEdit*		PathEdit;
	CStatic*			Status;	
	
	PathEdit = (CEdit*) GetDlgItem(IDC_PATH);
	Status = (CStatic*) GetDlgItem(IDC_STATUS);

	if (MyFDlg.DoModal() == IDOK) {
		PathEdit->SetWindowText(MyFDlg.GetPathName());
		Status->SetWindowText("File selected");
	}
	
}

void CDumper3Dlg::OnDump() 
{
	BYTE				WriteData[2];
	unsigned char		ReadData[15];
	unsigned char		Vektor[64 * maxblocks + 10];
	unsigned char		Verify[64 * maxblocks + 10];
	CString				Path;
	CEdit*				PathEdit;
	CStatic*			Status;	
	CProgressCtrl*		Progress;
	int 				n, i = 0;
	char 				msg[40];
	bool				check;
	bool				errn = false;


	Progress = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);
	Status = (CStatic*) GetDlgItem(IDC_STATUS);

	if (!MySerCom.Open(100, 100)) {
		Status->SetWindowText("The selected COM-port could not initialize");
	} else {

		if (!MySerCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		CWaitCursor	WQ;

		PathEdit = (CEdit*) GetDlgItem(IDC_PATH);

		MySerCom.Flush();
	
		PathEdit->GetWindowText( Path ); 
		if (BuildVektor(Path, Vektor, Status) == 1) {
		
//######################################################################
// Send Vector
		
			Status->SetWindowText("Sending Hex-file to EEPROM...");

			WriteData[0] = 0x56;
			MySerCom.Write((LPCSTR)WriteData,1);

			MySerCom.Read( (char *) ReadData, 1);

			if( ReadData[0] != 0x56 ) {
				if (ReadData[0] == 0) {
					Status->SetWindowText("Error: Link response time-out");
				} else {
					sprintf(msg, "Error: Illegal respons from link: %d\n", ReadData[0]);
					Status->SetWindowText(msg);
				}
			} else {

				Progress->SetPos(0);
				Progress->SetRange(0, maxblocks * 2);

				for(n=0; ((n < maxblocks) && (!errn)); n++) {		

					sprintf(msg, "Sending block %d of %d...\n", n, maxblocks);
					Status->SetWindowText(msg);

					Progress->SetPos(n);
					
					MySerCom.Write((LPCSTR) (Vektor + (64 * n)), 64);
				
					MySerCom.Read( (char *) ReadData, 1);

					if( ReadData[0] != 0x56 ) {
						switch (ReadData[0]) {
							case 0:
								Status->SetWindowText("Error: Link response time-out");
								break;
							case 255:
								Status->SetWindowText("Error writing to EEPROM. Is it connected?");
								break;
							default:
								sprintf(msg, "Error: Illegal respons from link: %d\n", ReadData[0]);
								Status->SetWindowText(msg);
						};
						errn = true;		// Cause loop to exit
					}

				}
				if( !errn ) {

					Status->SetWindowText("HEX-file succesfully dumped to prototype board");

					WriteData[0] = 0x57;
					MySerCom.Write((LPCSTR)WriteData,1);
	
					for (n=0 ; n < maxblocks ; n++) {

						sprintf(msg, "Receiving block: %d of %d\n", n, maxblocks);
						Status->SetWindowText(msg);

						Progress->SetPos(n + maxblocks);
	
						MySerCom.Read( (char *) (Verify + (n * 64)), 64);

/*						if (PeekMessage(&MyMsg, NULL, 0, 0, PM_REMOVE)) {
							TranslateMessage(&MyMsg);
							DispatchMessage(&MyMsg);
						}
*/
					}

					Status->SetWindowText("Testing received data against sent data...");

					check = TRUE;

					for (n=0 ; (n < (maxblocks * 64)) && (check == TRUE); n++) {
						if (Vektor[n] != Verify[n]) check = FALSE;
					}

					if (check) {
						Status->SetWindowText("Data successfully written to EEPROM");
					} else {
						sprintf(msg, "Verification error at address: %d\n", (n-1));
						Status->SetWindowText(msg);
					}
				}
				
			}
		} else {
			MessageBox("The specified file could not be opened", "Open file error", MB_OK | MB_ICONSTOP);
		}

		MySerCom.Close();
	}

}



/******************** BuildVektor ***********************************/

/*
	Funktionen läser in Hex-filen på Intel standard format till en vektor
	där position noll i vektorn motsvarar adress noll i EEPROMET.
	Vektorn är 32768 bytes lång. Vektorn fylls med FF varefter datat i
	hex-filen läses in till rätt plats i vektorn.
*/


int BuildVektor(CString Path, unsigned char Vektor[], CStatic* status)
{
	FILE			*hexfile;
	unsigned char	DataLength, OffsetLowByte, OffsetHighByte;
	unsigned char	RecType=0;
	int				k,n,Offset,l=0;

	for(k=0;k<maxblocks * 64;k++) {    //Fyller vektor med FF
		Vektor[k]=0xFF;
	}

	if (Path.GetLength() != 0) {

		hexfile = fopen(Path, "rb");
		if (hexfile != NULL) {
			if( (ReadNextByte(hexfile)) != ':' ) {
				status->SetWindowText("Error reading HEX-file. No ':' found");
				RecType = 1;
			} else {
				status->SetWindowText("Reading Hex-file...");
			}

			fseek(hexfile, 0, 0);
			while( RecType == 0 ) {

				l++;
				while((ReadNextByte(hexfile) != ':')) {;}
				DataLength = ReadNextByte(hexfile);
				OffsetHighByte = ReadNextByte(hexfile);
				OffsetLowByte = ReadNextByte(hexfile);
				Offset = ( OffsetHighByte*256 ) + OffsetLowByte;
				RecType = ReadNextByte(hexfile);

				if( RecType == 0 ) {
					for( n=0; n<DataLength; n++ ) {
						if (Offset < 32768) Vektor[Offset] = ReadNextByte(hexfile);
						Offset++;
					}
					ReadNextByte(hexfile); //Läser in chksumma, orsakar fel om 3A (:)2
				} else {
					if( RecType == 1 ) {
						if( (ReadNextByte(hexfile)) != 255 ) {	
							status->SetWindowText("Error in last record. Last byte was not FF");
						} else {
							status->SetWindowText("File succesfully read into vector");
						}
					} else {
						status->SetWindowText("Error reading HEX-file. Bad file format");
					}
				}
			}
		} else {
			status->SetWindowText("Could not open file");
			return 0;
		}
		fclose(hexfile);
		return 1;
	} else {
		status->SetWindowText("Path and file name field is empty");
		return 0;
	}
}



/****************** Funktion som läser nästa byten i hex-filen *******/
/*
	Funktionen läser två tecken vilka representerar en hexadecimal byte och
	gör om dessa till en unsigned char.
*/


unsigned char ReadNextByte(FILE *hexfile)
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





void CDumper3Dlg::OnConntest() 
{
	// TODO: Add your control notification handler code here
//	CSerialCom			MySerCom;	
	BYTE				WriteData[2];
	unsigned char		ReadData[15];
	CStatic*			Status;	
	char 				msg2[40];


	Status = (CStatic*) GetDlgItem(IDC_STATUS);
	
	if (!MySerCom.Open(100, 100)) {
		Status->SetWindowText("The selected COM-port could not initialize");
//		MessageBox("The selected COM-port could not initialize", "Open COM-port error", MB_OK | MB_ICONSTOP);
	} else {

		if (!MySerCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		MySerCom.Flush();

		Status->SetWindowText("Saying Hello!");
		WriteData[0] = 85;
//		WriteData[1] = 85;
		MySerCom.Write((LPCSTR)WriteData,1);
		Status->SetWindowText("Waiting for response...");

		MySerCom.Read( (char *) ReadData, 6);

		if (ReadData[0] == 0) {
			Status->SetWindowText("Error: Link respond time-out");
		} else {			
			sprintf(msg2, "Recivied this: '%s'", ReadData);
			Status->SetWindowText(msg2);
		}
		MySerCom.Close();
	}
}

void CDumper3Dlg::OnSetup() 
{
	CStatic*	Status;	
	int			OldCom;

	Status = (CStatic*) GetDlgItem(IDC_STATUS);


	OldCom = MySerCom.GetComNr();

	CComSelect	MySelDlg(this, OldCom);
	
	if (MySelDlg.DoModal() == IDOK) {
		MySerCom.SetComNr(MySelDlg.m_ComState, TRUE);
	};
}

void CDumper3Dlg::OnBackload() 
{
	FILE*				file;
	BYTE				WriteData[2];
	unsigned char		ReadData[15];
	unsigned char		Verify[64 * maxblocks + 10];
	CStatic*			Status;	
	CProgressCtrl*		Progress;
	int 				n, i = 0;
	char 				msg[40];
	bool				errn = false;


	Progress = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);
	Status = (CStatic*) GetDlgItem(IDC_STATUS);

	Progress->SetPos(0);
	Progress->SetRange(0, maxblocks);

	if (!MySerCom.Open(100, 100)) {
		Status->SetWindowText("The selected COM-port could not initialize");
	} else {

		if (!MySerCom.SetUp()) {
			MessageBox("Error setting up COM-port", "Open COM-port error", MB_OK | MB_ICONSTOP);
		}

		CWaitCursor	WQ;

		MySerCom.Flush();


		if( !errn ) {

			Status->SetWindowText("HEX-file succesfully dumped to prototype board");

			WriteData[0] = 0x57;
			MySerCom.Write((LPCSTR)WriteData,1);

			for (n=0 ; n < maxblocks ; n++) {

				sprintf(msg, "Receiving block: %d of %d\n", n, maxblocks);
				Status->SetWindowText(msg);

				Progress->SetPos(n);

				MySerCom.Read( (char *) (Verify + (n * 64)), 64);

			}

			Status->SetWindowText("Writing received data to hex.txt...");

			file = fopen("hex.txt", "wt");

			for (n=0 ; n < maxblocks * 64 ; n++) {
				fprintf(file, "%X\t%X\n", n, Verify[n]);
			}

			fclose(file);
				
			Status->SetWindowText("Backload done");

		}
		MySerCom.Close();
	}
}
