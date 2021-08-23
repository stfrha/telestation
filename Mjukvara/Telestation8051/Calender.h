/********************************************************
Fil:            $Id: Calender.h,v 1.4 2003/11/20 22:08:52 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:52 $

Revisionshistoria:
				$Log: Calender.h,v $
				Revision 1.4  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:35  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

********************************************************/

#ifndef	_CALENDER_INCLUDED_
#define _CALENDER_INCLUDED_

///////////////////////////////////
// Definitioner

// Non-volatile variabler:
#define NV_ANSWERING_ON 0x08
#define NV_NUM_SIGNALS  0x09
#define NV_PLAYED_MS    0x0A
#define NV_PLAYED_LS    0x0B
#define NV_SPEAKER_VOL  0x0C
//Max adress är 0x3F

#define SDA    P3.0
#define SCL    P3.1
//#define SDA    P1.5
//#define SCL    P1.6


///////////////////////////////////
// Strukturer

struct ST_TIME {
    char    seconds;
    char    minutes;
    char    hours;
};
#define TIME struct ST_TIME


struct ST_DATE {
    char    day;
    char    date;
    char    month;
    char    year;
};
#define DATE struct ST_DATE

///////////////////////////////////
// Funktioner och procedurer

extern void    InitializeCalender( void );
extern char    WOCB(char dat);
extern char    ROCB(char ack);
extern char    WriteCalenderByte(char address, char dat);
extern char    ReadCalenderByte(char address, char* dat);
extern void    GetTimeNow(TIME* tptr);
extern void    GetDateNow(DATE* tptr);
extern void    CreateTimeString(TIME* tptr, char* str);
extern void    CreateDateString(DATE* tptr, char* str);
extern void    CreateDateString2(DATE* tptr, char* str);
 

///////////////////////////
// Variabler




 
#endif