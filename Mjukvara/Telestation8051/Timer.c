/*************************************************************
Fil:            $Id: Timer.c,v 1.4 2003/11/20 22:08:53 Gemensam Exp $

Beskrivning:    Hanterar tidsfunktioner

Skapad:         99-11-18

Ändrad:         $Date: 2003/11/20 22:08:53 $

Ägare:          Fredrik Hoffman


Att göra:

Revisionshistoria:
				$Log: Timer.c,v $
				Revision 1.4  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:46  Gemensam
				Added header information
				

*************************************************************/
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Timer.h"
#include "Main.h"
#include "RS232.h"

//////////////////////
// Globala variabler

//////////////////////
// Lokala variabler

xdata int     mainTimer;
xdata unsigned int     timerLimits[TM_MAX_TIMERS];
xdata char    timerStatus[TM_MAX_TIMERS];             //0 = ledig, 1 = aktiv, 2 = timeout
xdata char    nActiveTimers;


//////////////////////
// Kod

void    InitializeTimer( void )
{                                                                                 
// Fosc/12 * 10ms = 9216. 65536-9216 = 56320 = 0xDC00

    CTCON &= ~(1<<7);               //T2PS1 = 0 => Prescaler fosc/12 eller fosc/24
    T2PS = 0;                       //T2PS = 0, => Prescaler fosc/12
    T2R1 = 1;                       //Timer 2 automatisk reload
    T2R0 = 0;   
    T2I1 = 0;                       //Timer- eller ingen funktion beroende på T2I0
    T2I0 = 0;                       //Timer 2 avstängd, använd T2I0 = 1 för att starta
    TF2 = 0;                        //Nollställ reload flagga
    CRCL = 0x00;                    //Reload värde, ger timeout varje 10 ms
    CRCH = 0xDC;
    EXEN2 = 0;                      //Stänger av reload avbrott
    EXF2 = 0;                       //Nollar reload flagga
    ET2 = 1;                        //Möjliggör timer 2 avbrott
    
    nActiveTimers = 0;
}

char StartTimer(unsigned int tenMillisecTimeout)
//Retunerar ett index till en timer, eller 0 om ingen är ledig
{
    xdata char    searchTimers = 1;
    
    while ((searchTimers < TM_MAX_TIMERS) && (timerStatus[searchTimers])) {
        searchTimers++;
    }
    if (searchTimers == TM_MAX_TIMERS) return 0;

    timerStatus[searchTimers] = 1;
    timerLimits[searchTimers] = mainTimer + (tenMillisecTimeout);
    if (!nActiveTimers) {
        T2I0 = 1;                                       //Startar timer 2 (HW)
    }
    nActiveTimers++;
    return searchTimers;
}

void    KillTimer(char  timerIndex)
{
    if (timerIndex < TM_MAX_TIMERS) {
        if (timerStatus[timerIndex] == 1) {
            nActiveTimers--;
            if (!nActiveTimers) {
                T2I0 = 0;                               //Stänger av timer 2 (HW)
            }
        }
        timerStatus[timerIndex] = 0;
    }
}

char    TimedOut(char timerIndex)
//Retunerar 1 om timern har fått timeout 0 annars.
{
    if (timerIndex < TM_MAX_TIMERS) {
        if (timerStatus[timerIndex] == 2) return 1;
    }
    return 0;
}

void    WaitFor(unsigned int tenMillisec)
{
    xdata char timer;
    
    timer = StartTimer(tenMillisec);
    if (timer) {
        while (!TimedOut(timer)) {
            WDT = 1;
            SWDT = 1;
        }
        KillTimer(timer);
    }
}

interrupt [0x2B] void T2_int (void)
// Timer 2 Overflow
{
    xdata char    i;
    
    ET2 = 0;                                            //Stoppar interrupt under avbrottsrutinen
    
    mainTimer++;
    
    for (i=0 ; i<TM_MAX_TIMERS ; i++) {
        if (timerStatus[i] == 1) {
            if (mainTimer == timerLimits[i]) {
                timerStatus[i] = 2;
//                SendRS232('T');
                nActiveTimers--;
                if (!nActiveTimers) {
                    T2I0 = 0;                           //Stänger av timer 2
                }
            }
        }
    }
    TF2 = 0;                                            //Nollställer reload-flagga
    ET2 = 1;                                            //Möjliggör avbrott på nytt
}    
