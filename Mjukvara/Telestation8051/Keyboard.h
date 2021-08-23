/*************************************************************
Fil:            $Id: Keyboard.h,v 1.2 2003/11/20 22:08:53 Gemensam Exp $

Beskrivning:    Header fil till Keyboard.c

Skapad:         03-11-18

Ändrad:         $Date: 2003/11/20 22:08:53 $

Ägare:          Fredrik Hoffman


Att göra:

Kommentarer:

Revisionshistoria:
				$Log: Keyboard.h,v $
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				

*************************************************************/

#ifndef	_KEYBOARD_INCLUDED_
#define _KEYBOARD_INCLUDED_

///////////////////////////////////
// Definitioner

#define KEYBOARD    (*(unsigned char xdata *) 0xC000)
#define KEY_CLK     P1.6
#define KEY_IRQ     P1.4

#define KS_IDLE         0x00
#define KS_WAITFIRST    0x01
#define KS_WAITMULTI    0x02

#define FIRST_KEY_TIMEOUT   10
#define MULTI_KEY_TIMEOUT   2
 

///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeKeyboard( void );
extern void    KeyboardOn( void );
extern void    KeyboardOff( void );
 
#endif