/*************************************************************
Fil:            $Id: Calender.c,v 1.4 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Hanterar realtids- och kalenderfunktioner

Skapad:         00-01-26

Ändrad:         $Date: 2003/11/20 22:08:52 $

Ägare:          Fredrik Hoffman


Att göra:


Revisionshistoria:
				$Log: Calender.c,v $
				Revision 1.4  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:35  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

*************************************************************/
                        
                        
//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <stdlib.h>
#include "calender.h"
#include "main.h"
#include "rs232.h"

 
//////////////////////
// Globala variabler


 
//////////////////////
// Lokala variabler


//////////////////////
// Kod

void    InitializeCalender( void )
{                                                                                 
    SDA = 1;
    SCL = 1;
}


void    CalenderWait4( void )
{
    xdata char    a;
    
    a = 1;
    a = a+1;
}

 
char WOCB(char dat)
//WriteOneCalenderByte (intern funktion)
{
    xdata char    ack;
    xdata char     i;

    for (i=0 ; i<8 ; i++) {
        SDA = (dat >> 7) & 0x01;
        SCL = 1;
        CalenderWait4();
        SCL = 0;
        CalenderWait4();
        dat = dat << 1;
    }

    SDA = 1;            //Sätt SDA till ingång
    CalenderWait4();
    SCL = 1;
    CalenderWait4();
    ack = SDA;            //Hämta ack-bit
    SCL = 0;
    CalenderWait4();
    return ack;
}
 
char ROCB(char ack)
//ReadOneCalenderByte (intern funktion)
{
    xdata char    dat;
    xdata char     i;

    dat = 0;

    SDA = 1;            //Sätt SDA till ingång
    CalenderWait4();

    for (i=0 ; i<8 ; i++) {
        dat = dat << 1;
        SCL = 1;
        CalenderWait4();
        dat = dat + SDA;
        SCL = 0;
        CalenderWait4();
    }


    SDA = ack;
    CalenderWait4();
    SCL = 1;
    CalenderWait4();
    SCL = 0;
    CalenderWait4();
    return dat;
}

char    WriteCalenderByte(char address, char dat)
{
    SDA = 0;            //Start
    CalenderWait4();
    SCL = 0;
    CalenderWait4();

    if (WOCB(0xD0)) return 1;        //Slave address och R/W = 0
    if (WOCB(address)) return 1;
    if (WOCB(dat)) return 1;

    SDA = 0;
    CalenderWait4();
    SCL = 1;
    CalenderWait4();
    SDA = 1;
    CalenderWait4();

    return 0;

}

char    ReadCalenderByte(char address, char* dat)
{
    SDA = 0;            //Start
    CalenderWait4();
    SCL = 0;
    CalenderWait4();
 
    if (WOCB(0xD0)) return 1;        //Slave address och R/W = 0
    if (WOCB(address)) return 1;

    SDA = 0;            //Stop condition
    CalenderWait4();
    SCL = 1;
    CalenderWait4();
    SDA = 1;
    CalenderWait4();

    SDA = 0;            //Restart
    CalenderWait4();
    SCL = 0;
    CalenderWait4();

    if (WOCB(0xD1)) return 1;        //Slave address och R/W = 1
    *dat = ROCB(1);

    SDA = 0;
    CalenderWait4();
    SCL = 1;
    CalenderWait4();
    SDA = 1;
    CalenderWait4();
 
    return 0;
}               

void    GetTimeNow(TIME* tptr)
{
    xdata char t;
    
    ReadCalenderByte(0, &t);
    tptr->seconds = t;

    ReadCalenderByte(1, &t);
    tptr->minutes = t;

    ReadCalenderByte(2, &t);
    tptr->hours = t;
}

void    GetDateNow(DATE* tptr)
{
    xdata char t;
    
    ReadCalenderByte(3, &t);
    tptr->day = t;

    ReadCalenderByte(4, &t);
    tptr->date = t;

    ReadCalenderByte(5, &t);
    tptr->month = t;

    ReadCalenderByte(6, &t);
    tptr->year = t;
}
 
/*
void    GetTimeNow(TIME* tptr)
{
    char t;
    
    ReadCalenderByte(0, &t);
    tptr->seconds = (((t >> 4) & 0x07) * 10) + (t & 0x0F);
 
    ReadCalenderByte(1, &t);
    tptr->minutes = (((t >> 4) & 0x07) * 10) + (t & 0x0F);
 
    ReadCalenderByte(2, &t);
    if (t & 0x40) {
        //12 visning:
        if (t & 0x20) {
            //PM
            tptr->hours = (((t >> 4) & 0x01) * 10) + (t & 0x0F) + 12;
        } else {
            //AM
            tptr->hours = (((t >> 4) & 0x01) * 10) + (t & 0x0F);
        }
    } else {
        //24 visning
        tptr->hours = (((t >> 4) & 0x03) * 10) + (t & 0x0F);
    }
}

void    GetDateNow(DATE* tptr)
{
    char t;
    
    ReadCalenderByte(3, &t);
    tptr->day = t;

    ReadCalenderByte(4, &t);
    tptr->date = (((t >> 4) & 0x03) * 10) + (t & 0x0F);
    
    ReadCalenderByte(5, &t);
    tptr->month = (((t >> 4) & 0x03) * 10) + (t & 0x0F);

    ReadCalenderByte(6, &t);
    tptr->year = (((t >> 4) & 0x0F) * 10) + (t & 0x0F);
}
*/

void    CreateTimeString(TIME* tptr, char* str)
{
    xdata char    i;
    
    i = 0;
    
    if (tptr->hours & 0x40) {
        str[i++] = ((tptr->hours >> 4) & 0x01) + '0';
    } else {
        str[i++] = ((tptr->hours >> 4) & 0x03) + '0';
    }
    str[i++] = (tptr->hours & 0x0F) + '0';
    
    str[i++] = ':';

    str[i++] = ((tptr->minutes >> 4) & 0x07) + '0';
    str[i++] = (tptr->minutes & 0x0F) + '0';
    
    str[i++] = ':';
 
    str[i++] = ((tptr->seconds >> 4) & 0x07) + '0';
    str[i++] = (tptr->seconds & 0x0F) + '0';

    if (tptr->hours & 0x40) {
        str[i++] = ' ';
        if (tptr->hours & 0x20) {
            str[i++] = 'A';
        } else {
            str[i++] = 'P';
        }
        str[i++] = 'M';  
    }
    str[i++] = 0x00;
} 



void    CreateDateString(DATE* tptr, char* str)
//Skapar en sträng med utseendet: YYYY-MM-DD
{
    xdata char    i;
    
    i = 0;

    str[i++] = '2';
    str[i++] = '0';
    str[i++] = ((tptr->year >> 4) & 0x0F) + '0';
    str[i++] = (tptr->year & 0x0F) + '0';
    str[i++] = '-';
    str[i++] = ((tptr->month >> 4) & 0x03) + '0';
    str[i++] = (tptr->month & 0x0F) + '0';
    str[i++] = '-';
    str[i++] = ((tptr->date >> 4) & 0x03) + '0';
    str[i++] = (tptr->date & 0x0F) + '0';
    str[i++] = 0x00;
}        
 
void    CreateDateString2(DATE* tptr, char* str)
//Skapar en sträng med utseendet: DD MMM
{
    xdata char    i;
    xdata char    c;
    
    i = 0;
              
    c = (tptr->date >> 4) & 0x03;
    
    if (c) {
        str[i++] = c + '0';
    } else {
        str[i++] = ' ';
    }
    str[i++] = (tptr->date & 0x0F) + '0';
    str[i++] = ' ';
    switch (tptr->month) {
    case 0x01 :
        str[i++] = 'J';
        str[i++] = 'A';
        str[i++] = 'N';
        break;
    case 0x02 :
        str[i++] = 'F';
        str[i++] = 'E';
        str[i++] = 'B';
        break;
    case 0x03 :
        str[i++] = 'M';
        str[i++] = 'A';
        str[i++] = 'R';
        break;
    case 0x04 :
        str[i++] = 'A';
        str[i++] = 'P';
        str[i++] = 'R';
        break;
    case 0x05 :
        str[i++] = 'M';
        str[i++] = 'A';
        str[i++] = 'Y';
        break;
    case 0x06 :
        str[i++] = 'J';
        str[i++] = 'U';
        str[i++] = 'N';
        break;
    case 0x07 :
        str[i++] = 'J';
        str[i++] = 'U';
        str[i++] = 'L';
        break;
    case 0x08 :
        str[i++] = 'A';
        str[i++] = 'U';
        str[i++] = 'G';
        break;
    case 0x09 :
        str[i++] = 'S';
        str[i++] = 'E';
        str[i++] = 'P';
        break;
    case 0x10 :                     //Jodå, det skall vara 0x10
        str[i++] = 'O';
        str[i++] = 'K';
        str[i++] = 'T';
        break;
    case 0x11 :
        str[i++] = 'N';
        str[i++] = 'O';
        str[i++] = 'V';
        break;
    case 0x12 :
        str[i++] = 'D';
        str[i++] = 'E';
        str[i++] = 'C';
        break;
    default:
        str[i++] = '?';
        str[i++] = '?';
        str[i++] = '?';
        break;
    }         

    str[i++] = 0x00;
}        



    
 
