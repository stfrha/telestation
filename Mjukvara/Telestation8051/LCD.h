/********************************************************
Fil:            $Id: LCD.h,v 1.4 2003/11/23 21:40:03 Gemensam Exp $

Ändrad:         $Date: 2003/11/23 21:40:03 $

Revisionshistoria:
				$Log: LCD.h,v $
				Revision 1.4  2003/11/23 21:40:03  Gemensam
				Parallell phone detection included.
				Removed obsolete #define's regarding LCD interface.
				Ver 0300
				
				Revision 1.3  2003/11/23 20:29:34  Gemensam
				Complete and tested volume setting in options menu.
				Ver 0201
				
				Revision 1.2  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.1  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				

********************************************************/

#ifndef	_LCD_INCLUDED_
#define _LCD_INCLUDED_

///////////////////////////////////
// Definitioner

#define LCD_DATA      (*(unsigned char xdata *) 0x8000)
#define LCD_INSTR     (*(unsigned char xdata *) 0x8001)
#define LCD_BUSY      (*(unsigned char xdata *) 0x8001)

#define DISPBUFFERSIZE  50
// DISPBUFFERSIZE är antalet samtal i dispBuffer (samtalsindexerat)
#define DISPCALLSIZE  40
// DISPCALLSIZE är antalet byte per samtalsrad
 
#define LCD_CHAR_PER_ROW	42

#define MENU_LEFT_COLUMN	4

#define MENU_ITEM_SPKR_VOL	0
#define MENU_ITEM_ANS_ONOFF	1
#define MENU_ITEM_MEMO		2
#define MENU_ITEM_TIMEDATE	3
#define MENU_ITEM_NUMSIG	4
#define MENU_ITEM_ERASE		5
#define MENU_NUM_OF_ITEMS	6


///////////////////////////////////
// Strukturer


///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeLCD( void );
extern void    ClearDisp( void );
extern void    DispString(char* str);
extern void    SetLCDReg(char regNr, char d);
extern void    LogCursorAt(int newCursorPos, char updateAlways);
extern void    FillDispBuffer(void);
extern void    DispInfoBox(char* str1, char* str2);
extern void    DisplayMenu(char* menuStr);
extern void    DispMemUsage( void );
extern void    OptionCursorAt(int  newCursorPos);
extern void    DispOptionsMenu(char answerOn, char numSignals);
extern void    DispSetTimeDate(char cursor, char year, char month, char day, char hour, char minute);
extern void    DispSetTimeDateHead( void );
extern void    DispEraseQuest( void );
extern void    DispMemoScreen( void );

#endif