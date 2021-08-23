#include "stdafx.h"  
char *ComData[] ={"COM1:","COM2:","COM3:","COM4:"};
char *BaudData[]={"1200","2400","4800","9600","19200","115200"};
char *BitData[]={"7","8"}; 
char *StopBitData[]={"1","2"}; 
char *ParitetData[]={"None","Even","Odd"};

#include "serialco.h"  



CSerialCom :: CSerialCom()
{         
	m_nComNr=2;   
	m_nComHand=INVALID_HANDLE_VALUE;
	m_nBaudNr=4;
	m_nParityNr=0;
	m_nBitNr=BIT_8;
	m_nStopBitNr=1;  
	m_nModified=TRUE;
}  

char* CSerialCom :: GetCom(int i) 
{
	return ComData[i];
}   
char* CSerialCom :: GetBaud(int i)
{	
	return BaudData[i];
} 
char* CSerialCom :: GetBit(int i)
{
	return BitData[i];
} 
char* CSerialCom :: GetStopBit(int i) 
{
	return StopBitData[i];
}
char* CSerialCom :: GetParitet(int i) 
{
	return ParitetData[i];
}

void CSerialCom :: Close()
{
	if(m_nComHand!=INVALID_HANDLE_VALUE) CloseHandle(m_nComHand);
	m_nComHand=INVALID_HANDLE_VALUE;
}
 
BOOL CSerialCom :: SetComNr(int Com, BOOL Init)
{   
	HANDLE OldComHand;
	int OldComNr;    
	BOOL Svar=TRUE;
	if(m_nComNr!=Com)
	{   
		OldComHand=m_nComHand;
		OldComNr=m_nComNr;
		m_nComNr=Com;
		if(!Init) Svar=Open(m_nReadCount,m_nWriteCount);
		if(Svar && OldComHand >=0) CloseHandle(OldComHand); 
		if(!Svar) { 
			m_nComNr=OldComNr;
			m_nComHand=OldComHand;
		}
		m_nModified=TRUE;
	}
	if(m_nComHand==INVALID_HANDLE_VALUE) return FALSE;  
	return Svar;
	
}
BOOL CSerialCom :: Open(int ReadCount,int WriteCount)
{   
	m_nReadCount=ReadCount;
	m_nWriteCount=WriteCount;
	m_nComHand=CreateFile(ComData[m_nComNr],GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ /*| FILE_SHARE_WRITE*/,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_OVERLAPPED*/,NULL);
	if(m_nComHand==INVALID_HANDLE_VALUE) {

		MessageBox(NULL, "Could not open serial port","Error",MB_OK | MB_ICONSTOP); 
		return FALSE;

	}   
	return TRUE;
}                                       

BOOL CSerialCom :: SetUp()
{
	char Command[100]; 
	char Par;
	Par=*ParitetData[m_nParityNr];
	DCB DCBcom;
	if(m_nComHand==INVALID_HANDLE_VALUE) return FALSE;
	if(m_nModified)
	{     
		sprintf(Command,"%s %s,%c,%s,%s",ComData[m_nComNr],BaudData[m_nBaudNr],Par,BitData[m_nBitNr],
					StopBitData[m_nStopBitNr]); 
		GetCommState(m_nComHand,&DCBcom);
		BuildCommDCB(Command,&DCBcom);
		DCBcom.fBinary=TRUE;
		DCBcom.fOutxDsrFlow=FALSE;
//		DCBcom.fOutX=TRUE;
//		DCBcom.fInX=TRUE;
		DCBcom.fOutX=FALSE;
		DCBcom.fInX=FALSE;
		DCBcom.fDtrControl=DTR_CONTROL_DISABLE;
		DCBcom.fRtsControl=RTS_CONTROL_DISABLE;	// DISABLE
//		DCBcom.fOutxCtsFlow=TRUE;				// Remove
		SetCommState(m_nComHand,&DCBcom);

		COMMTIMEOUTS	cto;

		cto.ReadIntervalTimeout = 500;   //2000
		cto.ReadTotalTimeoutMultiplier = 0;
		cto.ReadTotalTimeoutConstant = 500; //2000
		cto.WriteTotalTimeoutMultiplier = 0;
		cto.WriteTotalTimeoutConstant = 0;
		SetCommTimeouts(m_nComHand, &cto);
		
		Flush();  
		m_nModified=FALSE;

		EscapeCommFunction(m_nComHand, CLRDTR);
		EscapeCommFunction(m_nComHand, SETRTS);
	} 
	return TRUE;
}

CSerialCom :: ~CSerialCom()
{
	Close();
}

void CSerialCom :: Flush()
{
	FlushFileBuffers(m_nComHand);  
}
    
void CSerialCom :: Read(LPSTR Buffert,int Antal,int& ReadAntal)
{
	OVERLAPPED OverL;
	DWORD ACount;
	char ch;
	int i;
	OverL.hEvent=NULL;
	OverL.Offset=0;
	OverL.OffsetHigh=0;
	for(i=0,ch=0;i < min(Antal,m_nReadCount) && ch!='\n';i++)
	{
		ReadFile(m_nComHand,&Buffert[i],1,&ACount,&OverL);
		GetOverlappedResult(m_nComHand,&OverL,&ACount,TRUE);
		ch=Buffert[i];
	}
	Buffert[i]=0;
	ReadAntal=i;

}   

void CSerialCom :: Read(LPSTR Buffert,int Antal)
{
	DWORD ACount;
	ReadFile(m_nComHand,Buffert,min(Antal,m_nReadCount),&ACount,NULL);
	Buffert[ACount]=0;

}   

void CSerialCom :: Write(LPCSTR Buffert,int Antal)
{
	DWORD	ACount;

//	SetCommMask(m_nComHand, EV_RXCHAR);	//EV_RXCHAR

//	EscapeCommFunction(m_nComHand, SETRTS);
	WriteFile(m_nComHand,Buffert,min(Antal,m_nWriteCount),&ACount,NULL);
//	WaitCommEvent(m_nComHand, &Event, NULL);
//	EscapeCommFunction(m_nComHand, CLRRTS);
}


bool CSerialCom :: CharacterPending( void )
{
	COMSTAT			MyCommStat;
	unsigned long	Dummy;

	ClearCommError(m_nComHand, &Dummy, &MyCommStat);
	
	if (MyCommStat.cbInQue >= 1) return true;
	else return false;
}





