/*************************************************************
Fil:            $Id: PhoneAns.c,v 1.10 2003/12/26 16:17:01 Gemensam Exp $

Beskrivning:    Innehåller telefonsvararfunktioner

Skapad:         00-02-06

Ändrad:         $Date: 2003/12/26 16:17:01 $

Ägare:          Fredrik Hoffman


Att göra:

Revisionshistoria:
				$Log: PhoneAns.c,v $
				Revision 1.10  2003/12/26 16:17:01  Gemensam
				Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
				Fixed PPD detection by decreasing phone volume from 63 to 15.
				Version 0304.
				
				Revision 1.9  2003/11/29 22:36:55  Gemensam
				Fixed problem with to short pulse width on PPD.
				Ver 0302.
				
				Revision 1.8  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.7  2003/11/23 20:29:34  Gemensam
				Complete and tested volume setting in options menu.
				Ver 0201
				
				Revision 1.6  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.5  2003/11/18 19:17:35  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.4  2003/11/13 22:35:44  Gemensam
				Added watchdog reset at GotoLabel, FindLabel and InsertLabel.
				Added global variable to contain speaker volume, Default volume set to 50.
				Version 1.1
				
				Revision 1.3  2003/11/13 19:35:37  Gemensam
				Closed PB-file after initial read.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

*************************************************************/

//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PhoneAns.h"
#include "calender.h"
#include "main.h"
#include "FileSystem.h"
#include "ErrorCodes.h"
#include "RS232.h"
#include "Wav.h"
#include "DTMF.h"
#include "Phone.h"
#include "Timer.h"
#include "DPOT.h"
#include "LCD.h"
#include "LogFile.h"
#include "Keyboard.h"


//////////////////////
// Globala variabler

extern xdata char	terminateAnswer;
extern xdata char	speakerVolume;
extern xdata char	receivedDTMF;		//Innehåller den senaste mottagna DTMF-signalen
										//från avbrottsrutinen. Om inget tagits emot sätts
										//den till 0. För att få ett användbart värde skall
										//funktionen DecodeDTMF() köras.
extern xdata char	lastKey;
extern xdata long	logOffset;			//Nästa skrivposition i Log-filen
extern xdata int	callIndex[1024];	//Innehåller positionen i logfilen där resp
										//samtalsnummer ligger. Index 0 är det äldsta 
										//samtalet, 1 är det näst äldsta osv (kronologindexerad)
extern xdata char	msg[100];
extern xdata char	commandLine[CM_PARSIZE][CM_NRPAR];	//Här lagras kommandot och alla parametrar
										//Varje kommando/parameter får vara max cmParSize tecken
										//Till varje kommando kan cmNrPar-1 kopplas
extern xdata int	nrCommandParameters;//Här anges antal strängar kommandot bygger upp (dvs
										//antal parametrar + 1 för kommandot)
extern xdata int	error;				//Error-kod, se definitioner ovan
 
xdata char			answerOn;
xdata int			numOfCalls;
xdata int			lastPlayed;			//Kronologindexerad
//xdata FILE*   logFilePtr;
xdata char			callerCategory;
xdata int			nextMemoIndex;		//Indexet till det senaste Memo:et, kan också
										//ses som antal inspelade memo:n.
//////////////////////
// Lokala variabler

xdata char			labDTMF[MAX_LABEL_LEN];
xdata char			lab0[MAX_LABEL_LEN];
xdata char			lab1[MAX_LABEL_LEN];
xdata char			lab2[MAX_LABEL_LEN];
xdata char			lab3[MAX_LABEL_LEN];
xdata char			lab4[MAX_LABEL_LEN];
xdata char			lab5[MAX_LABEL_LEN];
xdata char			lab6[MAX_LABEL_LEN];
xdata char			lab7[MAX_LABEL_LEN];
xdata char			lab8[MAX_LABEL_LEN];
xdata char			lab9[MAX_LABEL_LEN];
xdata char			labPound[MAX_LABEL_LEN];
xdata char			labStar[MAX_LABEL_LEN];
xdata char			labCorrectc[MAX_LABEL_LEN];
xdata char			labIncorrectc[MAX_LABEL_LEN];
xdata char			labTimeout[MAX_LABEL_LEN];
xdata char			labHungUp[MAX_LABEL_LEN];
xdata unsigned int	timeoutValue;
//xdata int			screenStart;
xdata int			cursor;								//Samtalsindexerad
xdata char			menuSelection;
xdata char			numSignals;
xdata DATE			dateNow;
xdata char			year;
xdata char			month;
xdata char			date;
xdata TIME			timeNow;
xdata char			hour;
xdata char			minute;
xdata char			timCur;
xdata char			PBSpace[PB_MAXENTRIES * PB_POSTSIZE];	//PhoneBook filspegling
xdata int			pbNEntries;
xdata char			callerName[20];
//xdata char		recMsgs[1024];


//////////////////////
// Kod

void    InitializePhoneAns( void )
{       
//    SendStringRS232("Calls: ");
//    myInt2DecStr(numOfCalls, msg);
//    SendStringEOLRS232(msg);
    ReadPhoneBookFile();
}




void    WaitForCall( void )
/*
Kommandorad:

WAITCALL LS=SUCCESS_LABEL LT=TIMEOUT_LABEL

*/
{
    xdata char    DTMFString[40];
    xdata int     i;
    xdata int     j;
    xdata char    c;
    xdata char    timeStr[15];
    xdata char    menuState;
    xdata DATE    dateNow;
    xdata TIME    timeNow;
    xdata char    timer;
    xdata char    legalCallNumber;
    xdata FILE*   df;
 
    PhoneInVolume(63);
     
    SendStringEOLRS232("Init waitcall...");
    ClearDisp();
    DispInfoBox("Initializing answering machine", "Please wait...");

    if (BuildCallerList()) {
        ReadCalenderByte(NV_PLAYED_MS, &c);
        lastPlayed = c << 8;
        ReadCalenderByte(NV_PLAYED_LS, &c);
        lastPlayed += c;
        if (lastPlayed == 0xFFFF) lastPlayed = -1;
    } else {
        lastPlayed = -1;
        WriteCalenderByte(NV_PLAYED_MS, 0xFF);
        WriteCalenderByte(NV_PLAYED_LS, 0xFF);
        WriteCalenderByte(0x07, 0x80);          //Släck diod
        CreateLog();
    }
    terminateAnswer = 0;
        
    SetupUpQLabels();
    
    KeyboardOn();
    
    ReadCalenderByte(NV_ANSWERING_ON, &answerOn);
    if (answerOn) P3.5 = 0;
    else P3.5 = 1;
    ReadCalenderByte(NV_NUM_SIGNALS, &numSignals);

	ReadCalenderByte(NV_SPEAKER_VOL, &speakerVolume);
    
//    SendStringRS232("Num signals: ");
//    SendRS232('0' + numSignals);
//    SendStringEOLRS232(" ");
    
    legalCallNumber = 0;

    SendStringEOLRS232("Init waitcall done");

    menuSelection = 0;
    menuState = MS_LOG;

    ClearDisp();
    FillDispBuffer();
 
    cursor = 0;
    LogCursorAt(0, 1);
    DispMemUsage();
    DisplayMenu("  PLAY     UP     DOWN    MENU   PLAY NEW");
    
    do {    
        receivedDTMF = 0;
        ResetDTMFIndex();
 
        while (!receivedDTMF) {
            //Huvudloop för menysystem och knapptryckningar

            WDT = 1;
            SWDT = 1;
        
            while ((!receivedDTMF) && (!lastKey)) {
                WDT = 1;
                SWDT = 1;
            }
            switch (menuState) {
            case MS_LOG :
                if (lastKey == 1) {
                    PlayThisCall(numOfCalls - cursor - 1);
                    receivedDTMF = 0;
                    ResetDTMFIndex();
                    LogCursorAt(cursor, 1);
                } else if (lastKey == 2) {
                    if (cursor > 0) {
                        cursor--;
                        LogCursorAt(cursor, 0);
                    }
                } else if (lastKey == 4) {
                    if (cursor < numOfCalls-1) {
                        cursor++;
                        LogCursorAt(cursor, 0);
                    }
                } else if (lastKey == 8) {
                    menuState = MS_MENU;
                    DispOptionsMenu(answerOn, numSignals);
                    menuSelection = 0;
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT     UP     DOWN    BACK");			// Should be buttons of default menuSelection
                } else if (lastKey == 16) {
                    PlayNewMessages();
                    lastPlayed = numOfCalls - 1;
                    WriteCalenderByte(NV_PLAYED_MS, (lastPlayed >> 8) & 0xFF);
                    WriteCalenderByte(NV_PLAYED_LS, lastPlayed & 0xFF);
                    WriteCalenderByte(0x07, 0x80);          //Släck diod
                    receivedDTMF = 0;
                    ResetDTMFIndex();
                    FillDispBuffer();
                    LogCursorAt(cursor, 1);
                }
                break;
            case MS_MENU:
                if (lastKey == 1) {
                    menuSelection++;
                    if ((menuSelection == MENU_ITEM_SPKR_VOL) ||
						(menuSelection == MENU_ITEM_NUMSIG)) DisplayMenu("  NEXT     UP     DOWN    BACK");
                    if ((menuSelection == MENU_ITEM_ANS_ONOFF) || 
						(menuSelection == MENU_ITEM_MEMO) || 
						(menuSelection == MENU_ITEM_TIMEDATE) || 
						(menuSelection == MENU_ITEM_ERASE)) DisplayMenu("  NEXT   SELECT           BACK");
                    if (menuSelection == MENU_NUM_OF_ITEMS) menuSelection = 0;
                    OptionCursorAt(menuSelection);
                } else if (lastKey == 2) {
                    if (menuSelection == MENU_ITEM_ANS_ONOFF) {
                        if (answerOn) {
                            answerOn = 0;
                            P3.5 = 1;
                        } else {
                            answerOn = 1;
                            P3.5 = 0;
                        }
                        WriteCalenderByte(NV_ANSWERING_ON, answerOn);
                        DispOptionsMenu(answerOn, numSignals);
                        OptionCursorAt(menuSelection);
                    } else if (menuSelection == MENU_ITEM_MEMO) {
                        menuState = MS_MEMO;
                        DispMemoScreen();
                        DisplayMenu("  PLAY   RECORD            BACK");
                    } else if (menuSelection == MENU_ITEM_TIMEDATE) {
                        menuState = MS_TIMEDATE;
                        GetDateNow(&dateNow);
                        year = (dateNow.year & 0x0F) + (10 * ((dateNow.year >> 4) & 0x0F));
                        month = (dateNow.month & 0x0F) + (10 * ((dateNow.month >> 4) & 0x0F));
                        date = (dateNow.date & 0x0F) + (10 * ((dateNow.date >> 4) & 0x0F));
                        GetTimeNow(&timeNow);
                        hour = (timeNow.hours & 0x0F) + (10 * ((timeNow.hours >> 4) & 0x0F));
                        minute = (timeNow.minutes & 0x0F) + (10 * ((timeNow.minutes >> 4) & 0x0F));
                        timCur = 0;                                      
                        DispSetTimeDateHead();
                        DisplayMenu("  NEXT    UP      DOWN      DONE   CANCEL");
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (menuSelection == MENU_ITEM_NUMSIG) {
                        if (numSignals < 15) {
                            numSignals++;
                            WriteCalenderByte(NV_NUM_SIGNALS, numSignals);
                        }
                        DispOptionsMenu(answerOn, numSignals);
                        OptionCursorAt(menuSelection);
                    } else if (menuSelection == MENU_ITEM_ERASE) {
                        menuState = MS_ERASEQST;
                        DispEraseQuest();
					} else if (menuSelection == MENU_ITEM_SPKR_VOL) {
						if (speakerVolume < 63) {
							speakerVolume += 1;
							WriteCalenderByte(NV_SPEAKER_VOL, speakerVolume);
						}
                        DispOptionsMenu(answerOn, numSignals);
                        OptionCursorAt(menuSelection);
                    }
                } else if (lastKey == 4) {
                    if (menuSelection == MENU_ITEM_NUMSIG) {
                        if (numSignals > 0) {
                            numSignals--;
                            WriteCalenderByte(NV_NUM_SIGNALS, numSignals);
                        }
					} else if (menuSelection == MENU_ITEM_SPKR_VOL) {
						if (speakerVolume > 0) {
							speakerVolume -= 1;
							WriteCalenderByte(NV_SPEAKER_VOL, speakerVolume);
						}
                    }
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                } else if (lastKey == 8) {
                    LogCursorAt(cursor, 1);
                    DisplayMenu("  PLAY     UP     DOWN    MENU   PLAY NEW");
                    menuState = MS_LOG;
                }
                break;
            case MS_TIMEDATE:
                if (lastKey == 1) {
                    timCur++;
                    if (timCur > 4) timCur = 0;
                    DispSetTimeDate(timCur, year, month, date, hour, minute);
                } else if (lastKey == 2) {
                    if (timCur == 0) {
                        year++;
                        if (year > 99) year = 0;
                        CorrectDate(0);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 1) {
                        month++;
                        if (month > 12) month = 1;
                        CorrectDate(0);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 2) {
                        date++;
                        CorrectDate(1);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 3) {
                        hour++;
                        if (hour > 23) hour = 0;
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 4) {
                        minute++;
                        if (minute > 59) minute = 0;
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    }
                } else if (lastKey == 4) {
                    if (timCur == 0) {
                        if (year == 0) year = 99;
                        else year--;
                        CorrectDate(0);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 1) {
                        month--;
                        if (month < 1) month = 12;
                        CorrectDate(0);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 2) {
                        date--;
                        CorrectDate(1);
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 3) {
                        if (hour == 0) hour = 23;
                        else hour--;
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    } else if (timCur == 4) {
                        if (minute == 0) minute = 59;
                        else minute--;
                        DispSetTimeDate(timCur, year, month, date, hour, minute);
                    }
                } else if (lastKey == 8) {
                    menuState = MS_MENU;
                    c = year / 10;
                    WriteCalenderByte(0x06, (c << 4) + year - (c * 10));
                    c = month / 10;
                    WriteCalenderByte(0x05, (c << 4) + month - (c * 10));
                    c = date / 10;
                    WriteCalenderByte(0x04, (c << 4) + date - (c * 10));
                    c = hour / 10;
                    WriteCalenderByte(0x02, (c << 4) + hour - (c * 10));
                    c = minute / 10;
                    WriteCalenderByte(0x01, (c << 4) + minute - (c * 10));
                    WriteCalenderByte(0x00, 0);
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT   SELECT           BACK");
                } else if (lastKey == 16) {
                    menuState = MS_MENU;
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT   SELECT           BACK");
                }
                break;
            case MS_ERASEQST:
                if (lastKey == 2) {
                    DispInfoBox("Erasing messages", "Please wait...");
                    EraseDynamic();
                    WriteCalenderByte(NV_PLAYED_MS, 0xFF);
                    WriteCalenderByte(NV_PLAYED_LS, 0xFF);
                    lastPlayed = -1;
                    nextMemoIndex = 0;
                    df = fopen("Log.txt", "r");
                    if (df) {
                        fclose(df);
                        menuState = MS_MENU;
                        DispOptionsMenu(answerOn, numSignals);
                        OptionCursorAt(menuSelection);
                        DisplayMenu("  NEXT   SELECT           BACK");
                    } else {
                        CreateLog();

                        DispInfoBox("Initializing answering machine", "Please wait...");

                        if (BuildCallerList()) {
                            ReadCalenderByte(NV_PLAYED_MS, &c);
                            lastPlayed = c << 8;
                            ReadCalenderByte(NV_PLAYED_LS, &c);
                            lastPlayed += c;
                            if (lastPlayed == 0xFFFF) lastPlayed = -1;
                        }

                        menuSelection = 0;
                        menuState = MS_LOG;

                        ClearDisp();
                        FillDispBuffer();
 
                        cursor = 0;
                        LogCursorAt(0, 1);
                        DispMemUsage();
                        DisplayMenu("  PLAY     UP     DOWN    MENU   PLAY NEW");
    
                    }
                    menuState = MS_MENU;
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT   SELECT           BACK");
                } else if (lastKey == 8) {
                    menuState = MS_MENU;
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT   SELECT           BACK");
                }
                break;
            case MS_MEMO:
                if (lastKey == 1) {
                    PlayMemo();
                    DispMemoScreen();
                    DisplayMenu("  PLAY   RECORD            BACK");
                } else if (lastKey == 2) {
                    RecordMemo();
                    DispMemoScreen();
                    DispMemUsage();
                    DisplayMenu("  PLAY   RECORD            BACK");
                } else if (lastKey == 8) {
                    menuState = MS_MENU;
                    DispOptionsMenu(answerOn, numSignals);
                    OptionCursorAt(menuSelection);
                    DisplayMenu("  NEXT   SELECT           BACK");
                }
                break;
            }

            lastKey = 0;
 
        }

//        SendStringEOLRS232("DTMF detected");
 
        timer = StartTimer(300);                   //Tre sekunders timout

        do {
            c = DecodeReceivedDTMF();
            WDT = 1;
            SWDT = 1;
        } while ((!TimedOut(timer)) && (c != 'C'));
        
        KillTimer(timer);
        
        if (c == 'C') legalCallNumber = 1;

    } while (!legalCallNumber);

//    SendStringEOLRS232("Legal call detected");
 
    j = 0;

    while (c = GetNextDTMF()) {
        if ((c >= '0') && (c <= '9')) DTMFString[j++] = c;
        WDT = 1;
        SWDT = 1;
    } 
    
 
    DTMFString[j] = 0x00;

//    SendStringRS232("Number: ");
//    SendStringEOLRS232(DTMFString);
    
    callerCategory = 0;
    LookUpCaller(DTMFString, callerName, &callerCategory);

    WDT = 1;
    SWDT = 1;
    
//    i = ftell(logFilePtr);
    i = logOffset;
    callIndex[numOfCalls++] = i + 4;

    WDT = 1;
    SWDT = 1;

    GetDateNow(&dateNow);
    CreateDateString2(&dateNow, msg);
    msg[6] = ' ';

    WDT = 1;
    SWDT = 1;

    GetTimeNow(&timeNow);

    CreateTimeString(&timeNow, timeStr);

    WDT = 1;
    SWDT = 1;

    msg[7] = timeStr[0];
    msg[8] = timeStr[1];
    msg[9] = timeStr[2];
    msg[10] = timeStr[3];
    msg[11] = timeStr[4];
    msg[12] = 0x00; 
 
    ReadCalenderByte(0x07, &c);
    if (c == 0x80) WriteCalenderByte(0x07, 0);  //Tänd diod

    DispInfoBox("Now calling:", callerName);
    
    if (answerOn) i = numSignals;
    else i = 10;
    c =  WaitForRingSignals(i);

    LogMessage("NWC", callerName);
    LogMessage("TIM", msg);

    if (c == 1) {
        LogMessage("DSC", NULL);
        if (labTimeout[0] != 0) {
            GotoLabel(labTimeout);
        }
    } else if (c == 0) {
        if (labStar[0] != 0) {
            GotoLabel(labStar);
        }
    }
}

void    CorrectDate(char changedDate)
{
    xdata char    topDate;
    
    if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)) topDate = 31;
    else if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) topDate = 30;
    else {
        if ((year-(4*(year/4))) == 0) {
            topDate = 29;
        } else {
            topDate = 28;
        }
    }
    if (date < 1) date = topDate;
    else if (date > topDate) {
        if (changedDate) date = 1;
        else date = topDate;
    }
}




void    RecordCallerMessage( void )
/*
Kommandorad:

RECMSG LD=DTMF_ABORT_LABEL LH=HUNGUP_LABEL

*/
{
    xdata char    fileName[15];

    WDT = 1;
    SWDT = 1;

    SetupUpQLabels();
    CreateUniqueFilename(fileName);

    WDT = 1;
    SWDT = 1;

    RecPhoneFile(fileName);
 
    LogMessage("RCM", fileName);
    WriteCalenderByte(0x07, 0x10);  //Blinka diod

    WDT = 1;
    SWDT = 1;

    if (receivedDTMF) {
        LogMessage("DTM", NULL);
        receivedDTMF = 0;
//        SendStringEOLRS232("Received DTMF");
        if (labDTMF[0] != 0x00) {
            GotoLabel(labDTMF);
        }
    } else if (terminateAnswer) {
        if (labHungUp[0] != 0x00) {
            GotoLabel(labHungUp);
        }
//        LogMessage("End call");                //Skall denna ligga här eller vid någon "hangup" rutin???????
    }
    WDT = 1;
    SWDT = 1;
}






void    AskQuestionMsg( void )
/*
Kommandorad:

QUESTION QUEST1.WAW L1=LABEL1 L0=LABEL0 ... LS=LABELSTAR LP=LABELPOUND TV=10 LT=LABELTIMEOUT LH=LABELHUNGUP

*/
{
    xdata char    timer;
    xdata char    abort;
    xdata char    c;
    xdata unsigned int     rt;
 

    SetupUpQLabels();
    
    PlayFile(commandLine[1]);
 
    abort = 0;
    
    rt = timeoutValue * (unsigned int) 100;         //Blir fel

//    myLong2HexStr(timeoutValue, msg);
//    SendStringEOLRS232(msg);
                
    timer = StartTimer(2000);
    
    while (!abort) {
        WDT = 1;
        SWDT = 1;
        if (receivedDTMF) {
            c = DecodeReceivedDTMF();
            if ((c == '0') && (lab0[0] != 0x00)) {
                GotoLabel(lab0);
                LogMessage("DTM 0", NULL);
            } else if ((c == '1') && (lab1[0] != 0x00)) {
                GotoLabel(lab1);
                LogMessage("DTM 1", NULL);
            } else if ((c == '2') && (lab2[0] != 0x00)) {
                GotoLabel(lab2);
                LogMessage("DTM 2", NULL);
            } else if ((c == '3') && (lab3[0] != 0x00)) {
                GotoLabel(lab3);
                LogMessage("DTM 3", NULL);
            } else if ((c == '4') && (lab4[0] != 0x00)) {
                GotoLabel(lab4);
                LogMessage("DTM 4", NULL);
            } else if ((c == '5') && (lab5[0] != 0x00)) {
                GotoLabel(lab5);
                LogMessage("DTM 5", NULL);
            } else if ((c == '6') && (lab6[0] != 0x00)) {
                GotoLabel(lab6);
                LogMessage("DTM 6", NULL);
            } else if ((c == '7') && (lab7[0] != 0x00)) {
                GotoLabel(lab7);
                LogMessage("DTM 7", NULL);
            } else if ((c == '8') && (lab8[0] != 0x00)) {
                GotoLabel(lab8);
                LogMessage("DTM 8", NULL);
            } else if ((c == '9') && (lab9[0] != 0x00)) {
                GotoLabel(lab9);
                LogMessage("DTM 9", NULL);
            } else if ((c == '*') && (labStar[0] != 0x00)) {
                GotoLabel(labStar);
                LogMessage("DTM *", NULL);
            } else if ((c == '#') && (labPound[0] != 0x00)) {
                GotoLabel(labPound);
                LogMessage("DTM #", NULL);
            } else if (labDTMF[0] != 0x00) {
                GotoLabel(labDTMF);
                LogMessage("DTM", NULL);
            }
            abort = 1;
            receivedDTMF = 0x00;
        } else if (terminateAnswer){

            if (labHungUp[0] != 0x00) {
                GotoLabel(labHungUp);
            }

            abort = 1;
        } else if (TimedOut(timer)) {
            if (labTimeout[0] != 0x00) {
                SendStringEOLRS232("Quest timed out");
                GotoLabel(labTimeout);
            }
            abort = 1;
        }
    }
    KillTimer(timer);
}




void    EnterCode( void )
/*
Kommandorad:

ENTERCODE CODE.WAW 243875 LC=CORRECT_C_LABEL LI=INCORRECT_C_LABEL LH=LABELHUNGUP LT=LABELTIMEOUT

OBS: Alla kodinmatningar skall avslutas med fyrkant '#'

*/
{
    xdata char    abort = 0;
    xdata char    enteredCode[20];
    xdata char    timer;
    xdata char    c;
    xdata char    i = 0;
    xdata char*   cmp;

    SetupUpQLabels();

    PlayFile(commandLine[1]);
 
    receivedDTMF = 0;
    ResetDTMFIndex();
    cmp = commandLine[2];

    timer = StartTimer(2000);                  //Trettio sekunders timeout

    while (!abort) {
        c = DecodeReceivedDTMF();
        if (c != 'e') {
            i++;
            receivedDTMF = 0x00;
            if (c == '#') abort = 1;
            SendRS232(c);
        }
        if (TimedOut(timer)) abort = 2;
        if (i >= 20) abort = 3;
        if (terminateAnswer) abort = 4;
        WDT = 1;
        SWDT = 1;
    }
    
//    SendStringRS232("Abort code: ");
//    SendRS232('0' + abort);

    if (abort == 1) {
        //Check code
        i = 0;
        do {
            c = GetNextDTMF();
            enteredCode[i++] = c;
            WDT = 1;
            SWDT = 1;
        } while (c != '#');
    
        enteredCode[i-1] = 0x00;
/*        
        SendStringEOLRS232(" ");
        SendStringEOLRS232("Entered code: ");
        SendStringEOLRS232(enteredCode);
        SendStringEOLRS232(" ");
*/ 
        if (!myStrCmpNoCase(enteredCode, cmp)) {
//            SendStringRS232("Code OK ");
//            SendStringEOLRS232(labCorrectc);
            if (labCorrectc[0] != 0x00) {                //Code OK
                GotoLabel(labCorrectc);
                LogMessage("COK", NULL);
            }
        } else { 
//            SendStringRS232("Code Not OK ");
//            SendStringEOLRS232(labIncorrectc);
            if (labIncorrectc[0] != 0x00) {             //Code not OK
                GotoLabel(labIncorrectc);
                LogMessage("CNO", NULL);
            }
        }
    } else if (abort == 2) {
        if (labTimeout[0] != 0x00) {                    //Timed out
//            SendStringEOLRS232("Codeentering timed out");
            GotoLabel(labTimeout);
        }
    } else if (abort == 3) {
        if (labIncorrectc[0] != 0x00) {                 //Too many keypresses / Code not OK
            GotoLabel(labIncorrectc);
            LogMessage("CNO", NULL);
        }
    } else if (abort == 4) {
        if (labHungUp[0] != 0x00) {                     //Caller hung up
            GotoLabel(labHungUp);
        }
    }
        
    KillTimer(timer);

}

void    SetupUpQLabels( void )
{
    xdata char    i;
    xdata char*    str;

    labDTMF[0] = 0x00;
    lab0[0] = 0x00;
    lab1[0] = 0x00;
    lab2[0] = 0x00;
    lab3[0] = 0x00;
    lab4[0] = 0x00;
    lab5[0] = 0x00;
    lab6[0] = 0x00;
    lab7[0] = 0x00;
    lab8[0] = 0x00;
    lab9[0] = 0x00;
    labStar[0] = 0x00;
    labPound[0] = 0x00;
    labCorrectc[0] = 0x00;
    labIncorrectc[0] = 0x00;
    labTimeout[0] = 0x00;
    labHungUp[0] = 0x00;

    for (i=1 ; i<nrCommandParameters ; i++) {
        str = commandLine[i];
        if (str[2] == '=') {
            if (str[0] == 'L') {
                if (str[1] == '0') {
                    LabelCopy(lab0, str);
                } else if (str[1] == '1') {
                    LabelCopy(lab1, str);
                } else if (str[1] == '2') {
                    LabelCopy(lab2, str);
                } else if (str[1] == '3') {
                    LabelCopy(lab3, str);
                } else if (str[1] == '4') {
                    LabelCopy(lab4, str);
                } else if (str[1] == '5') {
                    LabelCopy(lab5, str);
                } else if (str[1] == '6') {
                    LabelCopy(lab6, str);
                } else if (str[1] == '7') {
                    LabelCopy(lab7, str);
                } else if (str[1] == '8') {
                    LabelCopy(lab8, str);
                } else if (str[1] == '9') {
                    LabelCopy(lab9, str);
                } else if (str[1] == 'S') {
                    LabelCopy(labStar, str);
                } else if (str[1] == 'P') {
                    LabelCopy(labPound, str);
                } else if (str[1] == 'C') {
                    LabelCopy(labCorrectc, str);
                } else if (str[1] == 'I') {
                    LabelCopy(labIncorrectc, str);
                } else if (str[1] == 'T') {
                    LabelCopy(labTimeout, str);
                } else if (str[1] == 'H') {
                    LabelCopy(labHungUp, str);
                } else if (str[1] == 'D') {
                    LabelCopy(labDTMF, str);
                }
            } else if ((str[0] == 'T') && (str[1] == 'V')) {
                timeoutValue = myStr2Int((char*) str + 3);
            }
        }
    }
}
        
void    LabelCopy(char* dest, const char* src)
{
    xdata char i;
    
    i = 0;
    
    while (src[i+3] != 0x00) {
        dest[i] = src[i+3];
        i++;
    }
    dest[i] = 0x00;
}
        





void    CreateUniqueFilename(char* str)
/*
Skapat namn blir ABCDEFGH.WAW där:

  A = 'M', huvud
  B = År enl 0-9
  C = Månad enl A-K (motsv 1-12)
  D = Dag enl A-Z, a-e (mostv 1-31)
  E = Timme enl 0-9, A-N (motsv 0-23)
  F = Minut enl 0-5 (tiotal motsv 0-50)
  G = Minut enl 0-9 (ental)
  H = Ordning enl 0-9, A-Z, a-z (motsv 0-62)

  A är alltid 'M' och B-G skapas från calendern. H prövas fram. Först testas '0', om det
  är upptaget prövas '1' osv. 
*/
{
    xdata TIME    tv;
    xdata DATE    dv;
    xdata char    order;
    xdata char    tmp;
    xdata FILE*   fp;

    GetTimeNow(&tv);
    GetDateNow(&dv);

    str[0] = 'M';
    str[1] = (dv.year & 0x0F) + '0';
    if (dv.month <= 9) {
        str[2] = dv.month + 'A' - 1;
    } else {
        str[2] = dv.month + 'A' - 1 -  16;
    }
    
    tmp = ((dv.date >> 4) & 0x03) * 10 + (dv.date & 0x0F);

    if (tmp <= 26) {
        str[3] = tmp + 'A' - 1;
    } else {
        str[3] = tmp + 'a' - 27;
    }

    tmp = ((tv.hours >> 4) & 0x03) * 10 + (tv.hours & 0x0F);

    if (tmp <= 9) {
        str[4] = tmp + '0';
    } else {
        str[4] = tmp + 'A' - 10;
    }
    str[5] = ((tv.minutes >> 4) & 0x07) + '0';
    str[6] = (tv.minutes & 0x0F) + '0';

    str[8] = '.';
    str[9] = 'W';
    str[10] = 'A';
    str[11] = 'W';
    str[12] = 0x00;

    order = '0';
    str[7] = '0';
    while (fp = fopen(str, "r")) {
        fclose(fp);
        order++;
        if (order > '9') order = 'A';
        if (order > 'Z') order = 'a';
        str[7] = order;
    }
    if (error == EM_FILENOTEXISTS) error = 0x00;
}


//-----------------------------------------
// Phone Book grejor

void    ReadPhoneBookFile( void )
{
    xdata FILE*     fp;
    xdata int       i, j;
    xdata char*     ptr;
    
    SendStringEOLRS232("Reading Phone book file..."); 
    DispInfoBox("Reading Phone Book", "Please wait...");
    
    ptr = PBSpace;
    
    fp = fopen("PB.tpb", "r");
    
    if (fp) {
        i = fgetc(fp);
        pbNEntries = i * 256;
        i = fgetc(fp);
        pbNEntries += i;
        if (pbNEntries > PB_MAXENTRIES) pbNEntries = PB_MAXENTRIES;
        for (i=0 ; i<pbNEntries ; i++) {
            WDT = 1;
            SWDT = 1;
            for (j=0 ; j<PB_POSTSIZE ; j++) *(ptr++) = fgetc(fp);
        }
		fclose(fp);
    } else {
        pbNEntries = 0;
    }
} 


void    LookUpCaller(char* number, char* name,  char* category)
{
    idata char* ptr;
    idata int   numberIndex;
    idata char  match = 0;
    idata char  c;
    idata int   currentPost = 0;
    
    for (currentPost = 0 ; currentPost < pbNEntries ; currentPost++) {
        ptr = PBSpace + currentPost * PB_POSTSIZE;
        numberIndex = 0;
        while (( (c = *(ptr++)) ==number[numberIndex++]) && (c != 0)) {
            WDT = 1;
            SWDT = 1;
        }
        if ((c == 0) && (number[numberIndex-1] == 0)) {
            do {              
                c = *(ptr++);
                *(name++) = c;
            } while (c != 0);
            *category = *ptr;
            return;
        }
    }
    strcpy(name, number);
    *category = 0;
    return;
}
                




char    CategoryHit(char* str, char category)
//OBS: Hoppar inte till label, kollar bara om category finns i den tolkade strängen "str"
{
    xdata char  tmpStr[5];
    xdata int   rangeStart;
    xdata int   rangeEnd;
    xdata char  c;
    xdata int   i = 0;  //Index i strängen som skall tolkas
    xdata int   j = 0;  //Index i den tilfälliga värdes-strängen (tmpStr)
    xdata char  categoryHit = 0;
    
    tmpStr[0] = 0;      //Termineringsnolla
    
    rangeStart = -1;    //Indikerar ingen "range" (x-y) på lut

//    SendStringEOLRS232("Checking this string:"); 
//    SendStringEOLRS232(str); 

    do {
        WDT = 1;
        SWDT = 1;
        c = str[i++];
//		SendRS232(c); 
        if ((c == ',') || (c == 0)) {
            if (rangeStart != -1) {
                rangeEnd = myStr2Int(tmpStr);
                if ((category >= rangeStart) && (category <= rangeEnd)) {
                    categoryHit = 1;
                }
            } else {
                if (category == myStr2Int(tmpStr)) categoryHit = 1;
            }
            tmpStr[0] = 0;
            j = 0;
        } else if (c == '-') {
            rangeStart = myStr2Int(tmpStr);
            tmpStr[0] = 0;
            j = 0;
        } else if ((c >= '0') && (c <= '9')) {
            tmpStr[j++] = c;
            tmpStr[j] = 0;
        }
    } while ((c != 0) && (!categoryHit));

//	if (categoryHit)	SendStringEOLRS232("Category hit"); 
//	else				SendStringEOLRS232("Category miss"); 

    return categoryHit;
}

//-----------------------------------------------------------------------
//Memo fil grejor:


void    IntitializeMemo( void )
//OBS: Kräver att antal memo max är 255
{
    xdata char  fileStr[15];
    xdata char  found = 0;
    xdata FILE* fp;

    nextMemoIndex = 0;

    while ((!found) && (nextMemoIndex < 256)) {
        WDT = 1;
        SWDT = 1;
        CreateMemoFileName(nextMemoIndex, fileStr);
        fp = fopen(fileStr, "r");
        if (fp) {
            nextMemoIndex++;
            fclose(fp);
        } else {
            found = 1;
        }
    }
}


void    RecordMemo( void )
//OBS: Funkar bäst om det finns ett ljud som heter "tone.waw". Annars
//går inspelningen ingång direkt efter knapptryckningen.
{
    xdata char memoName[15];
 
    WDT = 1;
    SWDT = 1;
    if (nextMemoIndex < 256) {    
        CreateMemoFileName(nextMemoIndex++, memoName);
        DispInfoBox("Now recording:", memoName);
        DisplayMenu("                   STOP                   ");
        SpeakerVolume(speakerVolume);
        PhoneInVolume(0);
        PlayFile("tone.waw");
        SpeakerVolume(0);
        PhoneInVolume(63);
        RecMicFile(memoName);
    }
}

void    PlayMemo( void )
{
    xdata char memoName[15];

    WDT = 1;
    SWDT = 1;

    if ((nextMemoIndex > 0) && (nextMemoIndex < 256)) {
        CreateMemoFileName(nextMemoIndex - 1, memoName);
        DispInfoBox("Now playing:", memoName);
        SpeakerVolume(speakerVolume);
        PhoneInVolume(0);
        PlayFile(memoName);
        SpeakerVolume(0);
        PhoneInVolume(63);
    }
}



void    CreateMemoFileName(int index, char* str)
{
    xdata char  indexStr[5];
    xdata char  c;
    xdata int   i = 0;

    myInt2DecStr(index, indexStr);

    myStrCpy(str, "Memo");
    str = str + 4;
    
    do {
        c = indexStr[i++];
        *(str++) = c;
    } while (c != 0);
    *(str-1) = '.';
    *(str++) = 'W';
    *(str++) = 'A';
    *(str++) = 'W';
    *(str++) = 0;
}    





 
 
 
 
 
 
 
 
