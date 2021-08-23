/********************************************************
Fil:            $Id: Timer.h,v 1.2 2003/11/20 22:08:53 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:53 $

Revisionshistoria:
				$Log: Timer.h,v $
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_TIMER_INCLUDED_
#define _TIMER_INCLUDED_

///////////////////////////////////
// Definitioner

#define TM_MAX_TIMERS   5

///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeTimer( void );
extern char    StartTimer(unsigned int tenMillisecTimeout);
extern void    KillTimer(char  timerIndex);
extern char    TimedOut(char timerIndex);
extern void    WaitFor(unsigned int tenMillisec);
 
#endif