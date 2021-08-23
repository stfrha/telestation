/*************************************************************
Fil:            $Id: main.c,v 1.7 2004/09/19 16:05:59 Gemensam Exp $

Beskrivning:    Bootning och kommandotolk f�r TeleStation applikationen

Skapad:         99-09-19

�ndrad:         $Date: 2004/09/19 16:05:59 $

�gare:          Fredrik Hoffman


Att g�ra:

*   Fel felmeddelande 0x14 om man skriver TYPE (utan filnamn) eftersom nollan 
    i parameter tv� st�mmer med nollan i filnamnet p� en ledig  filstruktur, fixa!

Revisionshistoria:
				$Log: main.c,v $
				Revision 1.7  2004/09/19 16:05:59  Gemensam
				Split initialization in HW-specific and Application specific in case application goes horrobly wrong and freezes all.
				
				Revision 1.6  2003/11/24 22:44:36  Gemensam
				Fixed PPD, not working yet,
				New command: speaker volume.
				Version 0301.
				
				Revision 1.5  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.4  2003/11/18 19:17:35  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.3  2003/11/13 22:35:44  Gemensam
				Added watchdog reset at GotoLabel, FindLabel and InsertLabel.
				Added global variable to contain speaker volume, Default volume set to 50.
				Version 1.1
				
				Revision 1.2  2003/11/10 22:44:46  Gemensam
				Added header information
				

*************************************************************/
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "commands.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "Phone.h"
#include "Calender.h"
#include "DPOT.h"
#include "main.h"
#include "RS232.h"
#include "WAV.h"
#include "DTMF.h"
#include "Phone.h"
#include "Timer.h"
#include "PhoneAns.h"
#include "LCD.h"
#include "Keyboard.h"
#include "Logfile.h"


//////////////////////
// Globala variabler

extern xdata int	lastPlayed;			//Kronologindexerad
extern xdata char	receivedDTMF;		//Inneh�ller den senaste mottagna DTMF-signalen
										//fr�n avbrottsrutinen. Om inget tagits emot s�tts
										//den till 0. F�r att f� ett anv�ndbart v�rde skall
										//funktionen DecodeDTMF() k�ras.
extern xdata char	answerOn;
extern xdata char   callerCategory;
extern xdata char   numSignals;
extern xdata char	speakerVolume;

xdata char			commandLine[CM_PARSIZE][CM_NRPAR];	//H�r lagras kommandot och alla parametrar
										//Varje kommando/parameter f�r vara max cmParSize tecken
										//Till varje kommando kan cmNrPar-1 kopplas
xdata int			nrCommandParameters;//H�r anges antal str�ngar kommandot bygger upp (dvs
														//antal parametrar + 1 f�r kommandot
xdata int			error;				//Error-kod, se definitioner ovan
xdata char			msg[100];
xdata char			timeout;			//Tidsm�tningsvariabel
xdata FILE*			batFilePtr;
xdata char			batLabelName[MAX_LABELS][MAX_LABEL_LEN];
xdata long			batLabelPos[MAX_LABELS];

// Nedan var ett f�rs�k till multipla bat-filer, kanske inf�rs senare
/*
BatFileObj* currentBatFile;
BatFileObj  batFiles[MAX_BATFILES];
Label       labelList0[MAX_LABELS];
Label       labelList1[MAX_LABELS];
Label       labelList2[MAX_LABELS];
Label       labelList3[MAX_LABELS];
Label       labelList4[MAX_LABELS];
Label       labelList5[MAX_LABELS];
Label       labelList6[MAX_LABELS];
Label       labelList7[MAX_LABELS];
*/


//////////////////////
// Lokala variabler


//////////////////////
// Kod

void main( void ) 
{   
    xdata char    timer;

//-------------------------------------------
// Initialisering av serieport 1 (RS232-porten)

    InitializeRS232();

    if ((IP0 & 1<<6) == 0) {
        SendStringEOLRS232("The SuperComputer and FRHA welcomes you, ver2");
    } else {
        SendStringEOLRS232("Dogged!!!!!");
    }
 
    InitializeComputer1();

    DispInfoBox("Waiting for", "terminal wakeup...");
    DisplayMenu(VERSION_MENU_STRING);

    SendStringEOLRS232("Send char for prompt...");
    
    timer = StartTimer(500);
    while ((!TimedOut(timer)) && (!(S1CON & 0x01))) {
        WDT = 1;
        SWDT = 1;
    }
    KillTimer(timer);
    
    if (!(S1CON & 0x01)) {
        SendStringEOLRS232("Running phone answer application");
		InitializePhoneAnsApp();
        ExecuteBatFile("ans.bat");
    }
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          
    DispInfoBox("TeleStation is under", "terminal control");
    MainShell();
}

void    InitializeComputer1( void )
{

//-------------------------------------------
// Initialisering av Watchdog
    WDTREL = 0x80;      //S�tter watchdog:en till max
    
//-------------------------------------------
// Initiering av Timer 0. Timer 0 anv�nds f�r att m�ta korta tidsf�rlopp. 
// Exempelvis f�r att ge timeout om ett kommando skickas till m�tattrappen 
// men inget svar kommer.

//  Tx0 ges enl:
//      �nskad tid till timeout: t
//      Kristallens klockfrekvens: f
//      Tx0 = 65536 - t*f/12


 
    TMOD = 0x71;        //Timer0 16-timer, Timer1 disabled
    TR0 = 0;            //Stannar Timer0
    ET0 = 1;            //Enable Timer 0 interrupt
    timeout = 0;
 

//-------------------------------------------
// Initialisering av LCD

    SendStringEOLRS232("LCD init...");
    InitializeLCD();

//-------------------------------------------
// Initialisering av FLASH

    SendStringEOLRS232("Flash init...");
    InitializeFlash();
    
//-------------------------------------------
// Initialisering av Filsystem

    SendStringEOLRS232("Filesys init...");
    InitializeFileSystem();

//-------------------------------------------
// Initialisering av WAV-modulen (ljud in- och uppspelning)

    SendStringEOLRS232("WAV init...");
    InitializeWAV();

//-------------------------------------------
// Initialisering av DTMF-modulen (tongenerering och tonavkodning)

    SendStringEOLRS232("DTMF init...");
    InitializeDTMF();

//-------------------------------------------
// Initialisering av Phone-modulen

    SendStringEOLRS232("Phone init...");
    InitializePhone();
    
//-------------------------------------------
// Initialisering av timer

    SendStringEOLRS232("Timer init...");
    InitializeTimer();

//-------------------------------------------
// Initialisering av calender

    SendStringEOLRS232("Calender init...");
    InitializeCalender();

//-------------------------------------------
// Initialisering av keyboard

    SendStringEOLRS232("Keyboard init...");
    InitializeKeyboard();

//-------------------------------------------
// Initialisering av DPot

    SendStringEOLRS232("DPOT init...");
    InitializeDPot();
//-------------------------------------------
// Initialisering av variabler

    error = EM_NOERROR;

//-------------------------------------------
// Initialisering av avbrottsprioriteringar
 
/*
Avbrottsk�llor:

*    SERIAL1 RS232
*    TIMER1        Samplingsklocka, skall g� i 8000 Hz. Matas med fosc/12,
                auto reload (dvs mode 2), reloadv�rde 256 - 116 = 140
*    TIMER2        Realtidsklocka uppdateras varje 100 millisekund.
                Matas med fosc/24, automatic reload med v�rdet 65536 - 46080 = 19456
*    INT0        DTMF

Prioritetsgrupper:

Grupp 1: INT0 SERIAL1
Grupp 4: TIMER1
Grupp 6: TIMER2

F�ljande prioritet skall g�lla:
H�gsta prio:    Grupp 4
Mellan prio:    Grupp 1
L�gsta prio:    Grupp 6
*/

    EAL = 1;            //M�jligg�r avbrottshantering
//    EAL = 0;            //Om�jligg�r avbrottshantering

}


void    InitializePhoneAnsApp( void )
{
//-------------------------------------------
// Initialisering av PhoneAns
 
    SendStringEOLRS232("Phone Answering init...");
    InitializePhoneAns();

//-------------------------------------------
// Initialisering av LogFile

    SendStringEOLRS232("Log file init...");
    InitializeLogFile();    
     
//-------------------------------------------
// Initialisering av Bat-fil hantering

    SendStringEOLRS232("Batch file init...");
    SetupBatFile();

//-------------------------------------------
// Initialisering av Memo-fil hantering

    SendStringEOLRS232("Memo file init...");
    IntitializeMemo();

}

void    MainShell( void )
//V�ntar p� en ny kommandorad (initierad av ett "Enter"-tecken.
//L�ser kommandoraden och bygger upp "commandLine" d�refter. Tolkar 
//och exekverar sedan kommandot.
{
    xdata char    cmdStr[CM_PARSIZE*CM_NRPAR];
    xdata int     len;
    xdata FILE*   cmdFile;
    xdata char    sysCom = 0;
    
    while (1) {   
        if (!sysCom) SendRS232('>');
        
        sysCom = 0;
 
        if (ReceiveLineRS232(cmdStr, CM_PARSIZE*CM_NRPAR) > 0) {
            if (cmdStr[0] == 0x1B) {
                if (cmdStr[1] == 'D') {
                    fdownLoad();   
                    sysCom = 1;
                } else if (cmdStr[1] == 'U') {
                    fupLoad();
                    sysCom = 1;
                }
            }
        }
        
        if (!sysCom) {
            if (!DecodeAndPerform(cmdStr)) {
                len = myStrLen(commandLine[0]);
                commandLine[0][len++] = '.';
                commandLine[0][len++] = 'B';
                commandLine[0][len++] = 'A';
                commandLine[0][len++] = 'T';
                commandLine[0][len++] = 0x00;
                cmdFile = fopen(commandLine[0], "r");
                if (cmdFile) {
//                    SendStringEOLRS232("Found a BAT-file");
                    fclose(cmdFile);
                    ExecuteBatFile(commandLine[0]);
//                    SendStringEOLRS232("BAT-file executed");
                } else {
                    SendStringEOLRS232("Unrecognized command or BAT-file");
//                error = 0x00;
                }
            }
 
//            cmdStr[myStrLen(cmdStr)] = ' ';
//            cmdStr[myStrLen(cmdStr)+1] = 0;

//            SendStringEOLRS232(cmdStr);
            
//            sprintf(msg, "Length: %d", myStrLen(cmdStr));
//            SendStringEOLRS232(msg);

            if (error) {
                SendStringRS232("Error code: ");
                myLong2HexStr(error, msg);
                SendStringEOLRS232(msg);
                error = 0;
            }
        }
    }
}



char    DecodeAndPerform(const char* cmdStr)
//Sorterar upp str�ngen cmdStr i "mellanslag-uppdelade" str�ngar
//S�ker sedan efter ett kommando motsv det i cmdStr och utf�r det.
//Om inget kommando finns retuneras 0, annars 1
{
    xdata int     i; 
    xdata char    j; 
    xdata char    n;
//    char    c;
//    char    copy;

    xdata char* tmpStr; 
    xdata char  delim[4] = " \t";
    
    xdata char  name[20];
    xdata char  category;
 
    if (cmdStr[0] == 0)  return 1;             //Ignorera tomma rader

    nrCommandParameters = 0;
    n = 0;
        
//    SendStringRS232("Command: ");
//    SendStringEOLRS232(cmdStr);


    i = 0;
    j = 0;



/*
    copy = 1;
    
    while ( (c = cmdStr[i++]) != 0x00) {
        if (copy) {
            SendRS232(c);
            if ((c == ' ') || (c == '\t')) {
                commandLine[nrCommandParameters][j++] = 0x00;
                j = 0;
                nrCommandParameters++;
                copy = 0;
            } else {
                commandLine[nrCommandParameters][j++] = c;
            }
        } else {
            if ((c != ' ') && (c != '\t')) {
                commandLine[nrCommandParameters][j++] = c;
                copy = 1;
            }
        }

    }
*/

    i = 0; 
    tmpStr = strtok(cmdStr, delim); 
    while ((tmpStr != NULL) && (n < CM_NRPAR)) {
        myStrCpy(commandLine[n++], tmpStr);
//        SendStringEOLRS232(tmpStr);
        tmpStr = strtok(NULL, delim); 
//        SendRS232('0' + i);
//        SendStringEOLRS232(" ");
//        i++;
    }


//    myInt2DecStr(n, msg);
//    SendStringEOLRS232(msg);
//    SendRS232('0' + n);
//    SendStringEOLRS232(" ");
    
    nrCommandParameters = n;

    
    for(i=n ; i < CM_NRPAR ; i++) {
        commandLine[i][0] = 0;
    }
            
/*
            for (i=0 ; i<n ; i++) {
                SendStringRS232("commandLine ");
                SendRS232('0' + i);
                SendStringRS232(": ");
                SendStringEOLRS232(commandLine[i]);
            }
 
*/

    if (!myStrCmpNoCase(commandLine[0], CM_HELLO)) {
        
        Hello();
/*              
            } else if (!myStrCmpNoCase(commandLine[0], CM_TESTHW)) {

                TestHW();

            } else if (!myStrCmpNoCase(commandLine[0], CM_ID)) {

                FlashID();
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_ERASE)) {

        EraseAll();

    } else if (!myStrCmpNoCase(commandLine[0], CM_CREATEDYNAMIC)) {

        StartDynamic();

    } else if (!myStrCmpNoCase(commandLine[0], CM_ERASEDYNAMIC)) {

        EraseDynamic();
        WriteCalenderByte(NV_PLAYED_MS, 0xFF);
        WriteCalenderByte(NV_PLAYED_LS, 0xFF);
        lastPlayed = -1;

/*
    } else if (!myStrCmpNoCase(commandLine[0], CM_WRITEFILE1)) {

        Append();

            } else if (!myStrCmpNoCase(commandLine[0], CM_WRITEFILE2)) {

                WF2();

            } else if (!myStrCmpNoCase(commandLine[0], CM_WRITEFILE3)) {

                WF3();

            } else if (!myStrCmpNoCase(commandLine[0], CM_READFILE1)) {

                RF1();

            } else if (!myStrCmpNoCase(commandLine[0], CM_READFILE2)) {

                RF2();

            } else if (!myStrCmpNoCase(commandLine[0], CM_READFILE3)) {

                RF3();
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_DIR)) {

        Dir();
/*
            } else if (!myStrCmpNoCase(commandLine[0], CM_CREATELONG)) {

                CreateLong();

            } else if (!myStrCmpNoCase(commandLine[0], CM_READSPARE)) {

                ReadSpare();

            } else if (!myStrCmpNoCase(commandLine[0], CM_READSPARE2)) {

                ReadSpare2();

    } else if (!myStrCmpNoCase(commandLine[0], CM_DOWNLOAD)) {

        fdownLoad();

            } else if (!myStrCmpNoCase(commandLine[0], CM_UPLOAD)) {

                fupLoad();
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_TYPE)) {

        Type();

    } else if (!myStrCmpNoCase(commandLine[0], CM_TYPEA)) {

        TypeA();

    } else if (!myStrCmpNoCase(commandLine[0], CM_ECHO_MIC)) {

        receivedDTMF = 0;
        EchoMic();

    } else if (!myStrCmpNoCase(commandLine[0], CM_ECHO_PHONE)) {

        receivedDTMF = 0;
        EchoPhone();

    } else if (!myStrCmpNoCase(commandLine[0], CM_REC_MIC)) {

        receivedDTMF = 0;
        RecMicFile(commandLine[1]);

    } else if (!myStrCmpNoCase(commandLine[0], CM_REC_PHONE)) {

        receivedDTMF = 0;
        RecPhoneFile(commandLine[1]);
/*
    } else if (!myStrCmpNoCase(commandLine[0], CM_REC_PORT)) {

        receivedDTMF = 0;
        RecPort1(commandLine[1]);
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_TONE)) {

        WavTone();

    } else if (!myStrCmpNoCase(commandLine[0], CM_PLAY)) {

        receivedDTMF = 0;
        PlayFile(commandLine[1]);

    } else if (!myStrCmpNoCase(commandLine[0], CM_DTMF)) {

        MakeDTMFTones(commandLine[1]);
/*
    } else if (!myStrCmpNoCase(commandLine[0], CM_WAIT_DTMF)) {

        WaitForDTMF();
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_HANG_UP)) {

        HangUp();

    } else if (!myStrCmpNoCase(commandLine[0], CM_LIFT_PHONE)) {

        LiftPhone();
/*
    } else if (!myStrCmpNoCase(commandLine[0], CM_WAIT_RING)) {

        i = myStr2Int(commandLine[1]);
 
        WaitForRingSignals(i);
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_WAIT_FOR)) {

        i = myStr2Int(commandLine[1]);
        WaitFor(i);

    } else if (!myStrCmpNoCase(commandLine[0], CM_ANS_ON)) {
 
        answerOn = 1;
        P3.5 = 0;
        WriteCalenderByte(NV_ANSWERING_ON, answerOn);

    } else if (!myStrCmpNoCase(commandLine[0], CM_ANS_OFF)) {
 
        answerOn = 0;
        P3.5 = 1;
        WriteCalenderByte(NV_ANSWERING_ON, answerOn);
        
    } else if (!myStrCmpNoCase(commandLine[0], CM_CRE_LOG)) {
        CreateLog();
        SendStringEOLRS232("Log created here too!");
    } else if (!myStrCmpNoCase(commandLine[0], CM_ENTER_CODE)) {
        EnterCode();
    } else if (!myStrCmpNoCase(commandLine[0], CM_READ_CALENDER_BYTE)) {
            
        i = myStr2Int(commandLine[1]);
        if ((i >= 0) && (i < 64)) {
            if (!ReadCalenderByte(i, &j)) {
                myLong2HexStr(j, msg);
                SendStringEOLRS232(msg);
            } else {
                SendStringEOLRS232("Error in read");
            }
        } else {
            SendStringEOLRS232("Illegal address");
        }
 
    } else if (!myStrCmpNoCase(commandLine[0], CM_WRITE_CALENDER_BYTE)) {
        i = myStr2Int(commandLine[1]);
        j = myStr2Int(commandLine[2]);
        if ((i >= 0) && (i < 64)) {
            if (WriteCalenderByte(i, j)) {
                SendStringEOLRS232("Error in write");
            }
        } else {
            SendStringEOLRS232("Illegal address");
        }

/*
    } else if (!myStrCmpNoCase(commandLine[0], CM_LCD_SET)) {

        i = myStr2Int(commandLine[1]);
        j = myStr2Int(commandLine[2]);
        SetLCDReg(i, j);
*/
    } else if (!myStrCmpNoCase(commandLine[0], CM_GOTO_LABEL)) {
        
        if (batFilePtr) {
            GotoLabel(commandLine[1]);
        } else {
            SendStringEOLRS232("No BAT-file is running!");
        }

    } else if (!myStrCmpNoCase(commandLine[0], CM_GOTO_CAT_EQUAL)) {
        
        if (batFilePtr) {
            if (callerCategory == myStr2Int(commandLine[1])) GotoLabel(commandLine[2]);
        } else {
            SendStringEOLRS232("No BAT-file is running!");
        }
    } else if (!myStrCmpNoCase(commandLine[0], CM_RECMSG)) {
        RecordCallerMessage();
    } else if (!myStrCmpNoCase(commandLine[0], CM_QUESTION)) {
        AskQuestionMsg();
    } else if (!myStrCmpNoCase(commandLine[0], CM_PLAYNEW)) {
        PlayNewMessages();
        receivedDTMF = 0;
        ResetDTMFIndex();
    } else if (!myStrCmpNoCase(commandLine[0], CM_WAITCALL)) {
        WaitForCall();
    } else if (!myStrCmpNoCase(commandLine[0], CM_READ_PB)) {
        ReadPhoneBookFile();
    } else if (!myStrCmpNoCase(commandLine[0], CM_LOOKUP_PB)) {
        LookUpCaller(commandLine[1], name,  &category);
        SendStringRS232("Caller: ");
        SendStringEOLRS232(name);
        SendStringRS232("Category: ");
        SendRS232('0' + category);
        SendStringEOLRS232(" ");

    } else if (!myStrCmpNoCase(commandLine[0], CM_SETWIPER)) {
        i = myStr2Int(commandLine[1]);
        j = myStr2Int(commandLine[2]);
        j = SetDPWiper(i, j);
        SendStringRS232("Returned: "); 
        myInt2DecStr(j, msg);
        SendStringEOLRS232(msg);

    } else if (!myStrCmpNoCase(commandLine[0], CM_SPEAKVOL)) {
        i = myStr2Int(commandLine[1]);
		speakerVolume = i;
		WriteCalenderByte(NV_SPEAKER_VOL, speakerVolume);
        SendStringRS232("Speaker volume set to: "); 
        myInt2DecStr(i, msg);
        SendStringEOLRS232(msg);
    } else if (!myStrCmpNoCase(commandLine[0], CM_SETDPREG)) {
    } else if (!myStrCmpNoCase(commandLine[0], CM_TRFTOWIPER)) {
    } else if (!myStrCmpNoCase(commandLine[0], CM_READWIPER)) {
        i = myStr2Int(commandLine[1]);
        i = ReadDPWiper(i, &j);
        SendStringRS232("Returned: "); 
        myInt2DecStr(i, msg);
        SendStringEOLRS232(msg);
        SendStringRS232("Value: "); 
        myInt2DecStr(j, msg);
        SendStringEOLRS232(msg);
    } else if (!myStrCmpNoCase(commandLine[0], CM_DATAH)) {
        DPDA = 1;
    } else if (!myStrCmpNoCase(commandLine[0], CM_DATAL)) {
        DPDA = 0;
    } else if (!myStrCmpNoCase(commandLine[0], CM_CLKH)) {
        DPCL = 1;
    } else if (!myStrCmpNoCase(commandLine[0], CM_CLKL)) {
        DPCL = 0;
    } else if (!myStrCmpNoCase(commandLine[0], CM_CATJMP)) {
        if (CategoryHit(commandLine[1], callerCategory)) {
			GotoLabel(commandLine[2]);
		}
    } else if (!myStrCmpNoCase(commandLine[0], CM_TESTCATJMP)) {
        if (CategoryHit(commandLine[1], myStr2Int(commandLine[2]))) {
            SendStringEOLRS232("Category Hit");
        } else {
            SendStringEOLRS232("Category Miss");
        }
    } else if (!myStrCmpNoCase(commandLine[0], CM_PLAYMEMO)) {
        PlayMemo();
    } else if (!myStrCmpNoCase(commandLine[0], CM_RECMEMO)) {
       RecordMemo();
    } else if (!myStrCmpNoCase(commandLine[0], CM_SETSIGNALS)) {
        i = myStr2Int(commandLine[1]);
        if ((i>0) && (i<15)) {
            numSignals = i;
            WriteCalenderByte(NV_NUM_SIGNALS, numSignals);
        }
    } else {
        return 0;
    }
    return 1;
}



/**************************************************************************************
Batfile (kommandofil) spec: 

N�r en batfil har hittats och skall k�ras letas ett ledigt BatFileObj fram fr�n BatFiles. 
En BatFileObj �r ledig om dess occupied-medlem = 0. BatFileObj fylls i med information om filen.
currentBatFile s�tts till det nya BatFileObj. Sedan h�mtas en rad i filen i taget och k�rs 
genom DecodeAndPerform. Rader som b�rjar med # skall inte skickas. Rader som b�rjar med : �r 
definitioner av lablar och skall inte skickas till DecodeAndPerform. 

Om en rad b�rjar med : �r det en label. Den skall l�ggas in i labelList. Den f�r bara l�ggas in 
i listan om den inte lagts in tidigare. Om samma labelnamn definierats tidigare i bat filen skall
ett felmeddelande ges och batfilen avbrytas. Om samma label definition (samma rad) k�rs en g�ng 
till pga ex en loop skall den ignoreras. S�ledes skall en label l�ggas i listan om det �r den f�rsta
g�ngen som den aktuella kommandoraden k�rs i batfilen. Om en label som skall definieras hittas i listan
och dess filposition st�mmer med den nuvarande skall labeldefinitionen ignoreras. Om filpositionen
inte st�mmer skall ett felmeddelande ges och exekveringen avbrytas.

N�r en batfil �r slut kollas dess caller-medlem. Om den �r NULL avslutas exekveringen helt och 
currentBatFile s�tts till NULL. Annars s�tts currentBatFile till caller och exekveringen fors�tter med
den gamla batfilen.


BatFiles
========
BatFileObj[MAX_BATFILES]    BatFiles    //Sammlar alla BatFileObj


BatFileObj
==========
FILE*               fp              //Filpekare till bat-filen
Label[MAX_LABELS]   labels          //Lista med ing�ende labels
char                labelIndex      //Pekar p� n�sta lediga label
BatFileObj*         caller          //Den batfil som kallade p� denna. Om anv�ndaren gav kommandot 
                                    //skall den ineh�lla NULL
char                occupied        //1 om detta objekt anv�nds, 0 annars
 

Label
=====
char                name[LABEL_LEN] //H�r l�ggs en labels namn in
long                pos             //filpositionen av raden efter labeln



********************************************************************************************/

void    SetupBatFile( void )
{
    xdata int     i;

    batFilePtr = NULL;

    for (i=0 ; i<MAX_LABELS ; i++) {
        batLabelPos[i] = -1;
    }

// Nedan var ett f�rs�k till multipla bat-filer, kanske inf�rs senare
 
/*
    int     i;
    Label*  l;
    Label*  l2;
//    int j;
    
    for (i=0 ; i<MAX_BATFILES ; i++) {
        batFiles[i].fp = NULL;
        batFiles[i].caller = NULL;
        batFiles[i].occupied = 0;
    }
    for (i=0 ; i<MAX_LABELS ; i++) {
        labelList0[i].name[0] = 0;
        labelList0[i].pos = -1;
        labelList1[i].name[0] = 0;
        labelList1[i].pos = -1;
        labelList2[i].name[0] = 0;
        labelList2[i].pos = -1;
        labelList3[i].name[0] = 0;
        labelList3[i].pos = -1;
        labelList4[i].name[0] = 0;
        labelList4[i].pos = -1;
        labelList5[i].name[0] = 0;
        labelList5[i].pos = -1;
        labelList6[i].name[0] = 0;
        labelList6[i].pos = -1;
        labelList7[i].name[0] = 0;
        labelList7[i].pos = -1;
    }
    batFiles[0].labels = labelList0;
    batFiles[1].labels = labelList1;
    batFiles[2].labels = labelList2;
    batFiles[3].labels = labelList3;
    batFiles[4].labels = labelList4;
    batFiles[5].labels = labelList5;
    batFiles[6].labels = labelList6;
    batFiles[7].labels = labelList7;
    
    l = batFiles[1].labels;
    
    for (i=0 ; i<MAX_LABELS ; i++) {
        if (labelList1[i].pos == -1) {
            SendStringEOLRS232("labelList == -1");
        } else {
            SendStringEOLRS232("labelList != -1");
        }
//        l2 = &l[i];
        l2 = labelList1;
        if (l->pos == -1) {
            SendStringEOLRS232("batFiles[1] == -1");
        } else {
            SendStringEOLRS232("batFiles[1] != -1");
        }
    }
*/    
}

void    ClearLabelList( void )
{
    xdata int    i;

    for (i=0 ; i<MAX_LABELS ; i++) {
        batLabelPos[i] = -1;
        batLabelName[0][i] = 0;     //var [i][0]
    }
}


void    ExecuteBatFile(const char* batFileName)
{
    xdata char        batLine[CM_PARSIZE*CM_NRPAR];
    xdata int         i;
    
    if (batFilePtr = fopen(batFileName, "r")) {
        while (ftell(batFilePtr) < batFilePtr->length) {

            for (i=0 ; i<CM_PARSIZE*CM_NRPAR ; i++) batLine[i] = 0;     //F�r att strtok skall fungera

            fgetLine(batFilePtr, batLine, CM_PARSIZE*CM_NRPAR);
	        SendStringEOLRS232(batLine);
            if (batLine[0] != '#') {
                if (batLine[0] == ':') {
                    InsertLabel(batLine, ftell(batFilePtr));
                } else {
                    if (!DecodeAndPerform(batLine)) {
                        SendStringEOLRS232("BAT: Unrecognized command or BAT-file");
                    }
                }
                if (error) {
                    SendStringRS232("BAT: Error code: ");
                    myLong2HexStr(error, msg);
                    SendStringEOLRS232(msg);
                    error = 0;
                }
            }
        }
        ClearLabelList();
        fclose(batFilePtr);
        batFilePtr = NULL;
        SendStringEOLRS232("BAT-file finished");
    }
}


char    InsertLabel(const char* name, long pos)
//Retunerar 1 om det gick att l�gga in labeln, 0 om det inte gick. error s�tts om det inte gick.
{
    xdata int     i = 0;
//    int     t;
    xdata char*   tmpStr; 
    xdata char    delim[5] = " \t:";
    
    //Omforma name s� att inledande ':' tas bort och raden klipps efter f�rsta ordet
    tmpStr = strtok(name, delim); 

    //Finns labeln redan?
    while ((i < MAX_LABELS) && (batLabelPos[i] != -1) && (myStrCmpNoCase(batLabelName[i], tmpStr))) {
/*
        SendStringRS232("Insertsearch for: ");
        SendStringRS232(tmpStr);
        sprintf(msg, ", at index: %d", i);
        SendStringRS232(msg);
        t = batLabelPos[i] & 0xFF;
        sprintf(msg, ", has position: %d", t);
        SendStringEOLRS232(msg);
*/
        i++;
        WDT = 1;
        SWDT = 1;
    }
    if (i >= MAX_LABELS) {
        //Den �r inte definierad tidigare och det finns inte fler labelplatser
        error = EM_TOOMANYLABELS;
        return 0;
    } else {
        if (batLabelPos[i] == -1) {
            //Den �r inte definierad tidigare, definier nu!
            myStrCpy(batLabelName[i], tmpStr);
            batLabelPos[i] = pos;

/*            SendStringRS232("Inserted label: ");
            SendStringRS232(tmpStr);
            sprintf(msg, ", at index: %d", i);
            SendStringRS232(msg);
            t = pos & 0xFF;
            sprintf(msg, ", from position: %d", t);
            SendStringEOLRS232(msg);
*/
            return 1;
        } else {
            //Den �r definierad tidigare, �r det vid samma filposition (omdefiniering)?
            if (batLabelPos[i] == pos) {
                //Detta �r en omdefinition, ignorera
/*
                SendStringRS232("Redefinition OK: ");
                SendStringEOLRS232(name);
*/
                return 1;
            } else {
                //Labeln f�rs�ks definieras med nytt v�rde, ge felmeddelande
/*
                SendStringRS232("Redefinition error: ");
                SendStringEOLRS232(name);
*/
                error = EM_REDEFLABELATTEMPT;
                return 0;
            }
        }
    }
}

int    FindLabel(const char *name)
//Letar efter en label. Om den hittas retuneras dess index. Annars retuneras -1
{
    xdata int     i = 0;
//    int     t;

    while ((i < MAX_LABELS) && (batLabelPos[i] != -1) && (myStrCmpNoCase(batLabelName[i], name))) {
        i++;
        WDT = 1;
        SWDT = 1;
    }
    if ((i >= MAX_LABELS) || (batLabelPos[i] == -1)) {
        //Labeln hittades inte

//        SendStringRS232("Could not find label: ");
//        SendStringEOLRS232(name);

        return -1;
    } else {
/*
        SendStringRS232("Found label: ");
        SendStringRS232(name);
        SendStringRS232(" at index: ");
        SendRS232(i + '0');
        SendStringRS232(" with pos: ");
        myLong2HexStr(batLabelPos[i], msg);
        SendStringEOLRS232(msg);
                
        
        sprintf(msg, ", at index: %d", i);
        SendStringRS232(msg);
        t = batLabelPos[i] & 0xFF;
        sprintf(msg, ", from position: %d", t);
        SendStringEOLRS232(msg);
*/
        return i;
    }
}


char    GotoLabel(const char* name)
//Hoppar till labeln name. Om den redan �r definierad g�r den dit direkt. �r den inte definierad
//s�ks BAT-filen rad f�r rad efter den och stannar n�r den hittas. I b�da dessa fall retuneras 1.
//Om labeln inte hittas retuneras 0
{       
    xdata char    done;
    xdata char    batLine[CM_PARSIZE*CM_NRPAR];
    xdata int     pos;
 
//    SendStringRS232("Trying to find label...");

    //Finns labeln i listan?
    if ((pos = FindLabel(name)) == -1 ) {
//	    SendStringRS232("Searching for label...");
        //Nej, leta efter den
        done = 0;
        while (!done) {
			WDT = 1;
			SWDT = 1;
            fgetLine(batFilePtr, batLine, CM_PARSIZE*CM_NRPAR);
            if (batLine[0] == ':') {
                if (!InsertLabel(batLine, ftell(batFilePtr))) {
                    //Om det inte g�r att l�gga in nya lablar kommer den s�kta aldrig hittas
                    error = EM_TOOMANYLABELS;
                    return 0;
                }
                if ((pos = FindLabel(name)) != -1) {
                    done = 1;
                }
            }
            if (ftell(batFilePtr) >= batFilePtr->length) {
                //Slutet p� filen har n�tts men labeln hittades inte
                error = EM_NOLABEL;
                return 0;
            }
        }
    }
    //Om detta n�s inneh�ller har labeln hittats
    fSetPos(batFilePtr, batLabelPos[pos]);
//    SendStringRS232("Jumped to: ");
//    SendStringEOLRS232(name);
//    myLong2HexStr(batLabelPos[pos], msg);
//    SendStringEOLRS232(msg);
    
    return 1;
    
}



// Nedan var ett f�rs�k till multipla bat-filer, kanske inf�rs senare
/*
BatFileObj*    VacantBatFileObj(FILE* fp)
//Letar i BatFiles efter ett BatFileObj vars occupied-medlem = 0. Om en hittas 
//fylls den i med filePtr = fp, labelIndex = 0, caller = currentBatFile, occupied = 1 och en
//pekare till den retuneras. Annars retuneras NULL.
{
    int i = 0;

    while ((i < MAX_BATFILES) && (batFiles[i].occupied == 1)) {
        i++;
    }
    
    if (i < MAX_BATFILES) {
        batFiles[i].fp = fp;
        batFiles[i].caller = currentBatFile;
        batFiles[i].occupied = 1;
        return &(batFiles[i]);
    }
    return NULL;
}

void    ExecuteBatFile(const char* batFileName)
{
    FILE*       bfp;
    char        batLine[CM_PARSIZE*CM_NRPAR];
    int         len;
    BatFileObj* bfo;
    
    if (bfp = fopen(batFileName, "r")) {
        currentBatFile = VacantBatFileObj(bfp);
        while (currentBatFile) {
            while (ftell(currentBatFile->fp) < currentBatFile->fp->length) {
                fgetLine(currentBatFile->fp, batLine, CM_PARSIZE*CM_NRPAR);
                if (batLine[0] != '#') {
                    if (batLine[0] == ':') {
                        if (!InsertLabel(batLine, ftell(currentBatFile->fp))) {
                            error = EM_TOOMANYLABELS;
                        }
                    } else {
                        if (!DecodeAndPerform(batLine)) {
                            len = myStrLen(commandLine[0]);
                            commandLine[0][len++] = '.';
                            commandLine[0][len++] = 'B';
                            commandLine[0][len++] = 'A';
                            commandLine[0][len++] = 'T';
                            commandLine[0][len++] = 0x00;
                            if (bfp = fopen(commandLine[0], "r")) {
                                if (bfo = VacantBatFileObj(bfp)) {
                                    currentBatFile = bfo;
                                } else {
                                    error = EM_TOOMANYBATFILES;
                                    fclose(bfp);
                                }
                            } else {
                                SendStringEOLRS232("BAT: Unrecognized command or BAT-file");
                                error = 0x00;
                            }
                        }
                    }
                }
            }
            ClearLabelList(currentBatFile->labels);
            currentBatFile->occupied = 0;
            fclose(currentBatFile->fp);
            currentBatFile = currentBatFile->caller;
        }
    }
}
 

char    InsertLabel(const char* name, long pos)
//Retunerar 1 om det gick att l�gga in labeln, 0 om det inte gick. error s�tts om det inte gick.
{
    int     i = 0;
    Label*  cl;
    char*   tmpStr; 
    char    delim[5] = " \t:";
    


    //Omforma name s� att inledande ':' tas bort och raden klipps efter f�rsta ordet
    tmpStr = strtok(name, delim); 

    cl = currentBatFile->labels;

    //Finns labeln redan?
    while ((i < MAX_LABELS) && (cl[i].pos != -1) && (myStrCmpNoCase(cl[i].name, tmpStr))) {
        SendStringRS232("Insertsearch for: ");
        SendStringRS232(tmpStr);
        sprintf(msg, ", at index: %d", i);
        SendStringRS232(msg);
        sprintf(msg, ", has position: %d", cl[i].pos);
        SendStringEOLRS232(msg);
        i++;
    }
    if (i >= MAX_LABELS) {
        //Den �r inte definierad tidigare och det finns inte fler labelplatser
        error = EM_TOOMANYLABELS;
        return 0;
    } else {
        if (cl[i].pos == -1) {
            //Den �r inte definierad tidigare, definier nu!
            myStrCpy(cl[i].name, tmpStr);
            cl[i].pos = pos;
            SendStringRS232("Inserted label: ");
            SendStringRS232(tmpStr);
            sprintf(msg, ", at index: %d", i);
            SendStringRS232(msg);
            sprintf(msg, ", from position: %d", pos);
            SendStringEOLRS232(msg);
            return 1;
        } else {
            //Den �r definierad tidigare, �r det vid samma filposition (omdefiniering)?
            if (cl[i].pos == pos) {
                //Detta �r en omdefinition, ignorera
                return 1;
            } else {
                //Labeln f�rs�ks definieras med nytt v�rde, ge felmeddelande
                error = EM_REDEFLABELATTEMPT;
                return 0;
            }
        }
    }
}
 
char    FindLabel(const char *name, Label* l)
//Letar efter en label. Om den hittas s�tts l att peka p� den och 1 retuneras. Annars s�tts
//l till NULL och 0 retuneras
{
    int     i = 0;
    Label*  cl = currentBatFile->labels;

    while ((i < MAX_LABELS) && (cl[i].pos != -1) && (myStrCmpNoCase(cl[i].name, name))) {
        i++;
    }
    if ((i >= MAX_LABELS) || (cl[i].pos == -1)) {
        //Labeln hittades inte
        l = NULL;
        return 0;
    } else {
        SendStringRS232("Found label: ");
        SendStringRS232(name);
        sprintf(msg, ", at index: %d", i);
        SendStringRS232(msg);
        sprintf(msg, ", from position: %d", cl[i].pos);
        SendStringEOLRS232(msg);
        l = &(cl[i]);
        return 1;
    }
}


char    GotoLabel(const char* name)
//Hoppar till labeln name. Om den redan �r definierad g�r den dit direkt. �r den inte definierad
//s�ks BAT-filen rad f�r rad efter den och stannar n�r den hittas. I b�da dessa fall retuneras 1.
//Om labeln inte hittas retuneras 0
{       
    Label*  l;
    char    done;
    char    batLine[CM_PARSIZE*CM_NRPAR];
 
    //Finns labeln i listan?
    if (!FindLabel(name, l)) {
        //Nej, leta efter den
        done = 0;
        l = NULL;
        while (!done) {
            fgetLine(currentBatFile->fp, batLine, CM_PARSIZE*CM_NRPAR);
            if (batLine[0] == ':') {
                if (!InsertLabel(batLine, ftell(currentBatFile->fp))) {
                    //Om det inte g�r att l�gga in nya lablar kommer den s�kta aldrig hittas
                    return 0;
                }
                if (FindLabel(name, l)) {
                    done = 1;
                }
            }
            if (ftell(currentBatFile->fp) >= currentBatFile->fp->length) {
                //Slutet p� filen har n�tts men labeln hittades inte
                return 0;
            }
        }
    }
    //Om detta n�s inneh�ller l en pekare till den hittade labeln
    fSetPos(currentBatFile->fp, l->pos);
    return 1;
    
}
*/

void    Hello( void )
{

//    TIME    t;
//    DATE    d;

/*    
    ClearScreen();
    SetUpFrame();

    PosTo(1, 1);
    Write("Goddag");
    PosTo(2, 2);
    Write("Hejsan");
    PosTo(35, 3);
    Write("Bruttonationalprodukt");
    PosTo(35, 4);
    WriteClip("Seglingssemester");
    PosTo(2, 7);
    WriteClip("PLAY       UP   DOWN    MENU   PLAY NEW");
    ShowScreen();        
        
*/    
/*
    GetTimeNow(&t);
    CreateTimeString(&t, msg);
    SendStringEOLRS232(msg);
    
    GetDateNow(&d);
    CreateDateString(&d, msg);
    SendStringEOLRS232(msg);
*/
/*
    lastKey = 0;
    KeyboardOn();
 
    while (!(S1CON & 0x01)) {
        WDT = 1;
        SWDT = 1;
        if (lastKey) { 
            SendRS232(lastKey + '0');
            lastKey = 0;
        }
    }
 
    KeyboardOff();
*/
/*
    SendRS232('H');
    WaitFor(20);
    SendRS232('e');
    WaitFor(20);
    SendRS232('l');
    WaitFor(20);
    SendRS232('l');
    WaitFor(20);
    SendRS232('o');
    WaitFor(20);
*/
    SendStringEOLRS232("Hello!");
}

/*


#define CLE     P4.0
#define ALE     P4.1
#define WP      P4.2
#define SE      P4.3
#define CE      P4.4
#define FBUSY   P4.5

#define FREG (*(unsigned char xdata *) 0xE000)
#define FLASH (*(unsigned char xdata *) 0xB000)


void    Erase( void )
{
    for (eraseAddress = 10 ; eraseAddress < 0x800000 ; eraseAddress += 0x2000){
        FLASHKey = 0xAA;
        EraseBlock();
    }
}

*/ 


void    myStrCpy(char* dest, const char* src)
{        
    xdata int index = 0;
 
    while (src[index] != 0) {
        dest[index] = src[index];
        index++;
    }
    dest[index] = 0;
}

int    myStrLen(char* str)
{
    xdata int i = 0;
    
    while(str[i] != 0) {
        i++;
    }
    
    return i;
}

/*
void    myToUpper(char* dest, const char* src)
{
    int     index = 0;
    
    while (src[index]) {
        dest[index] = src[index] & 0xDF;
        index++;
    }
}
*/

char    myStrCmpNoCase(const char* s1, const char* s2)
{
    xdata int     index = 0;
    
    while ((s1[index]) && (s2[index])) {
        if ((s1[index] & 0xDF) != (s2[index] & 0xDF)) return 1;
        index++;
    }

    if ((!s1[index]) && (!s2[index])) return 0;
    return 1;
}

void    myLong2HexStr(long v, char* str)
{
    xdata char    i;
    xdata int     tmp;
    
    i = 0;

    str[i++] = '0';
    str[i++] = 'x';
    tmp = (v >> 28) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 24) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 20) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 16) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 12) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 8) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = (v >> 4) & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    tmp = v & 0xF;
    if (tmp > 9) str[i++] = tmp + '7';
    else str[i++] = tmp + '0';
    str[i++] = 0;
}
 
void    myInt2DecStr(int v, char* str)
{
    xdata char    i;
    xdata char    tmp;
    xdata char    vt;
    
    
    i = 0;
    if (v > 255) {
        myStrCpy(str, "> 255");
    } else if (v < -255) {
        myStrCpy(str, "< -255");
    } else {
        if (v < 0) {
            v = -v;
            str[i++] = '-';
        }
        vt = (char) v;
        if (vt > 99) {
            tmp = (char) vt / 100;
            str[i++] = tmp + '0';
            vt = vt - (tmp * 100);
        }
        if (vt > 9) {
            tmp = (int) vt / 10;
            str[i++] = tmp + '0';
            vt = vt - (tmp * 10);
        }
        str[i++] = vt + '0';
        str[i++] = 0;
    }
}
 
void    myInt2DecStr00(char v, char* str)
{
    xdata char    i;
    xdata char    tmp;
    
    i = 0;
    if (v > 99) {
        myStrCpy(str, "PO");
    } else {
        tmp = v / 10;
        str[i++] = tmp + '0';
        v = v - (tmp * 10);

        str[i++] = v + '0';
        str[i++] = 0;
    }
}


int myStr2Int(const char* str)
{
    xdata int t = 0;
    
    while ((*(str) >= '0') && (*(str) <= '9')) {
        t = (10 * t) + *(str++) - '0';
    }
    return t;
}

/*
void    ReadSpare( void )
{
    xdata int     i;
    xdata char    c;
           
    readAddress = 0;
    SetupSpareReadSeq();
    
    SendStringEOLRS232("Reading from flash spare area...");
    c = WaitAndReceiveRS232();

    for (i=0 ; i<16 ; i++) {
        sprintf(msg, "Address: %04X, Data: ", (int) readAddress);
        SendStringRS232(msg);
        c = ReadFLASH();
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
    }
    TerminateReadSeq();
}
 

void    WF1( void )
{
    int         i;
    FILE*       fp;

    SendStringEOLRS232("Creating file...");
 
    fp = fopen("Hej.txt", "w");

    if (fp) {
        i = fp->startAddress; 
        sprintf(msg, "Startaddress: %04X", i);
        SendStringEOLRS232(msg);

        SendStringEOLRS232("Putting 1200 characters...");
        for (i=0 ; i<1200 ; i++) {
            fputc(fp, i & 0xFF);
        }
        SendStringEOLRS232("Closing file...");
        fclose(fp);
        SendStringEOLRS232("Done");
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}
 

void    WF2( void )
{
    FILE*       fp;
    int         i;
    
    SendStringEOLRS232("Creating file...");
 
    fp = fopen("Hejsan02.txt", "w");

    if (fp) {
        i = fp->startAddress; 
        sprintf(msg, "Startaddress: %04X", i);
        SendStringEOLRS232(msg);

        SendStringEOLRS232("Putting five characters...");
        fputc(fp, 10);
        fputc(fp, 11);
        fputc(fp, 12);
        fputc(fp, 13);
        fputc(fp, 14);
 
        SendStringEOLRS232("Closing file...");
        fclose(fp);
        SendStringEOLRS232("Done");
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}


void    RF1( void )
{
    FILE*       fp;
    char        c;
    int         i, j;
 
    SendStringEOLRS232("Opening file...");
 
    fp = fopen("Hej.txt", "r");

    if (fp) {
        i = fp->startAddress; 
        sprintf(msg, "Startaddress: %04X", i);
        SendStringEOLRS232(msg);

        SendStringEOLRS232("Pulling 1200 characters...");
        for (i=0 ; i<100 ; i++) {
            for (j=0 ; j<11 ; j++) {
                c = fgetc(fp) & 0xFF;
                sprintf(msg, "%02X ", c);
                SendStringRS232(msg);
            }
            c = fgetc(fp) & 0xFF;
            sprintf(msg, "%02X", c);
            SendStringEOLRS232(msg);
        }
 
        SendStringEOLRS232("Closing file...");
        fclose(fp);
        SendStringEOLRS232("Done");
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}
 
void    RF2( void )
{
    FILE*       fp;
    FILE*       fp1;
    char        c;
    int         i;

    SendStringEOLRS232("Opening file...");
 
    fp = fopen("Hejsan02.txt", "r");

    if (fp) {     
        i = fp->startAddress; 
        sprintf(msg, "Startaddress: %04X", i);
        SendStringEOLRS232(msg);
 
        SendStringEOLRS232("Pulling five characters...");
 
        fp1 = fopen("Hej.txt", "r");

        if (fp1) {
            SendStringEOLRS232("Second file...");
            c = fgetc(fp1) & 0xFF;
            sprintf(msg, "%02X", c);
            SendStringEOLRS232(msg);
            c = fgetc(fp1) & 0xFF;
            sprintf(msg, "%02X", c);
            SendStringEOLRS232(msg);
            c = fgetc(fp1) & 0xFF;
            sprintf(msg, "%02X", c);
            SendStringEOLRS232(msg);
            fclose(fp1);
            SendStringEOLRS232("Second done");
        }

        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
 
        i = ftell(fp);
        sprintf(msg, "Current pos: %d", i);
        SendStringEOLRS232(msg);
        
        SendStringEOLRS232("Setting pos one up...");
        
        fSetPos(fp, i-1);
        
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
        
        SendStringEOLRS232("Rewinding...");
        
        Rewind(fp);
        
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
        c = fgetc(fp) & 0xFF;
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);

        SendStringEOLRS232("Closing file...");
        fclose(fp);
        SendStringEOLRS232("Done");
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}

void    RF3( void )
{
    FILE*       fp;
    int         i;
 
    SendStringEOLRS232("Opening file...");
 
    fp = fopen("Rader.txt", "r");

    if (fp) {
        i = fp->startAddress; 
        sprintf(msg, "Startaddress: %04X", i);
        SendStringEOLRS232(msg);
 
        SendStringEOLRS232("Reading 5 rows...");
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 26);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        fgetLine(fp, msg, 50);
        SendStringEOLRS232(msg);
        
        SendStringEOLRS232("Closing file...");
        fclose(fp);
        SendStringEOLRS232("Done");
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}



void    CreateLong( void )
{
    SendStringEOLRS232("Creating long filehead at 0...");

    writeAddress = 0;

    SetupSpareWriteSeq();

    WriteFLASH(0xAA);
    WriteFLASH(0x00);    //Fill�ngd = 4194294 bytes
    WriteFLASH(0x3F);
    WriteFLASH(0xFF);
    WriteFLASH(0xF6);

    WriteFLASH('L');
    WriteFLASH('o');
    WriteFLASH('n');
    WriteFLASH('g');
    WriteFLASH(0);
    WriteFLASH(0);
    WriteFLASH(0);
    WriteFLASH(0);
    WriteFLASH('t');
    WriteFLASH('x');
    WriteFLASH('t');
    
    PerformWriteSeq();
    TerminateWriteSeq();
}




void    ReadSpare2( void )
{
    xdata int     i;
    xdata char    c;
           
    readAddress = 0x400000;
    SetupSpareReadSeq();
    
    SendStringEOLRS232("Reading from flash spare area 400000...");
    c = WaitAndReceiveRS232();

    for (i=0 ; i<16 ; i++) {
        sprintf(msg, "Address: %04X, Data: ", (int) readAddress);
        SendStringRS232(msg);
        c = ReadFLASH();
        sprintf(msg, "%02X", c);
        SendStringEOLRS232(msg);
    }
    TerminateReadSeq();
}
*/


void    Append( void )
{
/*
    FILE*       fp;

    SendStringEOLRS232("Writing to append file");
    fp = fopenA("Append.txt", 8192, -1);
    
    if (fp) {
        fputLine(fp, commandLine[1]);
        fclose(fp);
    } else {
        SendStringEOLRS232("Unable to open file");
    }
    SendStringEOLRS232("Done    ");
*/
}









interrupt void T0_int (void)
//N�r man skall anv�nda timer 0 s�tter man f�rst n�gra register s� att �nskad tid st�lls in 
//(se InitialiseComputer()). Sedan s�tter man ig�ng timer 0. Om timer 0 sl�r �ver s� s�tts 
//timeout till 1 och r�knaren st�ngs av.
{
    timeout = 1;               
    TR0 = 0;                                            //Stoppar timer0
}




