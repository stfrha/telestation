/*************************************************************
Fil:            $Id: DPot.h,v 1.2 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Header fil till DPot.c

Skapad:         03-11-17

Ändrad:         $Date: 2003/11/20 22:08:52 $

Ägare:          Fredrik Hoffman


Att göra:

Kommentarer:

    Varje pot har fyra nonvolatile register. Dessa definieras härmed enligt följande:
    *   0 = Zero Wiper  = 0
    *   1 = Variable Wiper  = ?
    *   2 = Oanvänd = X
    *   3 = Full Wiper = 255
    Vid initialiseringen skall värden för Zero och Full skrivas in i NV-registererna
    Sedan skall Wiper på alla pottar sättas till Zero.

Revisionshistoria:
				$Log: DPot.h,v $
				Revision 1.2  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				

*************************************************************/

#ifndef	_DPOT_INCLUDED_
#define _DPOT_INCLUDED_

///////////////////////////////////
// Definitioner

#define DPCL    P3.3
#define DPDA    P3.4
 
#define POT_PHONE_OUT   0x00
#define POT_PHONE_IN    0x03
#define POT_SPEAKER     0x01
#define POT_DTMF        0x02

#define DEFAULT_SPEAKER_VOLUME	50

///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeDPot( void );
extern void    RPWait( void );
extern char    WOEPB(char dat);
extern char    ROEPB(char ack);
extern char    SetDPWiper(char pot, char value);
extern char    ReadDPWiper(char pot, char* dat);
 
extern void    SpeakerVolume(char volume);
extern void    PhoneInVolume(char volume);
extern void    PhoneOutVolume(char volume);
extern void    DTMFVolume(char volume);

#endif