/********************************************************
Fil:            $Id: RS232.h,v 1.2 2003/11/20 22:08:53 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:53 $

Revisionshistoria:
				$Log: RS232.h,v $
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_RS232_INCLUDED_
#define _RS232_INCLUDED_

///////////////////////////////////
// Definitioner


///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void		InitializeRS232( void );
extern void		SendRS232(char tecken);
extern void		SendStringRS232(const char *str);
extern void		SendStringEOLRS232(const char *str);
extern char		WaitAndReceiveRS232( void );
extern char		WaitAndReceiveTimeout( void );
extern int      ReceiveLineRS232(char* str, int maxLen);

#endif