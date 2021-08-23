/*************************************************************
Fil:            $Id: Main.h,v 1.11 2004/09/19 16:05:59 Gemensam Exp $

Ändrad:         $Date: 2004/09/19 16:05:59 $

Revisionshistoria:
				$Log: Main.h,v $
				Revision 1.11  2004/09/19 16:05:59  Gemensam
				Split initialization in HW-specific and Application specific in case application goes horrobly wrong and freezes all.
				
				Revision 1.10  2003/12/26 16:17:01  Gemensam
				Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
				Fixed PPD detection by decreasing phone volume from 63 to 15.
				Version 0304.
				
				Revision 1.9  2003/12/09 20:52:49  Gemensam
				Commented out PPD detection since the HW not work yet.
				Ver 0302.
				
				Revision 1.8  2003/11/29 22:35:59  Gemensam
				Fixed problem with to short pulse width on PPD.
				Ver 0302.
				
				Revision 1.7  2003/11/24 22:44:36  Gemensam
				Fixed PPD, not working yet,
				New command: speaker volume.
				Version 0301.
				
				Revision 1.6  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.5  2003/11/23 20:29:34  Gemensam
				Complete and tested volume setting in options menu.
				Ver 0201
				
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

#ifndef	_MAIN_INCLUDED_
#define _MAIN_INCLUDED_

#include "FileSystem.h"


///////////////////////////////////
// Definitioner

#define VERSION_MENU_STRING	"Telestation 3.5       (c) 2004-09-19 FRHA"

#define CM_PARSIZE		24
#define CM_NRPAR		16

#define MAX_LABEL_LEN   8
#define MAX_LABELS      64

// Nedan var ett försök till multipla bat-filer, kanske införs senare
/*
#define MAX_BATFILES    8
*/

///////////////////////////////////
// Strukturer

// Nedan var ett försök till multipla bat-filer, kanske införs senare
/*
struct ST_Label {
    char    name[MAX_LABEL_LEN];
    long    pos;
};
#define Label struct ST_Label
 
struct ST_BatFileObj {
    FILE*       fp;                 //Filpekare till bat-filen
    Label*      labels;             //Pekare till lista med ingående labels
    char        labelIndex;         //Pekar på nästa lediga label
    BatFileObj* caller;             //Den batfil som kallade på denna. Om användaren gav kommandot 
                                    //skall den inehålla NULL
    char        occupied;           //1 om detta objekt används, 0 annars
};    
#define BatFileObj struct ST_BatFileObj
*/

///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeComputer1( void );
extern void    InitializePhoneAnsApp( void );
extern void    MainShell( void );
extern char    DecodeAndPerform(const char* cmdStr);
//int     MainCommandInterper(char c);
extern void    Hello( void );
extern void    TestHW( void );
extern void    FlashID( void );
extern void    Erase( void );
extern void    BackRead( void );
extern void    ReadSpare( void );
extern void    WF1( void );
extern void    WF2( void );
extern void    WF3( void );
extern void    RF1( void );
extern void    RF2( void );
extern void    RF3( void );
extern void    CreateLong( void );
extern void    ReadSpare2( void );
extern void    Append( void );

extern void    SetupBatFile( void );
extern void    ClearLabelList( void );
extern void    ExecuteBatFile(const char* batFileName);
extern char    InsertLabel(const char* name, long pos);
extern int     FindLabel(const char *name);
extern char    GotoLabel(const char* name);

extern void    myStrCpy(char* dest, const char* src);
extern int     myStrLen(char* str);
extern void    myLong2HexStr(long v, char* str);
extern void    myInt2DecStr(int v, char* str);
extern void    myInt2DecStr00(char v, char* str);
extern int     myStr2Int(const char* str);
extern char    myStrCmpNoCase(const char* s1, const char* s2);

// Nedan var ett försök till multipla bat-filer, kanske införs senare
/*
void    SetupBatFile( void );
void    ClearLabelList( void );
void    ClearLabelList(Label* list);
BatFileObj*    VacantBatFileObj(FILE* fp);
void    ExecuteBatFile(const char* batFileName);
char    InsertLabel(const char* name, long pos);
char    FindLabel(const char *name, Label* l);
char    GotoLabel(const char* name);
*/

///////////////////////////
// Globala variabler


///////////////////////////
// Lokala variabler


#endif