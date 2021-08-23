/*************************************************************
Fil:            $Id: DTMF.h,v 1.2 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Header fil till DTMF.c

Skapad:         03-11-18

Ändrad:         $Date: 2003/11/20 22:08:52 $

Ägare:          Fredrik Hoffman


Att göra:

Revisionshistoria:
				$Log: DTMF.h,v $
				Revision 1.2  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				

*************************************************************/

#ifndef	_DTMF_INCLUDED_
#define _DTMF_INCLUDED_

///////////////////////////////////
// Definitioner

#define DTMF_TRANS      (*(unsigned char xdata *) 0x9000)
#define DTMF_READ       (*(unsigned char xdata *) 0x9000)
#define DTMF_CONTROL    (*(unsigned char xdata *) 0x9001)
#define DTMF_STATUS     (*(unsigned char xdata *) 0x9001)
 
///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeDTMF( void );
extern void    MakeDTMFTones(const char *tones);
extern char    DecodeDTMF(char c);
extern char    DecodeReceivedDTMF( void );
extern void    ResetDTMFIndex( void );
extern char    GetNextDTMF( void );
 
#endif