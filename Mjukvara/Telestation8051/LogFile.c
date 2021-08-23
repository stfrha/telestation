/*************************************************************
Fil:            $Id: LogFile.c,v 1.5 2003/11/20 22:08:53 Gemensam Exp $

Beskrivning:    Hanterar logfilen och därtill relaterade funktioner

Skapad:         01-02-08

Ändrad:         $Date: 2003/11/20 22:08:53 $

Ägare:          Fredrik Hoffman


Att göra:

Revisionshistoria:
				$Log: LogFile.c,v $
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
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

*************************************************************/
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LogFile.h"
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
#include "DPOT.h"
#include "LCD.h"
#include "Keyboard.h"


//////////////////////
// Globala variabler

extern xdata char	msg[100];
extern xdata int	numOfCalls;
extern xdata char	speakerVolume;
extern xdata char	receivedDTMF;		//Innehåller den senaste mottagna DTMF-signalen
										//från avbrottsrutinen. Om inget tagits emot sätts
										//den till 0. För att få ett användbart värde skall
										//funktionen DecodeDTMF() köras.
extern xdata int	lastPlayed;			//Kronologindexerad

xdata long			logOffset;			//Nästa skrivposition i Log-filen 
xdata int			callIndex[1024];	//Innehåller positionen i logfilen där resp
										//samtalsnummer ligger. Index 0 är det äldsta 
										//samtalet, 1 är det näst äldsta osv (kronologindexerad)


//////////////////////
// Lokala variabler


//////////////////////
// Kod

void    InitializeLogFile( void )
{       
    logOffset = -1;
}

void    CreateLog( void )
{

    xdata DATE    dateNow;
    xdata TIME    timeNow;
    xdata char    timeStr[15];

    SendStringEOLRS232("Starting logfile...");
    GetDateNow(&dateNow);
    CreateDateString2(&dateNow, msg);
    msg[6] = ' ';
    GetTimeNow(&timeNow);
    WDT = 1;
    SWDT = 1;
    CreateTimeString(&timeNow, timeStr);

    msg[7] = timeStr[0];
    msg[8] = timeStr[1];
    msg[9] = timeStr[2];
    msg[10] = timeStr[3];
    msg[11] = timeStr[4];
    msg[12] = 0x00; 

    SendStringEOLRS232("Writing rows 1");
 
    WDT = 1;
    SWDT = 1;
    LogMessage("NWC", "Start of log");

    SendStringEOLRS232("Writing rows 2");
 
    WDT = 1;
    SWDT = 1;
    LogMessage("TIM", msg);

    SendStringEOLRS232("Writing rows 3");

    WDT = 1;
    SWDT = 1;
    LogMessage("END", NULL);

    SendStringEOLRS232("Logfile created");
}



void    LogMessage(const char* str1, const char* str2)
/*
    Skriver en rad i logfilen "Log.txt" både str1 och str2 skrivs ut med mellanslag mellan
    Raden avslutas med ny rad
*/
{
    xdata FILE*   fp;
    xdata int     i;
    
    fp = fopenA("Log.txt", 32768, logOffset);
    
    if (fp) {
        if (str1 != NULL) {
            i = 0;
            while (str1[i] != 0x00) {
                WDT = 1;
                SWDT = 1;
                fputc(fp, str1[i++]);
            }
//            SendStringEOLRS232(str1);
        }
        if (str2 != NULL) {
            fputc(fp, ' ');
            i = 0;
            while (str2[i] != 0x00) {
                WDT = 1;
                SWDT = 1;
                fputc(fp, str2[i++]);
            }
//            SendStringEOLRS232(str2);
        }
        fputc(fp, '\r');
        fputc(fp, '\n');
        
        if (fp->offset > 30000) {
            //Varna att filen håller på att ta slut
            SendStringEOLRS232("Log.txt low");
        }
        logOffset = ftell(fp);
    } else {
        SendStringEOLRS232("Could not open log file");
    }
    fclose(fp);

    WDT = 1;
    SWDT = 1;

}


char    BuildCallerList( void )
//Retunerar 1 om logfilen gick att öppna och läsa
//retunerar 0 annars
{
    xdata FILE*   fp;
    xdata long    t;

    numOfCalls = 0;
    fp = fopen("Log.txt", "r");

    if (fp) {    
        while( FindNextCall(fp) ) {
            t = ftell(fp);
/*
        SendStringRS232("Index: ");
        myInt2DecStr(numOfCalls, msg);
        SendStringRS232(msg);
        SendStringRS232(" Address: ");
        myLong2HexStr(t & 0xFFFF, msg);
        SendStringEOLRS232(msg);
*/
            callIndex[numOfCalls++] = t & 0xFFFF;
            WDT = 1;
            SWDT = 1;
        }

        logOffset = ftell(fp) - 1;
        fclose(fp);
        myInt2DecStr(numOfCalls, msg);
        SendStringRS232("nCalls: ");
        SendStringEOLRS232(msg);
        return 1; 
    } else {
        return 0;
    }
}





char    FindCall(FILE* fp, int index)
//Hittar den index:de uppringningen. Retunerar 1 om den hittas, eller 0 om den inte hittas.
//Retunerar 0 om logfilen inte kan öppnas (om den inte finns)
{
    if ((index < 0) || (index > numOfCalls)) return 0;
 
    if (fp) {
        fSetPos(fp, callIndex[index]);
/*  
        SendStringRS232("Index: ");
        myInt2DecStr(index, msg);
        SendStringRS232(msg);
        SendStringRS232(" Address: ");
        myLong2HexStr(callIndex[index], msg);
        SendStringEOLRS232(msg);
*/
        return 1;
    } else {
        return 0;
    }
}
 

char    FindNextCall(FILE* fp)
//Hittar nästa uppringning. Retunerar 1 om någon hittas, annars 0.
//OBS!: Kräver att logfilen är öppen i variabeln logFilePtr.
{
    xdata char    newCallFound = 0;
    xdata int     c = 0x00;

    newCallFound = 0;
    while ((!newCallFound) && (c != 0xFF)) {
        while ((c != 'N') && (c != 0xFF)) {
            c = fgetc(fp);
        }
        if (c == 'N') {
            if ((c = fgetc(fp)) == 'W')
                if ((c = fgetc(fp)) == 'C') 
                    if ((c = fgetc(fp)) == ' ') newCallFound = 1;
        }
    }
    return newCallFound;
}

void    GetCallInformation(FILE* fp, char* phoneNumber, char* time, int* noOfMessages)
{
    xdata char    c;
    xdata char    done;
    xdata int     i;
    xdata char    logCode[5];
    xdata int     nom = 0;

    i = 0;

    while (((c = fgetc(fp)) != '\r') && (c != 0xFF)) {
        phoneNumber[i++] = c;
        WDT = 1;
        SWDT = 1;
    }
    
    if (c == 0xFF) {
        SendStringEOLRS232("ERROR: End of Log file reached");
    }

    phoneNumber[i++] = 0;

    fgetc(fp);                                      //Ät upp '\n'-tecknet

    done = 0;

    while (!done) {
    
        WDT = 1;
        SWDT = 1;
    
        logCode[0] = fgetc(fp);
        logCode[1] = fgetc(fp);
        logCode[2] = fgetc(fp);
        logCode[3] = 0;
        fgetc(fp);
    
        if (!myStrCmpNoCase(logCode, "TIM")) {
            i = 0;
            while ((c = fgetc(fp)) != '\r') {
                time[i++] = c;
            }
            time[i++] = 0;
            fgetc(fp);                                      //Ät upp '\n'-tecknet
        } else if (!myStrCmpNoCase(logCode, "RCM")) {
            nom++;
            while (fgetc(fp) != '\r');
            fgetc(fp);                                  //Ät upp '\n'-tecknet
        } else if (!myStrCmpNoCase(logCode, "END")) {
            done = 1;
        } else if (!myStrCmpNoCase(logCode, "DSC")) {
            done = 1;
        } else if (logCode[0] == 0xFF) {
            done = 1;
        } else if (fp->offset >= fp->length) {
            done = 1;
        } else {
            while (fgetc(fp) != '\n');
        }
    }    
    *noOfMessages = nom; 
}


char    PlayThisCall(int clIndex)
//Retunerar 1 om callIndex motsvarar ett existaerande meddelande, 0 annars
//clIndex är kronologindexerad
{
    xdata char      file[30];
    xdata char      done;
    xdata char      c;
    xdata char      logCode[5];
    xdata int       i;
    xdata FILE*     fp;

    fp = fopen("Log.txt", "r");

    if (FindCall(fp, clIndex)) {
        while ((c = fgetc(fp)) != '\n');        //Äter upp telefonnummret
        
        done = 0;
        
        while (!done) {

            logCode[0] = fgetc(fp);
            logCode[1] = fgetc(fp);
            logCode[2] = fgetc(fp);
            logCode[3] = 0;
            fgetc(fp);
    
            if (!myStrCmpNoCase(logCode, "RCM")) {
                i = 0;
                while ((c = fgetc(fp)) != '\r') {
                    file[i++] = c;
                }
                file[i++] = 0;
                fgetc(fp);                                      //Ät upp '\n'-tecknet

                DispInfoBox("Now playing:", file);
                SpeakerVolume(speakerVolume);
                PhoneInVolume(0);
                PlayFile(file);   
                SpeakerVolume(0);
                PhoneInVolume(63);
                receivedDTMF = 0;

            } else if (!myStrCmpNoCase(logCode, "END")) {
                done = 1;
            } else if (!myStrCmpNoCase(logCode, "DSC")) {
                done = 1;
            } else if (logCode[0] == 0xFF) {
                done = 1;
            } else if (fp->offset >= fp->length) {
                done = 1;
            } else {
                while (fgetc(fp) != '\n');
            }
        }
        c = 1;
    } else {
        c = 0;
    }
    fclose(fp);
    return c;
}


void    PlayNewMessages( void )
{
    xdata int i;

    i = lastPlayed;
         
    do {
        i++;
        WDT = 1;
        SWDT = 1;
    } while (PlayThisCall(i));
    
}











 
