/*************************************************************
Fil:            $Id: DTMF.c,v 1.4 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Styr och kontrollerar DTMF-kretsen

Skapad:         99-11-03

Ändrad:         $Date: 2003/11/20 22:08:52 $

Ägare:          Fredrik Hoffman


Att göra:


Revisionshistoria:
				$Log: DTMF.c,v $
				Revision 1.4  2003/11/20 22:08:52  Gemensam
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
#include "DTMF.h"
#include "commands.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "Main.h"
#include "RS232.h"
#include "DPOT.h"


//////////////////////
// Globala variabler

xdata char    receivedDTMF;                     //Innehåller den senaste mottagna DTMF-signalen
												//från avbrottsrutinen. Om inget tagits emot sätts
                                                //den till 0. För att få ett användbart värde skall
                                                //funktionen DecodeDTMF() köras.

xdata char    DTMFBuffer[50];					//Innehåller de senaste mottagna tecknen
xdata char    DTMFSetIndex;
xdata char    DTMFGetIndex;

//////////////////////
// Lokala variabler

//////////////////////
// Kod

void    InitializeDTMF( void )
{             
    xdata char    dummy;

    DTMFVolume(0);
 
    dummy = DTMF_READ;                                  //DTMF Initialiseringsprocedur
    DTMF_CONTROL = 0x00;
    DTMF_CONTROL = 0x00;
    DTMF_CONTROL = 0x08;
    DTMF_CONTROL = 0x00;
    dummy = DTMF_READ;
    
    DTMF_CONTROL = 0x04;                                //Möjliggör avbrott från DTMF-chip
    
    receivedDTMF = 0;
    
    IT0 = 1;                                            //Externavbrott 1 negativ flanktriggad
    EX0 = 1;                                            //Möjliggör externavbrott 1
    
    DTMFSetIndex = 0;
    DTMFGetIndex = 0;
} 







void MakeDTMFTones(const char *tones)
{
    xdata char            tone;
    xdata unsigned char   i = 0;

    DTMFVolume(63);
    
    DTMF_CONTROL = 0x09;                                //"Tone out", DTMF, IRQ disable, Välj register B
    DTMF_CONTROL = 0x00;                                //Burst-mode

    tone = tones[i++];
    while (tone != 0) { 
        switch (tone) {
        case '0': 
            DTMF_TRANS = 10; 
            break;
        case '1': 
            DTMF_TRANS = 1; 
            break;
        case '2': 
            DTMF_TRANS = 2; 
            break;
        case '3': 
            DTMF_TRANS = 3; 
            break;
        case '4': 
            DTMF_TRANS = 4; 
            break;
        case '5': 
            DTMF_TRANS = 5; 
            break;
        case '6': 
            DTMF_TRANS = 6; 
            break;
        case '7': 
            DTMF_TRANS = 7; 
            break;
        case '8': 
            DTMF_TRANS = 8; 
            break;
        case '9': 
            DTMF_TRANS = 9; 
            break;
        case '*': 
            DTMF_TRANS = 11; 
            break;
        case '#': 
            DTMF_TRANS = 12; 
            break;
        case 'A': 
            DTMF_TRANS = 13; 
            break;
        case 'B': 
            DTMF_TRANS = 14; 
            break;         
        case 'C': 
            DTMF_TRANS = 15; 
            break;
        case 'D': 
            DTMF_TRANS = 0; 
            break;
        }
        SendRS232(tone);
        while (!(DTMF_STATUS & 0x02)) {
            WDT = 1;
            SWDT = 1;
        }
        tone = tones[i++];
    }

    DTMF_CONTROL = 0x04;                                //Möjliggör avbrott från DTMF-chip
    DTMFVolume(0);
}


/*
char    WaitForDTMF( void )
{
    char tmp;

    while (!receivedDTMF) {
        WDT = 1;
        SWDT = 1;
    }
    
//    SendStringRS232("Received: ");
    tmp = DecodeReceivedDTMF();
//    SendRS232(tmp);
//    SendStringEOLRS232(" ");
 
    return tmp;
}
*/

char    GetNextDTMF( void )
//Retunerar nästa avkodade tecknet i buffer eller 0 om buffern är tom
{
    if ((DTMFGetIndex == DTMFSetIndex) || (DTMFGetIndex > 50)) return 0;

    return DecodeDTMF(DTMFBuffer[DTMFGetIndex++]);
}

void    ResetDTMFIndex( void )
{
    DTMFSetIndex = 0;
    DTMFGetIndex = 0;
}

char    DecodeReceivedDTMF( void )
{
    char tmp;
    
    tmp = DecodeDTMF(receivedDTMF);
    if (tmp != 'e') receivedDTMF = 0;
    return tmp;
}


char    DecodeDTMF(char c)
//Omvandlar innehållet i receivedDTMF till ett tecken och retunerar det. Om receivedDTMF
//inte innehåller ett legalt tecken retuneras 0x00. 
{           
    switch (c) {
    case 0xF1 :
        return '1';
    case 0xF2 : 
        return '2';
    case 0xF3 : 
        return '3';
    case 0xF4 : 
        return '4';
    case 0xF5 : 
        return '5';
    case 0xF6 : 
        return '6';
    case 0xF7 : 
        return '7';
    case 0xF8 : 
        return '8';
    case 0xF9 : 
        return '9';
    case 0xFA : 
        return '0';
    case 0xFB : 
        return '*';
    case 0xFC : 
        return '#';
    case 0xFD : 
        return 'A';
    case 0xFE : 
        return 'B';
    case 0xFF : 
        return 'C';
    case 0xF0 : 
        return 'D';
    default :
        return 'e';
    }
}


interrupt [0x03] void EX0_int (void)
{
    if (DTMF_STATUS & 0x04) {                               //Finns det ett mottaget tecken?
        receivedDTMF = DTMF_READ;                           //Hämta det mottagna DTMF-tecknet
//        SendRS232(receivedDTMF);
        if (DTMFSetIndex < 50) DTMFBuffer[DTMFSetIndex++] = receivedDTMF;
//        while (!(DTMF_STATUS & 0x08)) {                     //Vänta tills DTMF-tonen försvinner
//            WDT = 1;
//            SWDT = 1;
//        }
    }
}





