// ScriptSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDIBitmap.h"
#include "ScriptSimulatorDlg.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SS_NONE				0x00
#define	SS_WAIT_CALL		0x01
#define	SS_WAIT_SECOND_RING	0x02
#define	SS_OK_SIGNALS		0x03
#define	SS_AQ_PLAYING		0x04
#define	SS_AQ_WAIT_ANS		0x05




/////////////////////////////////////////////////////////////////////////////
// CScriptSimulatorDlg dialog


CScriptSimulatorDlg::CScriptSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptSimulatorDlg)
	m_simulatorOn = FALSE;
	m_callerNumber = _T("");
	//}}AFX_DATA_INIT

	m_scriptDoc = NULL;
	m_projectDoc = NULL;
	m_currentSimObj = NULL;
	m_timer = 0;
	m_simState = SS_NONE;
	m_hungUp = FALSE;
}


void CScriptSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptSimulatorDlg)
	DDX_Control(pDX, IDC_HANG_UP, m_hangUp);
	DDX_Control(pDX, IDC_MAKE_CALL, m_makeCall);
	DDX_Check(pDX, IDC_SIMULATOR_ON, m_simulatorOn);
	DDX_Text(pDX, IDC_CALLER_NUMBER, m_callerNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptSimulatorDlg, CDialog)
	//{{AFX_MSG_MAP(CScriptSimulatorDlg)
	ON_BN_CLICKED(IDC_SIMULATOR_ON, OnSimulatorOn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MAKE_CALL, OnMakeCall)
	ON_BN_CLICKED(IDC_HANG_UP, OnHangUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptSimulatorDlg message handlers

BOOL CScriptSimulatorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_makeCall.EnableWindow(FALSE);
	m_hangUp.EnableWindow(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScriptSimulatorDlg::OnCancel() 
{

	if (m_scriptDoc) {
		m_scriptDoc->UpdateAllViews(NULL);
	}

	DestroyWindow();
}

void CScriptSimulatorDlg::OnSimulatorOn() 
{
	UpdateData(TRUE);
	if (m_simulatorOn) {
		if (!m_scriptDoc) {

		} else {
			m_currentSimObj = FindStartObject();

			if (!m_currentSimObj) {
				MessageBox("Could not find 'start'-label", "Error in script");
				m_simulatorOn = FALSE;
				UpdateData(FALSE);
			} else {
				m_scriptDoc->UpdateAllViews(NULL);
				m_hungUp = FALSE;
				PerformObject();
			}
		}
	} else {
		if (m_scriptDoc) {
			m_scriptDoc->UpdateAllViews(NULL);
			if (m_timer) KillTimer(m_timer);
			m_timer = 0;
			m_simState = SS_NONE;
			m_makeCall.EnableWindow(FALSE);
			m_hangUp.EnableWindow(FALSE);
		}
		m_currentSimObj = NULL;
	}
}

/*
CScriptObject*	CScriptSimulatorDlg::GetCurrentSimObject( void )
{
	if (m_scriptDoc) {
		return m_scriptDoc->m_objectList.GetHead();
	}
	MessageBox("m_scriptDoc = NULL", "Error", MB_OK);
	return NULL;

}
*/

void CScriptSimulatorDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();

	if (m_projectDoc) {
		m_projectDoc->InvalidateSimulatorDoc();
	}

	delete this;
}

void CScriptSimulatorDlg::InvalidateScriptDoc( void )
{
	if (m_scriptDoc) m_scriptDoc = NULL;
	m_simulatorOn = FALSE;
	UpdateData(FALSE);
}

void CScriptSimulatorDlg::InvalidateProjectDoc( void )
{
	m_projectDoc = NULL;
}

BOOL CScriptSimulatorDlg::DestroyWindow() 
{
	if (m_projectDoc) {
		m_projectDoc->InvalidateSimulatorDoc();
	}
	
	return CDialog::DestroyWindow();
}

CScriptObject* CScriptSimulatorDlg::FindStartObject( void )
{
	BOOL	noFromHereList;

	if (!m_scriptDoc) return NULL;

	CScriptObject*	tmpObj;

	POSITION pos = m_scriptDoc->m_objectList.GetHeadPosition();
	while (pos != NULL) {
		tmpObj = m_scriptDoc->m_objectList.GetNext(pos);
		if ((tmpObj->GetType() == OT_LABEL) && (tmpObj->GetLabel() == "start")) {
			if (tmpObj->m_fromHereList.IsEmpty()) noFromHereList = TRUE;
			else return tmpObj;
		}
	}

	if (noFromHereList) MessageBox("A 'start'-label exists but\nit has no transition from it", "Error");

	return NULL;
}

void CScriptSimulatorDlg::PerformObject( void )
{
	CString	msgPath;

	if (m_currentSimObj) {
		switch (m_currentSimObj->GetType() )  {
		case OT_LIFT_PHONE:
		case OT_DROP_PHONE:
		case OT_REC_MSG:
		case OT_ERASE_ALL:
		case OT_ANSWER_ON:
		case OT_ANSWER_OFF:
		case OT_PLAY_ALL:
		case OT_LABEL:
			if (m_timer) KillTimer(m_timer);
			m_timer = SetTimer(1, 500, NULL);
			if (!m_timer) MessageBox("Error creating timer", "Error");
			break;
		case OT_PLAY_FILE:
			if (m_timer) KillTimer(m_timer);

			msgPath = "";

			if (m_projectDoc) {
				msgPath = m_projectDoc->GetAnnouncementPath(m_currentSimObj->GetAnnouncement());
				PlaySound(msgPath, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			}
			m_timer = SetTimer(1, GetSoundDuration(msgPath), NULL);
			if (!m_timer) MessageBox("Error creating timer", "Error");
			break;
		case OT_WAIT_CALL:
			m_simState = SS_WAIT_CALL;
			m_makeCall.EnableWindow(TRUE);
			m_hangUp.EnableWindow(FALSE);
			break;
		case OT_ASK_QUESTION:
			if (m_timer) KillTimer(m_timer);

			msgPath = "";

			if (m_projectDoc) {
				msgPath = m_projectDoc->GetAnnouncementPath(m_currentSimObj->GetAnnouncement());
				PlaySound(msgPath, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			}
			m_simState = SS_AQ_PLAYING;
			m_timer = SetTimer(1, GetSoundDuration(msgPath), NULL);
			if (!m_timer) MessageBox("Error creating timer", "Error");
			break;

		}
	}
}

void CScriptSimulatorDlg::GotoNextObject( void )
{

	if (m_currentSimObj) {
		switch (m_currentSimObj->GetType() )  {
		case OT_LIFT_PHONE:
		case OT_DROP_PHONE:
		case OT_REC_MSG:
		case OT_ERASE_ALL:
		case OT_ANSWER_ON:
		case OT_ANSWER_OFF:
		case OT_PLAY_ALL:
		case OT_LABEL:
			if (m_timeout) {
				if (m_currentSimObj->m_fromHereList.IsEmpty()) {
					m_currentSimObj = FindLastLabel(m_currentSimObj->GetLabel() );
				} else if (m_currentSimObj->m_fromHereList.GetCount() > 1) {
					MessageBox("Multiple transitions from this object is not allowed", "Error in script");
					m_currentSimObj = NULL;
				} else {
					m_currentSimObj = m_currentSimObj->m_fromHereList.GetHead()->m_dstObject;
				}
				if (!m_currentSimObj) {
				MessageBox("Could not find exit transition", "Script terminated");
					TerminateSimulation();
				}

			}
			break;
		case OT_PLAY_FILE:
			if (m_currentSimObj->m_fromHereList.IsEmpty()) {
				m_currentSimObj = NULL;
			} else if (m_currentSimObj->m_fromHereList.GetCount() > 1) {
				MessageBox("Multiple transitions from play file is not allowed", "Error in script");
				m_currentSimObj = NULL;
			} else {
				m_currentSimObj = m_currentSimObj->m_fromHereList.GetHead()->m_dstObject;
			}
			if (!m_currentSimObj) {
				MessageBox("Could not find exit transition", "Script terminated");
				TerminateSimulation();
			}

			break;
		case OT_WAIT_CALL:
			if (m_currentSimObj->m_fromHereList.IsEmpty()) {
				m_currentSimObj = NULL;
			} else {

				CTransition*	tmpTrans;
				CScriptObject*	okSignals = NULL;;
				CScriptObject*	done = NULL;
				CScriptObject*	timeout = NULL;

				POSITION pos = m_currentSimObj->m_fromHereList.GetHeadPosition();
				while (pos) {
					tmpTrans = m_currentSimObj->m_fromHereList.GetNext(pos);

					if (tmpTrans->m_transitionType == TT_OK_SIGNALS) {
						okSignals = tmpTrans->m_dstObject;
					} else if (tmpTrans->m_transitionType == TT_DEFAULT) {
						done = tmpTrans->m_dstObject;
					} else if (tmpTrans->m_transitionType == TT_TIMEOUT) {
						timeout = tmpTrans->m_dstObject;
					}
				}

				if (m_simState == SS_OK_SIGNALS) {
					if (okSignals) m_currentSimObj = okSignals;
					else m_currentSimObj = done;
				}

				if (m_timeout) {
					if (timeout) m_currentSimObj = timeout;
					else m_currentSimObj = done;
				}
			}

			m_simState = SS_NONE;
				
			if (!m_currentSimObj) {
				MessageBox("Could not find exit transition", "Script terminated");
				TerminateSimulation();
			}

			break;
		}
		if (m_scriptDoc) {
			m_scriptDoc->UpdateAllViews(NULL);
		}
	}
	if (m_currentSimObj) {
		PerformObject();
	}	
}


void CScriptSimulatorDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent = m_timer) {

		KillTimer(m_timer);
		m_timer = 0;

		if (m_currentSimObj) {
			switch (m_currentSimObj->GetType()) {
			case OT_PLAY_FILE:
				m_timeout = TRUE;
				GotoNextObject();
				break;
			case OT_WAIT_CALL:
				if (m_simState == SS_WAIT_SECOND_RING) {
					m_simState = SS_OK_SIGNALS;
					PlaySound("ringin.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
					m_timeout = FALSE;
				}
				GotoNextObject();
				break;
			case OT_LIFT_PHONE:
			case OT_DROP_PHONE:
			case OT_REC_MSG:
			case OT_ERASE_ALL:
			case OT_ANSWER_ON:
			case OT_ANSWER_OFF:
			case OT_PLAY_ALL:
			case OT_LABEL:
				m_timeout = TRUE;
				GotoNextObject();
				break;
			case OT_ASK_QUESTION:
				if (m_simState == SS_AQ_PLAYING) {
					m_timer = SetTimer(1, 30000, NULL);
					if (!m_timer) MessageBox("Error creating timer", "Error");
					m_simState = SS_AQ_WAIT_ANS;
				} else if (m_simState == SS_AQ_WAIT_ANS) {
					m_timeout = TRUE;
					GotoNextObject();
				}
				break;
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}




void CScriptSimulatorDlg::TerminateSimulation( void )
{
	m_simulatorOn = FALSE;
	UpdateData(FALSE);
	if (m_scriptDoc) {
		m_scriptDoc->UpdateAllViews(NULL);
	}
}

CScriptObject* CScriptSimulatorDlg::FindLastLabel(CString label)
{
	CScriptObject*	tmpObj;

	POSITION pos = m_scriptDoc->m_objectList.GetHeadPosition();
	while (pos != NULL) {
		tmpObj = m_scriptDoc->m_objectList.GetNext(pos);
		if ((tmpObj->GetType() == OT_LABEL) && (tmpObj->GetLabel() == label)) {
			if (!tmpObj->m_fromHereList.IsEmpty()) return tmpObj;
		}
	}
	return NULL;
}


void CScriptSimulatorDlg::EnableOnSwitch(BOOL enable)
{
	CButton*	myOnSwitch = (CButton*) GetDlgItem(IDC_SIMULATOR_ON);
	
	myOnSwitch->EnableWindow(enable);
}




void CScriptSimulatorDlg::OnMakeCall() 
{
	UpdateData(TRUE);

	m_makeCall.EnableWindow(FALSE);

	int i;

	for (i=0 ; i<m_callerNumber.GetLength() ; i++) {
		switch (m_callerNumber[i]) {
		case '0':
			PlaySound("dtmf0.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '1':
			PlaySound("dtmf1.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '2':
			PlaySound("dtmf2.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '3':
			PlaySound("dtmf3.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '4':
			PlaySound("dtmf4.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '5':
			PlaySound("dtmf5.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '6':
			PlaySound("dtmf6.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '7':
			PlaySound("dtmf7.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '8':
			PlaySound("dtmf8.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '9':
			PlaySound("dtmf9.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '*':
			PlaySound("dtmfstar.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		case '#':
			PlaySound("dtmfpound.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
			break;
		}
	}
	PlaySound("ringin.wav", NULL, SND_FILENAME | SND_SYNC | SND_NOSTOP);
	if (m_simState == SS_WAIT_CALL) {
		if (m_timer) KillTimer(m_timer);
		m_timer = SetTimer(1, 4000, NULL);
		if (!m_timer) MessageBox("Error creating timer", "Error");
		m_simState = SS_WAIT_SECOND_RING;
	}
	m_hangUp.EnableWindow(TRUE);

}


void CScriptSimulatorDlg::OnHangUp() 
{
	if (m_simState == SS_WAIT_SECOND_RING) {
		KillTimer(m_timer);
		m_timer = 0;
		m_timeout = TRUE;
		m_hangUp.EnableWindow(FALSE);
		GotoNextObject();
	} else {
		if (m_currentSimObj) {
			switch (m_currentSimObj->GetType()) {
			case OT_PLAY_FILE:
				PlaySound(NULL, NULL, NULL);
				KillTimer(m_timer);
				m_timer = 0;
				m_hungUp = TRUE;
				GotoNextObject();
				break;
			case OT_ASK_QUESTION:
				if (m_simState == SS_AQ_PLAYING) {
					PlaySound(NULL, NULL, NULL);
				}
				KillTimer(m_timer);
				m_timer = 0;
				m_hungUp = TRUE;
				GotoNextObject();
				break;
			}
		}
	}
}

UINT	CScriptSimulatorDlg::GetSoundDuration(CString path)
{
	FILE*		fp;
	char		srstr[5];
	UINT		byteLen;
	long		formatLen;
	long		avgBytePerSec;


	fp = fopen(path, "rb");
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
		
		formatLen = fgetc(fp);
		formatLen += fgetc(fp) << 8;
		formatLen += fgetc(fp) << 16;
		formatLen += fgetc(fp) << 24;

		/*m_formatTag = */fgetc(fp);
		/*m_formatTag += */fgetc(fp) /*<< 8*/;

		/*m_nChannels = */fgetc(fp);
		/*m_nChannels += */fgetc(fp) /*<< 8*/;

		/*m_samplePerSec = */fgetc(fp);
		/*m_samplePerSec += */fgetc(fp) /*<< 8*/;
		if (formatLen >= 16) {
		/*	m_samplePerSec += */fgetc(fp) /*<< 16*/;
		/*	m_samplePerSec += */fgetc(fp) /*<< 24*/;
		}

		avgBytePerSec = fgetc(fp);
		avgBytePerSec += fgetc(fp) << 8;
		if (formatLen >= 16) {
			avgBytePerSec += fgetc(fp) << 16;
			avgBytePerSec += fgetc(fp) << 24;
		}


//		m_bitPerSample = 8 * m_avgBytePerSec / (m_nChannels * m_samplePerSec);

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
		
		byteLen = fgetc(fp);
		byteLen += fgetc(fp) << 8;
		byteLen += fgetc(fp) << 16;
		byteLen += fgetc(fp) << 24;

		fclose(fp);

		return 1000 * byteLen / avgBytePerSec;
	}

	return 0;
}

