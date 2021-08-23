/********************************************************
Fil:            $Id: wav.h,v 1.1 2003/11/23 20:29:34 Gemensam Exp $

Ändrad:         $Date: 2003/11/23 20:29:34 $

Revisionshistoria:
				$Log: wav.h,v $
				Revision 1.1  2003/11/23 20:29:34  Gemensam
				Complete and tested volume setting in options menu.
				Ver 0201
				
				

********************************************************/

#ifndef	_WAV_INCLUDED_
#define _WAV_INCLUDED_

///////////////////////////////////
// Definitioner

#define WS_NO_OP        0x00
#define WS_REC          0x01
#define WS_PLAY         0x02
#define WS_ECHO         0x03
#define WS_TONE         0x04
#define WS_PORT1        0x05
 
#define DAC0 (*(unsigned char xdata *) 0xA000)
#define DAC1 (*(unsigned char xdata *) 0xA001)
#define DAC2 (*(unsigned char xdata *) 0xA002)
#define DAC3 (*(unsigned char xdata *) 0xA003)

#define WAVBUFSIZE  512

//#define SPEAKER_ON  P1.5 = 1
//#define SPEAKER_OFF  P1.5 = 0

///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeWAV( void );
extern void    SetupMic( void );
extern void    SetupPhoneIn( void );
extern void    RecMicFile(const char* fileName);
extern void    RecPhoneFile(const char* fileName);
extern void    RecFile(const char* fileName);
extern void    PlayFile(const char* fileName);
extern void    EchoMic( void );
extern void    EchoPhone( void );
extern void    Echo( void );
extern void    WavTone( void );
 
#endif