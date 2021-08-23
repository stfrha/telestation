/********************************************************
Fil:            $Id: Phone.h,v 1.6 2003/12/26 16:17:01 Gemensam Exp $

Ändrad:         $Date: 2003/12/26 16:17:01 $

Revisionshistoria:
				$Log: Phone.h,v $
				Revision 1.6  2003/12/26 16:17:01  Gemensam
				Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
				Fixed PPD detection by decreasing phone volume from 63 to 15.
				Version 0304.
				
				Revision 1.5  2003/11/29 22:36:00  Gemensam
				Fixed problem with to short pulse width on PPD.
				Ver 0302.
				
				Revision 1.4  2003/11/24 22:44:36  Gemensam
				Fixed PPD, not working yet,
				New command: speaker volume.
				Version 0301.
				
				Revision 1.3  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_PHONE_INCLUDED_
#define _PHONE_INCLUDED_

///////////////////////////////////
// Definitioner

//PS = Phone Signals
#define PS_RV						P1.2
#define PS_LC						P1.7
#define PS_LCD						P1.1
#define PS_PPD						P1.0

#define WRS_POS_EDGE_TRIGG          10
#define WRS_NEG_EDGE_TRIGG          10
#define WRS_POST_SIGNAL_LIMIT       20
#define WRS_SIGNAL_MIN_INTERVALL    300
#define WRS_SIGNAL_MAX_INTERVALL    600

/* Examples of timeouts
Lift phone:		Play:
1E5				11
209				163
20C				11
200				4
216				E8	
1F3				F5
*/

#define P_PPD_PW					0x190
#define P_RIPPLE_FILTER_TIMEOUT		10

#define	DEFAULT_PHONE_OUT_VOLUME	15

///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializePhone( void );
extern void    HangUp( void );
extern void    LiftPhone( void );

#ifdef	__PPD_ON__
extern void	   ClearPPDRipple( void );
#endif

extern void    MonitorRingSignal( void );
extern int	   WaitForRingSignals(int  nSignals);

#endif