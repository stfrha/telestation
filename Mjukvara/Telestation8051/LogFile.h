/********************************************************
Fil:            $Id: LogFile.h,v 1.2 2003/11/20 22:08:53 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:53 $

Revisionshistoria:
				$Log: LogFile.h,v $
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_LOGFILE_INCLUDED_
#define _LOGFILE_INCLUDED_

#include "FileSystem.h"

///////////////////////////////////
// Definitioner

#define MS_LOG          0x01
#define MS_MENU         0x02
#define MS_TIMEDATE     0x03
#define MS_ERASEQST     0x04

///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeLogFile( void );
extern void    CreateLog( void );
extern void    LogMessage(const char* str1, const char* str2);
extern char    BuildCallerList( void );
extern char    FindCall(FILE* fp, int index);
extern char    FindNextCall(FILE* fp);
extern void    GetCallInformation(FILE* fp, char* phoneNumber, char* time, int* noOfMessages);
extern char    PlayThisCall(int callIndex);
extern void    PlayNewMessages( void );

#endif