/*************************************************************
Fil:            $Id: LCD.c,v 1.5 2003/11/23 20:29:33 Gemensam Exp $

Beskrivning:    Funktioner för att skriva på skärmen

Skapad:         00-03-30

Ändrad:         $Date: 2003/11/23 20:29:33 $

Ägare:          Fredrik Hoffman


Att göra:


Revisionshistoria:
				$Log: LCD.c,v $
				Revision 1.5  2003/11/23 20:29:33  Gemensam
				Complete and tested volume setting in options menu.
				Ver 0201
				
				Revision 1.4  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

*************************************************************/
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "calender.h"
#include "main.h"
#include "FileSystem.h"
#include "ErrorCodes.h"
#include "RS232.h"
#include "Wav.h"
#include "DTMF.h"
#include "Phone.h"
#include "Timer.h"
#include "PhoneAns.h"
#include "LogFile.h"
#include "FileSystem.h"
#include "DPOT.h"


//////////////////////
// Globala variabler

extern xdata int	numOfCalls;
extern xdata char	speakerVolume;
extern xdata int	lastPlayed;			//Kronologindexerad


//////////////////////
// Lokala variabler

xdata char			theScreen[42*8]; 
xdata char			xPos;
xdata char			yPos;

//statiska variabler för DispLog:
xdata unsigned int  cursorPos;      //Aktuell position på markören, 0 = äldsta samtal, 1 näst äldsta osv (samtalindexerat)
xdata unsigned int  dispStart;      //Det överst synliga samtalet (samtalsindexerat)
xdata unsigned int  dispBuffStart;  //Det översta samtalet i dispBuffer (samtalsindexerat)
xdata unsigned int  dispBuffEnd;    //Den sista raden i dispBuffer som innehåller
                                    //ett legalt samtal
// MAO: Om dispBuffEnd < DISPBUFFERSIZE så gäller att:
// dispBuffStart + dispBuffEnd = är det äldsta samtalet i hela logfilen
// dispBuffStart + dispBuffEnd = är alltid det äldsta samtalet i dispBuffer
// dispBuffEnd kan aldrig vara större DISPBUFFERSIZE
xdata char          dispBuffer[DISPBUFFERSIZE * DISPCALLSIZE];  //Displaybuffern (ca 2 kByte)

xdata int           optionCursorPos; //Markören position i "options" menyn
 

//////////////////////
// Kod

void    InitializeLCD( void )
{            
//    LCDE0;

    xPos = 0;
    yPos = 0;                                                                     

    SetLCDReg(0, 0x30);
    SetLCDReg(1, 0x75);
    SetLCDReg(2, 40);
    SetLCDReg(3, 63);
    SetLCDReg(4, 0x07);
    SetLCDReg(8, 0x00);
    SetLCDReg(9, 0x00);
    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);

    cursorPos = 0;
    dispStart = 0;
    dispBuffStart = 0;

    optionCursorPos = 0;
    
    ClearDisp();
}

void    SetLCDReg(char regNr, char d)
{
    while (LCD_BUSY & 0x80) { WDT = 1; SWDT = 1; }
    LCD_INSTR = regNr;
    LCD_DATA = d;
}


void    ClearDisp( void )
{
    xdata char i, j;

    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);
    
    for (i = 0 ; i<42 ; i++) {
        for (j=0 ; j<8 ; j++) {
            SetLCDReg(12, ' ');
        }
    }
}

void    DispString(char* str)
{
    char    i;
    char    len = 0;
         
    while (str[len++] != 0);

    len--;
    
    for (i=0 ; i<len ; i++) SetLCDReg(12, str[i]);
}

void DispInfoBox(char* str1, char* str2)
//Visar en alert-box liknande ruta som kan innehålla en enkelradig sträng (str)
{
    unsigned char i = 0;
    unsigned char len1, len2, len;
    unsigned char left;

    while (str1[i++] != 0);
    len1 = i;
 
 
    i = 0;
 
    while (str2[i++] != 0);
    len2 = i;
    
    if (len1 > len2) len = len1 + 2;
    else len = len2 + 2;
    
    left = 20 - len / 2;
    
    WDT = 1;
    SWDT = 1;
 
    SetLCDReg(10, 42+left);
    SetLCDReg(11, 0x00);
    
    SetLCDReg(12, '+');
    for (i=0 ; i<len ; i++) SetLCDReg(12, '-');
    SetLCDReg(12, '+');
    
    WDT = 1;
    SWDT = 1;

    SetLCDReg(10, 84+left);
    SetLCDReg(11, 0x00);
    
    SetLCDReg(12, '|');
    SetLCDReg(12, ' '); 
    for (i=0 ; i<len1 ; i++) SetLCDReg(12, str1[i]);
    for ( ; i<len-1 ; i++) SetLCDReg(12, ' ');
    SetLCDReg(12, '|');
    
    WDT = 1;
    SWDT = 1;

    SetLCDReg(10, 126+left);
    SetLCDReg(11, 0x00);
    
    SetLCDReg(12, '|');  
    SetLCDReg(12, ' '); 
    for (i=0 ; i<len2 ; i++) SetLCDReg(12, str2[i]);
    for ( ; i<len-1 ; i++) SetLCDReg(12, ' ');
    SetLCDReg(12, '|');
    
    WDT = 1;
    SWDT = 1;

    SetLCDReg(10, 168+left);
    SetLCDReg(11, 0x00);
    
    SetLCDReg(12, '+');
    for (i=0 ; i<len ; i++) SetLCDReg(12, '-');
    SetLCDReg(12, '+');

    WDT = 1;
    SWDT = 1;
}    
    

void    WriteClipToMem(char* str, int maxLen, char* adr)
{          
    xdata int i = 0;  

    for (i=0 ; (i<maxLen) && (str[i] != 0) ; i++) {
        *(adr++) = str[i];
        WDT = 1;
        SWDT = 1;
    }
}



//-----------------------------------------------------
//  DispLog

 
  
  
void LogCursorAt(int newCursorPos, char updateAlways)
// Flyttar markören till angiven rad "newCursorPos" som är antal telefonnummer (samtalindexerad). 
{
    unsigned int    oldDispStart = dispStart;   //Kom ihåg dispStart
    int             i, j;
    unsigned int    n;

    //Måste dispBuff uppdateras?
    if ((newCursorPos < dispBuffStart) || (newCursorPos >= dispBuffStart + dispBuffEnd + 1)) {
        //Ja
        //Scrolla uppåt eller nedåt?
        if (newCursorPos > cursorPos) {
            //Nedåt
            dispBuffStart = newCursorPos - 5;
        } else {
            //Uppåt          
            if (newCursorPos < DISPBUFFERSIZE) {
                dispBuffStart = 0;
            } else {
                dispBuffStart = newCursorPos - DISPBUFFERSIZE + 6;
            }
        }
        FillDispBuffer();
    }            
        
    //Måste skärmen uppdateras eller scrollas?
    if ((updateAlways) || (newCursorPos < dispStart) || (newCursorPos > dispStart+5)) {
        //Måste skärmen scrollas?
        if ((newCursorPos < dispStart) || (newCursorPos > dispStart+5)) {
            //Scrolla till newCursorPos
            //Scrolla uppåt eller nedåt?
            if (newCursorPos > cursorPos) {
                //Nedåt
                dispStart = newCursorPos - 5;
            } else {
                //Uppåt
                dispStart = newCursorPos;
            }
        }
        n = (dispStart * DISPCALLSIZE) - (dispBuffStart * DISPCALLSIZE);
        SetLCDReg(10, 0x00);
        SetLCDReg(11, 0x00);
        for (i=0 ; i<6 ; i++) {
            SetLCDReg(12, ' ');
            for(j=0 ; j<40 ; j++) {
                SetLCDReg(12, dispBuffer[n++]);
            }
            SetLCDReg(12, ' ');
        }
    }
        
    //Flytta markörtecknet
    SetLCDReg(10, (cursorPos - oldDispStart)*42);    //Ta bort gammla
    SetLCDReg(11, 0x00);
    SetLCDReg(12, ' ');
        
    SetLCDReg(10, (newCursorPos - dispStart)*42);    //Skriv nya
    SetLCDReg(11, 0x00);
    SetLCDReg(12, '>');         
        
    cursorPos = newCursorPos; 
}            
    
void FillDispBuffer( void )
{   
    xdata FILE*     fp;
    xdata char      number[50];
    xdata char      time[15];
    xdata int       noOfMsg;
    unsigned int    n;
    int             i;
    char*           j;
    char*           jClear;
    
    DispInfoBox("Reading log file", "Please wait...");
    
    if (dispBuffStart+DISPBUFFERSIZE > numOfCalls) {
        dispBuffEnd = numOfCalls - dispBuffStart - 1;
    } else {
        dispBuffEnd = DISPBUFFERSIZE - 1;
    }

    fp = fopen("Log.txt", "r");
    
    if (fp) {
    
        //Verifierat att callIndex inte korrupt här!            
    
        for (i = dispBuffEnd ; i >= 0 ; i--) {
        //Siffran 'i' är aktuell rad i dispBuffer (nedifrån och upp)
            if (FindCall(fp, numOfCalls - dispBuffStart - i - 1)) {
//                SendRS232('.');
                WDT = 1;
                SWDT = 1;
                GetCallInformation(fp, number, time, &noOfMsg);
//                SendRS232('^');
                j = i * DISPCALLSIZE + dispBuffer;
                if ((j < dispBuffer) || (j+DISPCALLSIZE > dispBuffer + DISPBUFFERSIZE * DISPCALLSIZE)) {
                    SendStringEOLRS232("ERROR: Attempt to write outside dispBuff");
                } else {
                    jClear = j;
                    for (n=0 ; n<DISPCALLSIZE ; n++) *(jClear++) = 0;
                    WriteClipToMem(number, 24, (char*) j);
//                SendRS232('-');
                    j += 24;
                    *(j++) = ' ';
                    *(j++) = '0' + noOfMsg;
                    if ((numOfCalls - i - 1) > lastPlayed) {
                        *(j++) = 'N';
                    } else {
                        *(j++) = 'P';
                    }
                    *(j++) = ' ';
                    WriteClipToMem(time, 12, (char*) j);
//                SendRS232('=');
                }
            } else {

                SendRS232('E');

/*
                SendStringRS232("ERROR: Could not find call nr: ");
                myInt2DecStr(numOfCalls - i - 1, msg);
                SendStringEOLRS232(msg);
*/
            }
        }
    }
    fclose(fp);
}





//-----------------------------------------------------
//  Menuraden
 
void    DisplayMenu(char* menuStr)
{   
    xdata int i = 0;
    xdata int len;
    
    while(menuStr[i++] != 0);
    len = i;
    
    SetLCDReg(10, 0x26);
    SetLCDReg(11, 0x01);
    
    for (i=0 ; i<len ; i++) SetLCDReg(12, menuStr[i]);
    for ( ; i<42 ; i++) SetLCDReg(12, ' ');
}


   
//-----------------------------------------------------
//  MemUsage raden

void    DispMemUsage( void )
{   
    xdata int    j,m;
    
    SetLCDReg(10, 0xFC);
    SetLCDReg(11, 0x00);
 
    m = MemUsage();
    
    for (j=0 ; j<13 ; j++) SetLCDReg(12, '=');
    for (j=0 ; j<m ; j++) SetLCDReg(12, '#');
    for (j=0 ; j<16-m ; j++) SetLCDReg(12, '.');
    for (j=0 ; j<12 ; j++) SetLCDReg(12, '=');
}





    
    
//-----------------------------------------------------
//  OptionsMenu

void    OptionCursorAt(int  newCursorPos)
{
    SetLCDReg(10, optionCursorPos * LCD_CHAR_PER_ROW + MENU_LEFT_COLUMN - 1);
    SetLCDReg(11, 0x00);
    DispString(" o ");
    
    SetLCDReg(10, newCursorPos * LCD_CHAR_PER_ROW + MENU_LEFT_COLUMN - 1);
    SetLCDReg(11, 0x00);
    DispString("[o]");
    
    optionCursorPos = newCursorPos;
}


void    DispOptionsMenu(char answerOn, char numSignals)
{
    xdata char  i, j;
    xdata char  msg[5];

    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);
    
    for (i = 0 ; i<42 ; i++) {
        for (j=0 ; j<6 ; j++) {
            SetLCDReg(12, ' ');
        }
    }

	SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_SPKR_VOL * LCD_CHAR_PER_ROW);
    SetLCDReg(11, 0);
    DispString("o SPEAKER VOLUME [");

	for (i=0 ; i<speakerVolume ; i+=4) SetLCDReg(12, '#');
	for ( ; i<64 ; i+=4) SetLCDReg(12, '.');
	SetLCDReg(12, ']');
    
	SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_ANS_ONOFF * LCD_CHAR_PER_ROW);
    SetLCDReg(11, 0);
    DispString("o ANSWERING ");
    if (answerOn) {
        DispString("ON");
    } else {
        DispString("OFF");
    }

    SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_MEMO * LCD_CHAR_PER_ROW);	// 50
    SetLCDReg(11, 0);
    DispString("o PLAY AND RECORD MEMO");

    SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_TIMEDATE * LCD_CHAR_PER_ROW);	// 92
    SetLCDReg(11, 0);
    DispString("o SET TIME AND DATE");

    SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_NUMSIG * LCD_CHAR_PER_ROW);	// 134
    SetLCDReg(11, 0);
    DispString("o ANSWER AFTER ");
    myInt2DecStr(numSignals, msg);
    DispString(msg);
    DispString(" SIGNALS");

    SetLCDReg(10, MENU_LEFT_COLUMN + MENU_ITEM_ERASE * LCD_CHAR_PER_ROW);	// 176
    SetLCDReg(11, 0);
    DispString("o ERASE MESSAGES");
}

    
//-----------------------------------------------------
//  Set time and date

 
void    DispSetTimeDate(char cursor, char year, char month, char day, char hour, char minute)
{
    xdata char  str[45];
    xdata char  msg[5];
    xdata int   i;
    
 
    for (i=0 ; i<45 ; i++) str[i] = ' ';

    str[38] = 0; 

    str[1] = '2';
    str[2] = '0';

    myInt2DecStr00(year, msg);
    str[3] = msg[0];
    str[4] = msg[1];
    if (month == 1) {
        strcpy((char*) (str + 10), "JAN");
    } else if (month == 2) {
        strcpy((char*) (str + 10), "FEB");
    } else if (month == 3) {
        strcpy((char*) (str + 10), "MAR");
    } else if (month == 4) {
        strcpy((char*) (str + 10), "APR");
    } else if (month == 5) {
        strcpy((char*) (str + 10), "MAY");
    } else if (month == 6) {
        strcpy((char*) (str + 10), "JUN");
    } else if (month == 7) {
        strcpy((char*) (str + 10), "JUL");
    } else if (month == 8) {
        strcpy((char*) (str + 10), "AUG");
    } else if (month == 9) {
        strcpy((char*) (str + 10), "SEP");
    } else if (month == 10) {
        strcpy((char*) (str + 10), "OKT");
    } else if (month == 11) {
        strcpy((char*) (str + 10), "NOV");
    } else if (month == 12) {
        strcpy((char*) (str + 10), "DEC");
    }
    str[13] = ' ';  //Tar bort EOS-nolla ditlagd av strcpy
    
    myInt2DecStr00(day, msg);
    str[19] = msg[0];
    str[20] = msg[1];
    myInt2DecStr00(hour, msg);
    str[26] = msg[0];
    str[27] = msg[1];
    myInt2DecStr00(minute, msg);
    str[35] = msg[0];
    str[36] = msg[1];
    if (cursor == 0) {
        str[0] = '[';
        str[5] = ']';
    } else if (cursor == 1) {
        str[9] = '[';
        str[13] = ']';
    } else if (cursor == 2) {
        str[18] = '[';
        str[21] = ']';
    } else if (cursor == 3) {
        str[25] = '[';
        str[28] = ']';
    } else if (cursor == 4) {
        str[34] = '[';
        str[37] = ']';
    }               
    
    str[38] = 0; 
    
    SetLCDReg(10, 42*3 + 1);
    SetLCDReg(11, 0x00);

//    SendStringEOLRS232(str);
    
    DispString(str);    
}
 
void    DispSetTimeDateHead( void )
{
    xdata char  i, j;

    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);
    
    for (i = 0 ; i<42 ; i++) {
        for (j=0 ; j<6 ; j++) {
            SetLCDReg(12, ' ');
        }
    }

    SetLCDReg(10, 44);
    SetLCDReg(11, 0);
    DispString("YEAR    MONTH    DAY    HOUR    MINUTE");
}  
    
    
//-----------------------------------------------------
//  Fråga: Erase messages?

void    DispEraseQuest( void )
{
    xdata char  i, j;
 
    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);
    
    for (i = 0 ; i<42 ; i++) {
        for (j=0 ; j<6 ; j++) {
            SetLCDReg(12, ' ');
        }
    }

    SetLCDReg(10, 42 + 10);
    SetLCDReg(11, 0);
    DispString("Are you sure you want");

    SetLCDReg(10, 42 * 3 + 10);
    SetLCDReg(11, 0);
    DispString("to erase all messages?");
    DisplayMenu("          YES                NO");
}


void    DispMemoScreen( void )
{
    xdata char  i, j;

    SetLCDReg(10, 0x00);
    SetLCDReg(11, 0x00);
    
    for (i = 0 ; i<42 ; i++) {
        for (j=0 ; j<6 ; j++) {
            SetLCDReg(12, ' ');
        }
    }

    SetLCDReg(10, 42 + 8);
    SetLCDReg(11, 0);
    DispString("Play and record memo files");

} 
    
