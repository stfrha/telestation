/*************************************************************
Fil:            $Id: Keyboard.c,v 1.4 2003/11/20 22:08:53 Gemensam Exp $

Beskrivning:    Sk�ter gr�nsnittet mot tangentbordet

Skapad:         00-02-09

�ndrad:         $Date: 2003/11/20 22:08:53 $

�gare:          Fredrik Hoffman


Att g�ra:


Revisionshistoria:
				$Log: Keyboard.c,v $
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
#include "Keyboard.h"
#include "commands.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "Main.h"
#include "RS232.h"
#include "DTMF.h"


//////////////////////
// Globala variabler

xdata char			lastKey;


//////////////////////
// Lokala variabler

xdata char			keyboardState;
xdata char			timeoutCount;


//////////////////////
// Kod

void    InitializeKeyboard( void )
{
    lastKey = 0;
    keyboardState = KS_IDLE;
}



void    KeyboardOn( void )
{   
    xdata char    dummy;
 
    lastKey = 0;
    KEY_CLK = '1';          //M�ste vara ett f�r att systemklockan skall kunna generers h�r
    CLK = '1';              //Startar systemklockan p� P1.6
    dummy = KEYBOARD;       //Rensar eventuella v�ntande avbrott
    I2FR = 0;               //Avbrott vid negativ flank p� P1.4
    IEX2 = 0;               //Nollst�ll avbrottsflagga
    EX2 = 1;                //M�jligg�r avbrott fr�n extern avbrott 2
}


void    KeyboardOff( void )
{   
    KEY_CLK = '1';          //M�ste vara ett f�r att systemklockan skall kunna generers h�r
    CLK = '0';              //Startar systemklockan p� P1.6
    EX2 = 0;                //St�nger av avbrott fr�n extern avbrott 2
}




interrupt [0x4B] void EX2_int (void)
{
    xdata char    key;
    
    key = (~KEYBOARD) & 0x1F;

    if (key != 0) {
        timeoutCount = 0;
        keyboardState = KS_WAITFIRST;
        CTCON = (CTCON & 0x80) + 0x03;      //fosc/16, rensa CTF (compare timer overflow)
        CTRELH = 0x79;                      //Ladda reload register med 30976 = 0x7900, ty (0x10000 - 0x7900)*16/fosc = 50 ms
        CTRELL = 0x00;                      //Skrivning till CTRELL ger omedelbar reload
        IEN2 |= (1<<3);                     //M�jligg�r avbrott fr�n Compare timer, ECT = 1
        lastKey = key;
    } else {
        IEN2 &= ~(1<<3);                    //St�ng av avbrott fr�n compare timer, ECT = 0
        keyboardState = KS_IDLE;
    }
}


interrupt [0x9B] void CT_int (void)
{
    timeoutCount++;
    if  ((keyboardState == KS_WAITFIRST) && (timeoutCount >= FIRST_KEY_TIMEOUT)) {
        lastKey = (~KEYBOARD) & 0x1F;
        timeoutCount = 0;
        keyboardState = KS_WAITMULTI;
    } else if ((keyboardState == KS_WAITMULTI) && (timeoutCount >= MULTI_KEY_TIMEOUT)) {
        lastKey = (~KEYBOARD) & 0x1F;
        timeoutCount = 0;
    }    
}

