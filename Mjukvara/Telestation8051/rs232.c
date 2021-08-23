/*************************************************************
Fil:            $Id: rs232.c,v 1.4 2003/11/20 22:08:53 Gemensam Exp $

Beskrivning:    Procedurer och funktioner för RS232-port till 
                SuperComputer

Skapad:         99-09-19

Ändrad:         $Date: 2003/11/20 22:08:53 $

Ägare:          Fredrik Hoffman

Revisionshistoria:
				$Log: rs232.c,v $
				Revision 1.4  2003/11/20 22:08:53  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:36  Gemensam
				Added menu option of setting speaker volume (untested)
				Completely changed header files
				Version 0200.
				
				Revision 1.2  2003/11/10 22:44:46  Gemensam
				Added header information
				

*************************************************************/

//////////////////////
// Inkluderingar

#include <io517a.h>
#include <stdio.h>
#include <string.h>
#include "RS232.h"
#include "Main.h"


//////////////////////
// Globala variabler

extern xdata char	timeout;			//Tidsmätningsvariabel


//////////////////////
// Lokala variabler

//xdata char  transBusy;
//xdata char  receivedChar;


//////////////////////
// Kod

void InitializeRS232( void )
{
    
    S1CON = 0x98;                                //Ställer in mode B = 8-bit UART, nollar flaggor och enablar mottagning
    S1RELH = 0x03;                                //Ställer in baudrate 19200 bytes/s                                
    S1RELL = 0xEE;
    ADCON0 = 0x80;                                //Startar baudrate generering

    S1BUF = '>';                                    //Dummytecken för att SendRS232 skall funka    
    
//    IEN2 &= ~1;                                    //Sätter ES1, dvs möjliggör avbrott på serieport 1
}                                                                        

void    SendStringRS232(const char *str)
//Skickar en 0-terminerad sträng på RS232 porten.
{
    xdata int   i, l;
    
    l = myStrLen(str);
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //Sänder tecken     
        WDT = 1;
        SWDT = 1;
    }
}        

void    SendStringEOLRS232(const char *str)
//Skickar en 0-terminerad sträng på RS232 porten med "Carrige Return" och "Line Feed" efterråt.
{
    xdata int   i, l;
                                                        
    l = myStrLen(str);                                    //Längden på strängen
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //Sänder tecken     
        WDT = 1;
        SWDT = 1;
    }

    SendRS232('\r');                                      //"Carrige Return"
    SendRS232('\n');                                      //"Line Feed"
 
}        


char   WaitAndReceiveRS232( void )
//Väntar tills det finns ett tecken på RS232 porten redo för mottagning. Retunerar det tecknet.
{
    char    tecken;

    while(!(S1CON & 0x01)) {                               //Väntar på mottagning
        WDT = 1;
        SWDT = 1;
    }
    tecken = S1BUF;                                     //Läser mottaget tecken   
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          
    
    return tecken;
}

char   WaitAndReceiveTimeout( void )
//Väntar på att ett tecken skall bli klart för mottagning på RS232-porten men inte
//längre än 4 ms. Timer 0 används för tidtagning. 
//Retunerar det mottagna tecknet om det går bra eller 0x00 om det blir timeout.
{
    xdata unsigned char     tecken;

    TL0 = 0x00;                                         //Reset timeout value
    TH0 = 0x00;                                         //0 <=> ca 71 ms timeout
    timeout = 0;
    TR0 = 1;                                            //Start timer
 
    while((!(S1CON & 1)) && (!timeout)) {           //Väntar på mottagning
        WDT = 1;
        SWDT = 1;
    }

    TR0 = 0;
    if (!timeout) {    
        tecken = S1BUF;                                 //Läser mottaget tecken   
        S1CON &= ~(1);                                  //Nollar mottagnings-interrupt-flagga     
    } else {
        tecken = 0x00;
    }
    return tecken;
}

void    SendRS232(char tecken)
//Sänder ett tecken på RS232-porten och väntar tills det är sänt.
{
    while (!(S1CON & 0x02));
    S1CON &= ~(2);                                      //Nollar sändnings-interrupt-flagga      
    S1BUF = tecken;                                     //Sänder tecken 
}

int        ReceiveLineRS232(char* str, int maxLen)
{
    xdata char    c;
    xdata char    index = 0;

    str[index] = 0;

    do {
        c = WaitAndReceiveRS232();
        if (c == 0x1B) {                                //ESC
            str[0] = 0x1B;
            c = WaitAndReceiveRS232();
            str[1] = c;
            str[2] = 0;
            return 2;
        }
        if (c >= ' ') {
            if (index < maxLen) {
                SendRS232(c);
                str[index++] = c;
                str[index] = 0;
            }
        } else if (c == '\b') {
            if (index > 0) {
                SendRS232('\b');
                SendRS232(' ');
                SendRS232('\b');
                index--;
                str[index] = 0;
            }
        }
    
    } while (c != '\r');
    str[index] = 0;
    str[index+1] = 0;
 
    SendRS232('\r');                                      //"Carrige Return"
    SendRS232('\n');                                      //"Line Feed"

    return index;
}
    
            
 

