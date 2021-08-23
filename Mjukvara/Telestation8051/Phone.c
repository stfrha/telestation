/*************************************************************
Fil:            $Id: Phone.c,v 1.12 2003/12/26 16:17:01 Gemensam Exp $

Beskrivning:    Styr och kontrollerar telefongr�nsnittet

Skapad:         99-11-08

�ndrad:         $Date: 2003/12/26 16:17:01 $

�gare:          Fredrik Hoffman

Att g�ra:

Revisionshistoria:
				$Log: Phone.c,v $
				Revision 1.12  2003/12/26 16:17:01  Gemensam
				Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
				Fixed PPD detection by decreasing phone volume from 63 to 15.
				Version 0304.
				
				Revision 1.11  2003/12/09 20:52:49  Gemensam
				Commented out PPD detection since the HW not work yet.
				Ver 0302.
				
				Revision 1.10  2003/11/29 22:36:00  Gemensam
				Fixed problem with to short pulse width on PPD.
				Ver 0302.
				
				Revision 1.9  2003/11/24 22:44:36  Gemensam
				Fixed PPD, not working yet,
				New command: speaker volume.
				Version 0301.
				
				Revision 1.8  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.7  2003/11/23 20:40:29  Gemensam
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
				
				Revision 1.3  2003/11/10 23:41:41  Gemensam
				Fixed header
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

*************************************************************/

//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Phone.h"
#include "commands.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "Main.h"
#include "RS232.h"
#include "Timer.h"
#include "LogFile.h"
#include "DPOT.h"
#include "Main.h"
#include "DTMF.h"
#include "LCD.h"


//////////////////////
// Globala variabler

extern xdata char	speakerVolume;
extern xdata char	receivedDTMF;		//Inneh�ller den senaste mottagna DTMF-signalen
										//fr�n avbrottsrutinen. Om inget tagits emot s�tts
										//den till 0. F�r att f� ett anv�ndbart v�rde skall
										//funktionen DecodeDTMF() k�ras.
extern xdata int	error;				//Error-kod

xdata char			terminateAnswer;
xdata char			callerHungUp;
xdata char			parallellPhoneDetected;


//////////////////////
// Lokala variabler

//////////////////////
// Kod

void InitializePhone( void )
{
    terminateAnswer = 0;
	callerHungUp = 0;
	parallellPhoneDetected = 0;
    SpeakerVolume(0);
    PhoneOutVolume(0);
#ifdef __PPD_ON__
	SendStringEOLRS232("PPD on");
#endif
}

void HangUp( void )
{
    SpeakerVolume(0);
    PhoneOutVolume(0);
    EX4 = 0;								//Disable hangup IRQ
#ifdef	__PPD_ON__
	EX3 = 0;								//Disable parallell phone detection IRQ
#endif
    PS_LC = 1;								//Hang up
}

void LiftPhone( void )
{
    SpeakerVolume(speakerVolume);
    PhoneOutVolume(DEFAULT_PHONE_OUT_VOLUME);		
    receivedDTMF = 0;						//Fult men funkar tills vidare
    terminateAnswer = 0;
	callerHungUp = 0;
	parallellPhoneDetected = 0;
    PS_LC = 0;								//Lift phone
#ifdef	__PPD_ON__
	ClearPPDRipple();
#else
	WaitFor(50);							// If PPD is not on, this delay must be used!!
#endif
    IEX4 = 0;								//Clear possible triggered IRQ
    EX4 = 1;								//Enable hangup IRQ
#ifdef	__PPD_ON__
	IEX3 = 0;								//Clear possible triggered IRQ
	EX3 = 1;								//Enable parallell phone detection IRQ
#endif
}

#ifdef	__PPD_ON__

void ClearPPDRipple( void )
// V�ntar tills PPD har g�tt h�g och ev ripple har f�rsvunnit. 
// PPD m�ste ligga h�g i 10ms f�r att ripplet skall anses vara borta.
// Om PPD inte g�r h�g inom 2 sek s� avbryts funktionen i alla fall.
// Detta kommer i s� fall leda till interrupt.
{
	xdata char	twoSecTimer;
	xdata char	tenMiliTimer;

	twoSecTimer = StartTimer(200);			//Starta timer p� tv� sekunder
	if (0 == twoSecTimer) {
		error = EM_TOOMANYTIMERS;			//Ingen timer finns ledig. Eftersom det inte g�r 
		WaitFor(100);						//att f�lja PPD med timeout s� v�ntas en sekund vilket
											//verkar vara mer �n dubbla tiden som beh�vs.
		return;								//Sedan avslutas rutinen.
	}
	while (!TimedOut(twoSecTimer) && 0 == PS_PPD) {
		WDT = 1;
		SWDT = 1;
	}
	if (TimedOut(twoSecTimer)) {
		KillTimer(twoSecTimer);				//PPD gick aldrig h�g (skall inte kunna h�nda)
		return;
	}
	KillTimer(twoSecTimer);

	do {
		tenMiliTimer = StartTimer(10);		//Anta ledig timer (vi sl�ppte precis en!!!)
		while((1 == PS_PPD) && (!TimedOut(tenMiliTimer))) {
			WDT = 1;
			SWDT = 1;
		}
		if (TimedOut(tenMiliTimer)) {
			KillTimer(tenMiliTimer);		//Inget ripple kvar, returnera.
			WaitFor(1);						//(men f�rst avvakta tio milli till f�r s�kerhets skull)
			return;							
		}
		KillTimer(tenMiliTimer);			//En l�g niv� �r detekterad, st�ng av timer
		while (0 == PS_PPD) {				//V�nta tills PPD g�r h�g igen
			WDT = 1;
			SWDT = 1;
		}
	} while (1);							//H�ll p� tills tio millisekunder har g�tt
}
#endif

int WaitForRingSignals(int  nSignals)
/*
Retunerar:
0 = nSignals korrekta signaler mottagna
1 = Avbruten
3 = �vrigt fel se "error"

Hur definieras en signal:

20-30 pulser med pulsbredden 40 ms (f = 25 Hz) f�ljt av minst 
5 sekunder nollor. Kolla i telespec!!!! 
*/
{
    xdata char    sampleCount;
    xdata char    pulseCount;
    xdata char    signalFound;
    xdata char    signalCount = 0;
    xdata char    pulseTimer = 0;
    xdata char    signalTimer = 0;
 

//    SendStringRS232("Signals: ");
//    SendRS232('0' + nSignals);
//    SendStringEOLRS232(" ");
    

    do {
        signalFound = 0;                       
        pulseCount = 0;                       
        sampleCount = 0;

        signalTimer = StartTimer(WRS_SIGNAL_MAX_INTERVALL);
        if (!signalTimer) {
            error = EM_TOOMANYTIMERS;
            return 3;
        }
        
        while ((sampleCount < WRS_POS_EDGE_TRIGG) && (!TimedOut(signalTimer)) && (!(S1CON & 0x01))) {          //Letar positiv flank
            if (PS_RV) {
                sampleCount++;
            } else {
                sampleCount = 0;
            }
            WDT = 1;
            SWDT = 1;
        }
        
        if (TimedOut(signalTimer)) {
            KillTimer(signalTimer);
            SendStringEOLRS232("Timeout");
            return 1;       //Det tog f�r l�ng tid innan signalen b�rjade, samtalet avbrutet
        }
        KillTimer(signalTimer);
        signalTimer = 0;



//Positiv flank


        do {
            if (!(S1CON & 0x01)) {

//            SendRS232('P');
                sampleCount = 0;

                while (sampleCount < WRS_NEG_EDGE_TRIGG) {          //Letar negativ flank
                    if (!PS_RV) {
                        sampleCount++;
                    } else {
                        sampleCount = 0;
                    }
                    WDT = 1;
                    SWDT = 1;
                }        
//            SendRS232('N');
    
                pulseCount++;                                       //En pos och en neg flank ger en puls
    
                pulseTimer = StartTimer(WRS_POST_SIGNAL_LIMIT);          //200 ms
    
                if (!pulseTimer) {
                    error = EM_TOOMANYTIMERS;
                    return 3;
                }

//                SendRS232(pulseTimer + '0');
    
                sampleCount = 0;
                while ((sampleCount < WRS_POS_EDGE_TRIGG) && (!TimedOut(pulseTimer)) && (!(S1CON & 0x01))) {
                    if (PS_RV) {
                        sampleCount++;
                    } else {
                        sampleCount = 0;
                    }
                    WDT = 1;
                    SWDT = 1;
                }
        
                if (TimedOut(pulseTimer)) {
                    signalFound = 1;
                }

                KillTimer(pulseTimer);
                pulseTimer = 0;
            }
        } while ((!signalFound) && (!(S1CON & 0x01)));
 
        signalCount++;
//        SendStringEOLRS232("A signal");
        
    } while ((signalCount < nSignals) && (!(S1CON & 0x01)));

    if (pulseTimer) KillTimer(pulseTimer);
    if (signalTimer) KillTimer(signalTimer);

    if (S1CON & 0x01) {
        return 1;
    } else {
        WDT = 1;
        SWDT = 1;
//        SendStringEOLRS232("Ok signals");
        return 0;
    }
}





interrupt [0x5B] void EX4_int (void)
//En positiv flank p� LCD triggar detta avbrott. Detta skall detektera att uppringaren har
//lagt p� luren. Avbrottet skall bara vara aktiverat n�r luren �r av och deaktiveras n�r
//luren l�ggs p�.
{
    xdata char  filterCount = 0;

    while (filterCount < P_RIPPLE_FILTER_TIMEOUT) {		//V�nta tills eventuellt rippel har f�rsvunnit
        WDT = 1;
        SWDT = 1;
        if (PS_LCD == 1) filterCount = 0;
        else filterCount++;
    }

    EX4 = 0;						//Disable interrupt until next answer
    terminateAnswer = 1;
	callerHungUp = 1; 
}


#ifdef	__PPD_ON__

interrupt [0x53] void EX3_int (void)
//En negativ flank p� "parallell phone detection" (PPD) signalen triggar detta avbrott. 
//Detta skall detektera att en annan telefon har lyft p� luren. En negativ flank kan
//komma vid andra tillf�llen ocks�. Endast om pulsen �r tillr�ckligt l�ng s� s�tts 
//terminateAnswer flaggan.
//Avbrottet skall bara vara aktiverat n�r luren �r av och deaktiveras n�r luren l�ggs p�.
//Avbrottet f�ljer PPD signalen och sl�pper den inte f�rr�ns den har varit h�g i 10 loopar
//f�r att eleminera eventuella glitchar.
{
	idata char	filterCount = 0;
	idata int	pulseWidth = 0;
#ifdef __DEBUG_PHONE__
	xdata char	msg[5];
#endif

	while (filterCount < P_RIPPLE_FILTER_TIMEOUT) {
        WDT = 1;
        SWDT = 1;
		if (0 == PS_PPD) filterCount = 0;
		else filterCount++;
		pulseWidth++;
	}

#ifdef __DEBUG_PHONE__
	pulseWidth -= P_RIPPLE_FILTER_TIMEOUT;			// Ta bort den extra PW som l�ggs p� av ripplefiltret.
	myLong2HexStr(pulseWidth, msg);
    SendStringRS232("PPD Pulsewidth: ");
    SendStringEOLRS232(msg);
#endif


	if (pulseWidth > P_PPD_PW) {
	    terminateAnswer = 1;
		parallellPhoneDetected = 1;
		EX3 = 0;									//Disable interrupt until next answer
	}
}

#endif