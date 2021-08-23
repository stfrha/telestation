/*************************************************************
Fil:            $Id: wav.c,v 1.5 2003/12/26 16:17:01 Gemensam Exp $

Beskrivning:    In- och uppspelning av WAV-filer. Tar över hanteringen
                av filsystemet under in- och uppspelning.

Skapad:         99-10-26

Ändrad:         $Date: 2003/12/26 16:17:01 $

Ägare:          Fredrik Hoffman


Att göra:

*   Fel felmeddelande 0x14 om man skriver TYPE (utan filnamn) eftersom nollan 
    i parameter två stämmer med nollan i filnamnet på en ledig  filstruktur, fixa!

Revisionshistoria:
				$Log: wav.c,v $
				Revision 1.5  2003/12/26 16:17:01  Gemensam
				Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
				Fixed PPD detection by decreasing phone volume from 63 to 15.
				Version 0304.
				
				Revision 1.4  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.3  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.2  2003/11/10 22:44:46  Gemensam
				Added header information
				

*************************************************************/
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Wav.h"
#include "commands.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "RS232.h"
#include "Main.h"
#include "Keyboard.h"
#include "Phone.h"
#include "DTMF.h"
#include "Phone.h"
 

//////////////////////
// Globala variabler

extern xdata long	writeAddress;		//Innehåller den address som en skriv-operation kommer att använda.
extern xdata int	FLASHWriteState;	//Håller reda på i vilket tillstånd skriv-sekvensen befinner sig enl:
										// OFF = SetupWriteSeq ej körd, skrivning ej möjlig.
										// READY = SetupWriteSeq har körts, data kan läggas in i registret.
										// NEMPTY = Registret innehåller data, skrivning till celler kan göras.
										// WRITING = Programmeringen har startats (250 us). Inför nästa 
										//           skrivning måste SetupWriteSeq köras.
extern xdata char	receivedDTMF;		//Innehåller den senaste mottagna DTMF-signalen
										//från avbrottsrutinen. Om inget tagits emot sätts
										//den till 0. För att få ett användbart värde skall
										//funktionen DecodeDTMF() köras.
extern xdata char	lastKey;
extern xdata char	terminateAnswer;
extern xdata char	callerHungUp;
extern xdata char	parallellPhoneDetected;


//////////////////////
// Lokala variabler

xdata char          WAVBuffer[WAVBUFSIZE];
idata char          wavState;
idata char xdata *  nextWrite;
idata char xdata *  nextRead;
idata char          sample;
idata unsigned char wavAbort;
idata unsigned char daCount0;
idata unsigned char daCount1;
idata unsigned char daCount2;
idata unsigned char daCount3;
idata unsigned char daComp0;
idata unsigned char daComp1;
idata unsigned char daComp2;
idata unsigned char daComp3;


//////////////////////
// Kod

void    InitializeWAV( void )
{
    xdata char    i;
    
    wavState = WS_NO_OP;    
    TR1 = 0;                                //Stänger av timer 1

    ET1 = 0;                                //Omöjliggör avbrott från Timer 1

    i = TMOD;
    
    TMOD = (i & 0x0F) + 0x20;                //Sätter TIMER1 = Mode 2, timer operation, no gating

    TH1 =  132;                             //Sätter reload värde på 115
                                            //115 / (fosc/12) = 124,8us => fs = 8014 Hz

    ADEX = 0;                               //Intern AD-omvandlingstrigger
    ADM = 1;                                //Enkel (inte kontinuerlig) AD-omvandling
} 

void    SetupMic( void ) 
{
    ADCON1 = 0x08;                          //Sätter MIC = analog ingång P8.0 samt "/4" prescaler
}

void    SetupPhoneIn( void ) 
{
    ADCON1 = 0x09;                          //Sätter PHONE = analog ingång P8.1 samt "/4" prescaler
}

void    RecMicFile(const char* fileName)
{
    SetupMic();
    wavState = WS_REC;   
    RecFile(fileName);
}

void    RecPhoneFile(const char* fileName)
{
    SetupPhoneIn();
    wavState = WS_REC;   
    RecFile(fileName);
}

/*
void    RecPort1(const char* fileName)
{
    wavState = WS_PORT1;
    RecFile(fileName);
}
*/

void    RecFile(const char* fileName)
{
    FILE*       fptr;

//Nedan var idata....
    xdata unsigned char device;
    xdata unsigned char pageStart0;
    xdata unsigned char pageStart1;
    xdata unsigned int  pageStart;
    xdata unsigned char v;
//...till hit    
    
    xdata unsigned long tmpLen;
    xdata unsigned long longPage;
    xdata unsigned long l_ps0;
    xdata unsigned long l_ps1;
    
//    xdata char  msg[100]; 
//    xdata int  l;
    
    
    fptr = fopen(fileName, "w");
    
    if (fptr) {
        nextWrite = WAVBuffer;
        nextRead = WAVBuffer;
        wavAbort = 0; 
        device = (writeAddress >> 22) & 0x03;
        pageStart = (writeAddress >> 9) & 0x01FFF;      //Startaddressen / WAVBUFSIZE
        pageStart1 = pageStart >> 8;
        pageStart0 = pageStart;
        FLASHWriteState = NEMPTY;
        S1CON &= ~(1);                                  //Nollar RS232 mottagnings-interrupt-flagga          
        receivedDTMF = 0;
        KeyboardOn();
        lastKey = 0;
 
        ET1 = 1;                                        //Möjliggör avbrott från timer 1
        ADDATL = 0;                                     //Starta första AD-omvandlingen
        TR1 = 1;                                        //Starta timer 1
        while (!wavAbort) {
            WDT = 1;
            SWDT = 1;
            if (nextRead != nextWrite) {
                v = *(nextRead++);                      //= WAVBuffer[dataFieldCounter++];
                FLASH = v;
                if (nextRead == (WAVBuffer + WAVBUFSIZE)) {
                    nextRead = WAVBuffer;
                    CLE = 1;                                        //Sätter minnet i kommandomod
                    FLASH = 0x10;                                   //Startar programmering av FLASH-celler. Tar ca 250 us enl. datablad.
//Här finns tid att göra mer!!!
                    if (!(++pageStart0)) ++pageStart1;
                    if (pageStart1 == 32) {                         //pageStart == 8192
                        pageStart1 = 0;                             //pageStart0 är redan 0-ställd
                        device++;
                        FREG = device + 0x08;                       //Väljer rätt FLASH-chip och sätter dess CE till 0
                    }
                    if (device > 2) wavAbort = 1;
                    if (S1CON & 0x01) wavAbort = 1;
                    if (terminateAnswer) wavAbort = 1;
                    if (receivedDTMF) wavAbort = 1;
                    if (lastKey) wavAbort = 1;
//Till hit!!!!!!
                    while (!FBUSY);
 
                    FLASH = 0x80;                                       //Sätter minnet i programmeringsmod

                    CLE = 0;                                            //Sätter minnet i adresseringsmod
                    ALE = 1;
    
                    FLASH = 0x00;                                       //Börjar alltid i början på en page
                    FLASH = pageStart0;                                 // A9 - A16
                    FLASH = pageStart1;                                 //A17 - A21
            
                    ALE = 0;                                            //Sätter minnet i datamod
                }
            }
        }
        TR1 = 0;
        ET1 = 0;
        wavState = WS_NO_OP;
        pageStart = (pageStart1 * 256) +  pageStart0;

        l_ps0 = pageStart0;
        l_ps1 = pageStart1;

//        longPage = (pageStart * 0x200) & 0x1FFFE00;
        longPage = (l_ps1 * 0x20000);// & 0x1FE0000;
        longPage += (l_ps0 * 0x200);// & 0x1FE00;                    //= pageStart * 512

/* 
////////////////////////////////////////////////////////
//Debug area
        l = (fptr->startAddress >> 16) & 0xFFFF;
        sprintf(msg, "Start: 0x%04X", l);
        SendStringRS232(msg);
        l = fptr->startAddress & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
        
        l = device;
        sprintf(msg, "device: %02X", l);
        SendStringEOLRS232(msg);
        
        l = pageStart0;
        sprintf(msg, "pageStart0: %02X", l);
        SendStringEOLRS232(msg);

        l = pageStart1;
        sprintf(msg, "pageStart1: %02X", l);
        SendStringEOLRS232(msg);

        l = pageStart;
        sprintf(msg, "pageStart: %04X", l);
        SendStringEOLRS232(msg);

        l = (l_ps0 >> 16) & 0xFFFF;
        sprintf(msg, "l_ps0: 0x%04X", l);
        SendStringRS232(msg);
        l = l_ps0 & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
        
        l = (l_ps1 >> 16) & 0xFFFF;
        sprintf(msg, "longPage: 0x%04X", l);
        SendStringRS232(msg);
        l = l_ps1 & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
        
        l = (longPage >> 16) & 0xFFFF;
        sprintf(msg, "longPage: 0x%04X", l);
        SendStringRS232(msg);
        l = longPage & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
        
        l = ((int)(nextRead - WAVBuffer)) & 0xFFFF;
        sprintf(msg, "nextRead-WAVBuf: %04X", l);
        SendStringEOLRS232(msg);
        
*/         
        tmpLen = device * 0x400000;
/*        
        l = (tmpLen >> 16) & 0xFFFF;
        sprintf(msg, "tmpLen0: 0x%04X", l);
        SendStringRS232(msg);
        l = tmpLen & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/        
        tmpLen += longPage;
/*        
        l = (tmpLen >> 16) & 0xFFFF;
        sprintf(msg, "tmpLen1: 0x%04X", l);
        SendStringRS232(msg);
        l = tmpLen & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/        
        tmpLen += ((int)(nextRead - WAVBuffer));
/*        
        l = (tmpLen >> 16) & 0xFFFF;
        sprintf(msg, "tmpLen2: 0x%04X", l);
        SendStringRS232(msg);
        l = tmpLen & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/        
        tmpLen -= fptr->startAddress;
/*
        l = (tmpLen >> 16) & 0xFFFF;
        sprintf(msg, "tmpLen3: 0x%04X", l);
        SendStringRS232(msg);
        l = tmpLen & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/
        fptr->length = tmpLen;

/*
        l = (fptr->length >> 16) & 0xFFFF;
        sprintf(msg, "Length: 0x%04X", l);
        SendStringRS232(msg);
        l = fptr->length & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/
////////////////////////////////////////////////////////
 
        fclose(fptr);
        S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          
//        KeyboardOff();
    }
}



void    PlayFile(const char* fileName)
{
    xdata FILE*       fptr;

    fptr = fopen(fileName, "r");
    
//    SendStringRS232("Playing file: ");
 
    
    if (fptr) {

//        SendStringEOLRS232(fileName);

        wavState = WS_PLAY;
        wavAbort = 0; 
        daComp0 = (fptr->length + fptr->startAddress) & 0xFF;
        daComp1 = ((fptr->length + fptr->startAddress) >> 8) & 0xFF;
        daComp2 = ((fptr->length + fptr->startAddress) >> 16) & 0xFF;
        daCount0 = fptr->startAddress & 0xFF;
        daCount1 = (fptr->startAddress >> 8) & 0xFF;
        daCount2 = (fptr->startAddress >> 16) & 0xFF;
        sample = 128;
        S1CON &= ~(1);                                  //Nollar RS232 mottagnings-interrupt-flagga
//        receivedDTMF = 0;
        KeyboardOn();
        lastKey = 0;
        ET1 = 1;                                        //Möjliggör avbrott från timer 1
        ADDATL = 0;                                     //Starta första AD-omvandlingen
        TR1 = 1;                                        //Starta timer 1
        while (!wavAbort) {
            WDT = 1;
            SWDT = 1;
            if (daCount2 == 128) wavAbort = 1;             //Slutet på FLASH-minnet
            if (S1CON & 0x01) wavAbort = 1;
            if (terminateAnswer) wavAbort = 1;
            if (receivedDTMF) wavAbort = 1;
            if (lastKey) wavAbort = 1;
            if ((daComp2 <= daCount2) && (daComp1 <= daCount1) && (daComp0 <= daCount0)) wavAbort = 1;
        }
        TR1 = 0;
        ET1 = 0;
        wavState = WS_NO_OP;

        fclose(fptr);
        S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          

//        KeyboardOff();

#ifdef __WAW_DEBUG__
		if (wavAbort) {
			SendStringRS232("Aborted because: ");
			if (receivedDTMF) SendStringEOLRS232("DTMF");
			else if (lastKey) SendStringEOLRS232("Key pressed");
			else if (daCount2 == 128) SendStringEOLRS232("Out of memory");
			else if (S1CON & 0x01) SendStringEOLRS232("Received RS232");
			else if (terminateAnswer) {
				if (callerHungUp) SendStringEOLRS232("Caller hung up");
				else if (parallellPhoneDetected) SendStringEOLRS232("Parallell phone detected");
				else SendStringEOLRS232("Terminated by unknown causes");
			} 
			else SendStringEOLRS232("Other");
		}
#endif
    }
}        



void    EchoMic( void )
{
    SetupMic();
    Echo();
}

 
void    EchoPhone( void )
{
    SetupPhoneIn();
    Echo();
}

void    Echo( void )
{
    wavState = WS_ECHO;
    S1CON &= ~(1);                      //Nollar RS232 mottagnings-interrupt-flagga          
    ET1 = 1;                            //Möjliggör avbrott från timer 1
    ADDATL = 0;                         //Starta första AD-omvandlingen
    TR1 = 1;                            //Starta timer 1
    while (!(S1CON & 0x01)) {
        WDT = 1;
        SWDT = 1;
    }
    TR1 = 0;
    ET1 = 0;
    wavState = WS_NO_OP;
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          
}

 
void    WavTone( void )
{
    wavState = WS_TONE;
    sample = 128;
    S1CON &= ~(1);                      //Nollar RS232 mottagnings-interrupt-flagga          
    ET1 = 1;                            //Möjliggör avbrott från timer 1
    ADDATL = 0;                         //Starta första AD-omvandlingen
    TR1 = 1;                            //Starta timer 1
    while (!(S1CON & 0x01)) {
        WDT = 1;
        SWDT = 1;
    }
    TR1 = 0;
    ET1 = 0;
    wavState = WS_NO_OP;
    DAC0 = 128;
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          
}





interrupt [0x1B] void T1_int (void)
{
    char    v;

    if (wavState == WS_REC) {
        v = ADDATH;
        *(nextWrite++) = v;                         //WAVBuffer[nextWrite++] = v;
        if (nextWrite == (WAVBuffer + WAVBUFSIZE)) {
            nextWrite = WAVBuffer;
        }
        ADDATL = 0;                                 //Startar ny AD-omvandling
    } else if (wavState == WS_PLAY) {
        DAC0 = sample;
        sample = FLASH;
        if (!++daCount0)
            if (!++daCount1) {
                ++daCount2;
                if (daCount2 == 64) {
                    FREG = 0x01 + 0x08;            //Väljer rätt FLASH-chip och sätter dess CE till 0
                    ALE = 0;                        //Sätter minnet i Kommandomod
                    CLE = 1;
    
                    FLASH = 0;                     //Adresserar första sidhälften med 
                                           //kommandokod 0x00
    
                    CLE = 0;                       //Sätter minnet i adresseringsmod
                    ALE = 1;
    
                    FLASH = 0;                      // A0 - A7 OBS: A8 skriven i kommandot ovan
                    FLASH = 0;                      // A9 - A16
                    FLASH = 0;                      //A17 - A21

                    ALE = 0;                        //Sätter minnet i datamod
    
 
//OBS: Väntan på att FLASH-minnet skall föra över data från FLASH-arean till registerarean görs
//inte här! Endast väntan tills BUSY aktiveras (går låg) görs här. Processorn kan sedan jobba 
//under tiden som FLASH-minnet jobbar. Detta gör att innan varje läsning måste BUSY-flaggan kollas.

//            while (FBUSY);                   //Väntar tills busy går låg (max 100 ns enligt datablad)
 
                }
            }
        while(!FBUSY);
/*
    } else if (wavState == WS_PORT1) {
        v = P1;
        *(nextWrite++) = v;                         //WAVBuffer[nextWrite++] = v;
        if (nextWrite == (WAVBuffer + WAVBUFSIZE)) {
            nextWrite = WAVBuffer;
        }
*/
    } else if (wavState == WS_TONE) {
        if (sample == 127) sample = 0;
        else if (sample == 0) sample = 128;
        else if (sample == 128) sample = 255;
        else if (sample == 255) sample = 127;
        DAC0 = sample;
    } else if (wavState == WS_ECHO) {
        DAC0 = ADDATH;
        ADDATL = 0;

    }
}

