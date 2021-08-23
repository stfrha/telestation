/*************************************************************
Fil:            $Id: DPot.c,v 1.4 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Hanterar digitala potentiometern och kan stänga av och
                sätt på ljudkällot

Skapad:         01-02-04

Ändrad:         $Date: 2003/11/20 22:08:52 $

Ägare:          Fredrik Hoffman


Att göra:

Kommentarer:

    Varje pot har fyra nonvolatile register. Dessa definieras härmed enligt följande:
    *   0 = Zero Wiper  = 0
    *   1 = Variable Wiper  = ?
    *   2 = Oanvänd = X
    *   3 = Full Wiper = 255
    Vid initialiseringen skall värden för Zero och Full skrivas in i NV-registererna
    Sedan skall Wiper på alla pottar sättas till Zero.

Revisionshistoria:
				$Log: DPot.c,v $
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
#include "DPOT.h"
#include "main.h"
#include "rs232.h"

//////////////////////
// Globala variabler

xdata char	speakerVolume;

//////////////////////
// Lokala variabler


//////////////////////
// Kod

void    InitializeDPot( void )
{                                                                                 
    DPDA = 1;
    DPCL = 1;

	speakerVolume = DEFAULT_SPEAKER_VOLUME;
}


void    RPWait( void )
{
//    xdata char    a;
   
//    a = 1;
//    a = a+1;

    xdata unsigned int   i;
    
    for (i = 0 ; i<10 ; i++) {
        WDT = 1;
        SWDT = 1;
    }
    
}

 
char WOEPB(char dat)
//WriteOneCalenderByte (intern funktion)
{
    xdata char    ack;
    xdata char     i;

    for (i=0 ; i<8 ; i++) {
        DPDA = (dat >> 7) & 0x01;
        RPWait();
        DPCL = 1;
        RPWait();
        DPCL = 0;
        RPWait();
        dat = dat << 1;
    }

    DPDA = 1;            //Sätt DPDA till ingång
    RPWait();
    DPCL = 1;
    RPWait();
    ack = DPDA;            //Hämta ack-bit
    DPCL = 0;
    RPWait();
    return ack;
}
 
char ROEPB(char ack)
//ReadOneCalenderByte (intern funktion)
{
    xdata char    dat;
    xdata char     i;

    dat = 0;

    DPDA = 1;            //Sätt DPDA till ingång
    RPWait();

    for (i=0 ; i<8 ; i++) {
        dat = dat << 1;
        DPCL = 1;
        RPWait();
        dat = dat + DPDA;
        DPCL = 0;
        RPWait();
    }


    DPDA = ack;
    RPWait();
    DPCL = 1;
    RPWait();
    DPCL = 0;
    RPWait();
    return dat;
}


char    SetDPWiper(char pot, char value)
{   
    DPCL = 1;            //Start
    RPWait();
    DPDA = 0;
    RPWait();
    DPCL = 0;
    RPWait();
 
    if (WOEPB(0x50)) return 1;                   //Adressering
    if (WOEPB(0xA0 + ((pot & 0x03) << 2))) return 2;      //Instruktion
    if (WOEPB(value)) return 3;      //Data
 
    DPDA = 0;            //Stopp
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();
    DPCL = 0;
    return 0;
}

/*
char    SetDPNVRegister(char pot, char reg, char value)
{
    DPCL = 1;            //Start
    RPWait();
    DPDA = 0;
    RPWait();
    DPCL = 0;
    RPWait();

    if (WOEPB(0x50)) return 1;                       //Adressering
    if (WOEPB(0xC0 + (pot << 2) + reg)) return 2;    //Instruktion
    if (WOEPB(value)) return 3;                      //Data

    DPDA = 0;            //Stopp
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    DPCL = 0;
    RPWait();

    while (WOEPB(0x50)) {                           //Ack polling
        WDT = 1;
        SWDT = 1;
    }
    return 0;
}

char    TransferDPNVRegister(char pot, char reg)
{
    DPCL = 1;            //Start
    RPWait();
    DPDA = 0;
    RPWait();
    DPCL = 0;
    RPWait();

    if (WOEPB(0x50)) return 1;                   //Adressering
    if (WOEPB(0xD0 + (pot << 2) + reg)) return 2;      //Instruktion
 
    DPDA = 0;            //Stopp
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();
    DPCL = 0;
    return 0;
} 
*/ 

char    ReadDPWiper(char pot, char* dat)
{
    DPCL = 1;            //Start
    RPWait();
    DPDA = 0;
    RPWait();
    DPCL = 0;
    RPWait();

    if (WOEPB(0x50)) return 1;                   //Adressering
    if (WOEPB(0x90 + ((pot & 0x03) << 2))) return 3;      //Instruktion
    *dat = ROEPB(1);

    DPDA = 0;            //Stopp
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();
    DPCL = 0;
    return 0;
}


 
 //Volume control
 
void    SpeakerVolume(char volume)
{
    SetDPWiper(POT_SPEAKER, volume & 0x3F);
}
 
void    PhoneInVolume(char volume)
{
    SetDPWiper(POT_PHONE_IN, volume & 0x3F);
}
 
void    PhoneOutVolume(char volume)
{
    SetDPWiper(POT_PHONE_OUT, volume & 0x3F);
}
 
void    DTMFVolume(char volume)
{
    SetDPWiper(POT_DTMF, volume & 0x3F);
}




/*    Arv från Calender.c



char    WriteDPByte(char address, char dat)
{
    DPDA = 0;            //Start
    RPWait();
    DPCL = 0;
    RPWait();

    if (WOEPB(0xD0)) return 1;        //Slave address och R/W = 0
    if (WOEPB(address)) return 1;
    if (WOEPB(dat)) return 1;
 
    DPDA = 0;
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();

    return 0;

}

char    ReadDPByte(char address, char* dat)
{
    DPDA = 0;            //Start
    RPWait();
    DPCL = 0;
    RPWait();
 
    if (WOEPB(0xD0)) return 1;        //Slave address och R/W = 0
    if (WOEPB(address)) return 1;

    DPDA = 0;            //Stop condition
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();

    DPDA = 0;            //Restart
    RPWait();
    DPCL = 0;
    RPWait();

    if (WOEPB(0xD1)) return 1;        //Slave address och R/W = 1
    *dat = ROCB(1);

    DPDA = 0;
    RPWait();
    DPCL = 1;
    RPWait();
    DPDA = 1;
    RPWait();
 
    return 0;
}               

*/


    
 
