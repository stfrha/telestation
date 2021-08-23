/********************************************************
Fil:            $Id: PhoneAns.h,v 1.2 2003/11/20 22:08:53 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:53 $

Revisionshistoria:
				$Log: PhoneAns.h,v $
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_PHONEANS_INCLUDED_
#define _PHONEANS_INCLUDED_

#include "Main.h"
#include "Calender.h"

///////////////////////////////////
// Definitioner

#define MS_LOG          0x01
#define MS_MENU         0x02
#define MS_TIMEDATE     0x03
#define MS_ERASEQST     0x04
#define MS_MEMO         0x05

#define PB_MAXENTRIES   512
#define PB_POSTSIZE     32

///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializePhoneAns( void );
extern void    WaitForCall( void );
extern void    CorrectDate(char changedDate);
extern void    RecordCallerMessage( void );
extern void    AskQuestionMsg( void );
extern void    EnterCode( void );
extern void    SetupUpQLabels( void );
extern void    LabelCopy(char* dest, const char* src);
extern void    CreateUniqueFilename(char* str);
extern void    ShowTimeDateSet( void );
extern void    ShowEraseQuest( void );
extern void    ReadPhoneBookFile( void );
extern void    LookUpCaller(char* number, char* name,  char* category);
extern void    IntitializeMemo( void );
extern void    RecordMemo( void );
extern void    PlayMemo( void );
extern void    CreateMemoFileName(int index, char* str);
extern char    CategoryHit(char* str, char category);

#endif