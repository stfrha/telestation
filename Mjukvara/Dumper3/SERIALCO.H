#ifndef _COMSERIALS
#define _COMSERIALS


#define ANTAL_COM 	4
#define ANTAL_BAUD	5
#define ANTAL_BIT	2
#define ANTAL_STOP	2
#define ANTAL_PARITY 3
#define IDC_BAUD_1200 	0
#define IDC_BAUD_2400 	1 
#define IDC_BAUD_4800	2
#define IDC_BAUD_9600	3
#define IDC_BAUD_19200  4    
#define IDC_BAUD_115200 5
#define BIT_7 		0
#define BIT_8		1 

class CSerialCom : public CObject
{
	public:
		CSerialCom();
		~CSerialCom(); 
		BOOL Open(int ReadCount,int WriteCount);//,int BaudRate=BAUD_9600,int Bits=BIT_8,
//						int StopBits=1,BOOL Paritet=FALSE);   
		BOOL SetUp();
		void Write(LPCSTR,int); 
		void WaitTXEmpty( void );
		void Read(LPSTR,int,int &);
		void Read(LPSTR,int);
		void Close(); 
		void Flush(); 
		char *GetCom(int i); 
		char *GetBaud(int i); 
		char *GetBit(int i); 
		char *GetStopBit(int i);
		char *GetParitet(int i);
		int GetBaudNr() {return m_nBaudNr;}
		int GetComNr() {return m_nComNr;}
		int GetBitNr() { return m_nBitNr;}
		int GetStopBitNr() { return m_nStopBitNr;}
		int GetParitetNr() {return m_nParityNr;}
		void SetBaudNr(int Baud) {m_nModified|=m_nBaudNr!=Baud; m_nBaudNr=Baud;}
		void SetBitNr( int Bit) {m_nModified|=m_nBitNr!=Bit; m_nBitNr=Bit;}
		void SetStopBitNr(int Bit) {m_nModified|=m_nStopBitNr!=Bit; m_nStopBitNr=Bit;}
		void SetParitet(int Par) {m_nModified|=m_nParityNr!=Par; m_nParityNr=Par;}
		BOOL SetComNr(int Com,BOOL Init=FALSE); 
		bool CharacterPending();
 
 	protected:
       int m_nComNr;
       int m_nBaudNr;
        int m_nBitNr;
        int m_nStopBitNr;
		int m_nParityNr;
        BOOL m_nModified;
        HANDLE m_nComHand;
        int m_nReadCount;
        int m_nWriteCount;
} ;
#endif
