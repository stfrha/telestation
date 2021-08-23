// TermEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProgrammer.h"
#include "TermEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermEdit

CTermEdit::CTermEdit()
{
	m_timer = 0;
}

CTermEdit::~CTermEdit()
{
//	StopTimer();
}


BEGIN_MESSAGE_MAP(CTermEdit, CEdit)
	//{{AFX_MSG_MAP(CTermEdit)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_CONTROL_REFLECT(EN_ERRSPACE, OnErrspace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTermEdit::SetSerialCom(CSerialCom* serCom)
{
	m_serialCom = serCom;
}

void CTermEdit::SetActive(BOOL* active)
{
	m_active = active;
}

void CTermEdit::SetHexMonitor(BOOL* hexMonitorOn)
{
	m_hexMonitor = hexMonitorOn;
}

void CTermEdit::StartTimer(void)
{
	m_timer = SetTimer(1, 50, NULL);
}

void CTermEdit::StopTimer(void)
{
	if (m_timer) {
		KillTimer(m_timer);
		m_timer = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTermEdit message handlers

void CTermEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char	buff[2];

	buff[0] = nChar;
	buff[1] = 0;

	if (m_active) {
		m_serialCom->Write(buff, 1);
	}


	//CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CTermEdit::OnTimer(UINT nIDEvent) 
{
	char	rBuff[256];
	char	sBuff[7];
	int		l, i;
	int		currStart;
	int		currEnd;
	int		c;

	if (l = m_serialCom->CharacterPending()) {
		if (l > 255) l = 255;
		m_serialCom->Read(rBuff, l);
		for (i=0 ; i<l ; i++) {
			c = rBuff[i] & 0xFF;
			if (c == '\b') {
				if (*m_hexMonitor) {
					GetSel(currStart, currEnd);
					SetSel(currStart, currStart-4, FALSE);
					Clear();
				} else {
					GetSel(currStart, currEnd);
					SetSel(currStart, currStart-1, FALSE);
					Clear();
				}
				i += 2;					//Hoppa över ' \b'
			} else if (*m_hexMonitor) {
				sprintf(sBuff, "<%02X>", c);
				ReplaceSel(sBuff);
			} else if (c == '\r') {
				PutChar('\r');
			} else if (c == '\n') {
				PutChar('\n');
			} else if (c == '\t') {
				PutChar('\t');
			} else if (c < ' ') {
				sprintf(sBuff, "<%02X>", c);
				ReplaceSel(sBuff);
			} else {
				PutChar(c);
			}
		}
	}

	CEdit::OnTimer(nIDEvent);
}

/*
				if (LineIndex(GetLineCount()) >= MAX_CHARS) {

					GetSel(currStart, currEnd);

					SetSel(0, 1, TRUE);
					SetSel(0, LineLength(0)+2, TRUE);
					Clear();

					SetSel(currStart, currEnd, FALSE);
				}
*/


void CTermEdit::PutChar(char c)
{
	char	sBuff[10];
	int		currStart;
	int		currEnd;
	CString	s;


	sBuff[0] = c;
	sBuff[1] = 0;

	ReplaceSel(sBuff);

	if ( GetLineCount() >= MAX_LINES) {
		GetSel(currStart, currEnd);

		SetSel(0, 1, TRUE);
		SetSel(0, LineLength(0)+2, TRUE);
		Clear();

		SetSel(currStart, currEnd, FALSE);
	}
	
/*
	l = GetWindowTextLength();
	if (l >= MAX_CHARS) {
		GetSel(currStart, currEnd);

		SetSel(0, 1, TRUE);
		SetSel(0, LineLength(0)+2, TRUE);
		Clear();

		SetSel(currStart, currEnd, FALSE);
	}
*/
}


void CTermEdit::OnErrspace() 
{
	// TODO: Add your control notification handler code here
	
}
