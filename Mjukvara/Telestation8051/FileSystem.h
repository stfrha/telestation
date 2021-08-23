/**************************************************************************************
Fil:            $Id: FileSystem.h,v 1.4 2003/11/20 22:08:53 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:53 $

Kommentarer:	Filhuvudet befinner sig i "Spare" arean tillhörande filens första sida. Huvudet består 
				av 16 bytes enligt följande lista:
				0       Huvud       0xAA = Legalt filhuvud, annat tillsvidare odefinierat
				1-4     Fillängd    Pos 1 = MSB, pos 4 = LSB
				5-12    Filtitel    Enl "Titel.ext". Om titeln är mindre än 8 tecken avslutas det med nollor
				13-15   Filext      Enl "Titel.ext". Om ext är mindre än 3 tecken avslutas det med nollor

				En nolla skriven i fileName[0] indikerar att
				filstrukturen är ledig.
	
Revisionshistoria:
				$Log: FileSystem.h,v $
				Revision 1.4  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

**************************************************************************************/

#ifndef	_FILESYSTEM_INCLUDED_
#define _FILESYSTEM_INCLUDED_

///////////////////////////////////
// Definitioner

#define FS_MAXFILES         8 
#define FS_FILENAMELENGTH   13
#define FS_FLASHSIZE        0x800000

#define OFF            0
#define READY        1
#define NEMPTY        2
#define WRITING        3
#define OCCUPIED    4

#define CLE     P4.0                                //Command Latch Enable
#define ALE     P4.1                                //Address Latch Enable
#define WP      P4.2                                //Write Protect
#define SE      P4.3                                //Spare Enable
//#define CE      P4.4                                //Chip Enable
#define FBUSY   P4.5                                //Busy

#define FREG (*(unsigned char xdata *) 0xE000)
#define FLASH (*(unsigned char xdata *) 0xB000) 

///////////////////////////////////
// Strukturer

struct ST_FILE_HEAD {
    unsigned long   length;
    char            name[FS_FILENAMELENGTH];
};
#define FILE_HEAD struct ST_FILE_HEAD


struct ST_FILE {
    char            fileName[FS_FILENAMELENGTH];
    unsigned long   startAddress;
    unsigned long   length; 
    unsigned long   offset; 
    char            write;
    char            append;
};
#define FILE struct ST_FILE 



///////////////////////////////////
// Funktioner och procedurer

extern void            InitializeFileSystem( void );
extern void            InitializeFlash( void );
extern void            SetupReadSeq( void );
extern unsigned char   ReadFLASH( void );
extern void            TerminateReadSeq( void );
extern void            GetDeviceIDRS232(void);
extern void            SetupWriteSeq( void );
extern void            WriteFLASH(unsigned char);
extern void            TerminateWriteSeq( void );
extern void            PerformWriteSeq( void );
extern void            EraseBlock( void );
extern void            WriteFileHead(const char* name, long size);
extern FILE*           fopen(const char* name, const char* def);
extern FILE*		   fopenA(const char* name, long maxSize, long offset);
extern void            fputc(FILE* filePtr, char c);
extern void            fputLine(FILE* filePtr, const char* str);
extern void            fupLoad( void );
extern int             fgetc(FILE* filePtr);
extern void            fgetLine(FILE* filePtr, char* str, int maxLen);
extern void            fdownLoad( void );
extern void            fclose(FILE* filePtr);
extern long            ftell(FILE* filePtr);
//void            Rewind(FILE* filePtr);
extern void            fSetPos(FILE* filePtr, long offset);
extern void            ReleaseCurrFile( void );
extern void            EraseAll( void );
extern void            StartDynamic( void );
extern void            EraseDynamic( void );
extern char			   MemUsage( void );
extern void			   Dir( void );
extern void			   Type( void );
extern void			   TypeA( void );

#endif