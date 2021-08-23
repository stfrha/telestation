/*************************************************************
Fil:            Program.c

Beskrivning:    Självständig applikation för att programmera 
                det låga programminnet. Körs genom att PLD 
                ställs in för programmering genom en strapp 
                (ev en switch på "burken").

Skapad:         99-09-19

Ägare:          Fredrik Hoffman
*************************************************************/

//////////////////////
//Externa definitioner

#include <io517a.h>
#include <stdio.h> 
#include <string.h> 

//////////////////////
//Interna definitioner

///////////////////////////////////
//Externa funktioner och procedurer


///////////////////////////
//Externa globala variabler
                           
                           
///////////////////////////////////
//Interna funktioner och procedurer
void    InitializeRS232( void );
void    SendStringRS232(const char *str);
void    SendStringEOLRS232(const char *str);
char    WaitAndReceiveRS232( void );
void    SendRS232(char tecken);

void    programmLowMem ( void );
void    SendBackAll( void );
 
///////////////////////////
//Interna globala variabler
                           
//////////////////////////
//Interna lokala variabler
                           

void main( void )
{
    char    c;

    InitializeRS232();

    if ((IP0 & 1<<6) == 0) {
        SendStringEOLRS232("The SuperComputer Programmer and FRHA welcomes you, ver2");
    } else {
        SendStringEOLRS232("Dogged!!!!!");
    }
    
    
    while (1) {
        c = WaitAndReceiveRS232();
        switch (c) {
        case 'H' :
            SendStringEOLRS232("Hello from the SuperComputer programmer!");
            break;
        case 'P' :
            programmLowMem();
            break;
        case 'V' :
//            SendBackAll();
            break;
        }
    }
}    
    
    

void    InitializeRS232( void )
{
    EAL = 0;                                                //Stänger av alla avbrott
    
    S1CON = 0x98;                                //Ställer in mode B = 8-bit UART, nollar flaggor och enablar mottagning
    S1RELH = 0x03;                                //Ställer in baudrate 19200 bytes/s                                
    S1RELL = 0xEE;
    ADCON0 = 0x80;                                //Startar baudrate generering
}    


void    SendStringRS232(const char *str)
//Skickar en 0-terminerad sträng på RS232 porten.
{
    int   i, l;
    
    l = strlen(str);
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //Sänder tecken     
        WDT = 1;
        SWDT = 1;
    }
}        

void    SendStringEOLRS232(const char *str)
//Skickar en 0-terminerad sträng på RS232 porten med "Carrige Return" och "Line Feed" efterråt.
{
    int   i, l;
                                                        
    l = strlen(str);                                    //Längden på strängen
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //Sänder tecken     
        WDT = 1;
        SWDT = 1;
    }
    SendRS232(13);                                      //"Carrige Return"
    SendRS232(10);                                      //"Line Feed"
}        


char   WaitAndReceiveRS232( void )
//Väntar tills det finns ett tecken på RS232 porten redo för mottagning. Retunerar det tecknet.
{
    char    tecken;

    while(!(S1CON & 1)) {                               //Väntar på mottagning
        WDT = 1;
        SWDT = 1;
    }
    tecken = S1BUF;                                     //Läser mottaget tecken   
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          

    return tecken;
}

void    SendRS232(char tecken)
//Sänder ett tecken på RS232-porten och väntar tills det är sänt.
{
    S1BUF = tecken;                                     //Sänder tecken 
    while(!(S1CON & 2));                                //Väntar på att alla bitar sänts
    S1CON &= ~(2);                                      //Nollar sändnings-interrupt-flagga      
}

    
    
    
void programmLowMem ( void )
{
    char            command;
    char            packLen;
    char xdata *    address;
    unsigned int    adrTmp;
    char            c;
    char            cs;
    char            cc;
    char            checkSum;
    char            index;
    char            buffer[65];
//    char            msg[25];

    SendRS232('P');

    do {
        command = WaitAndReceiveRS232();

        if (command == 'F') {
            c = WaitAndReceiveRS232();          //Väntar på adress (MSB) från PC
            
            adrTmp = (c * 256);                  //MSB av adrespekaren
 
            c = WaitAndReceiveRS232();          //Väntar på adress (LSB) från PC
            
            adrTmp += c;
            address = (char xdata *) adrTmp;
 
            packLen = WaitAndReceiveRS232();          //Väntar på paketlängd från PC
                                    
            checkSum = 0xFF;
             
            for (index = 0 ; index < packLen ; index++) {            
                c = WaitAndReceiveRS232();        //Väntar på databyte från PC
                checkSum ^= c;                  //Beräkna checksum
                buffer[index] = c;
            }
 
            cs = WaitAndReceiveRS232();          //Väntar på checksum från PC
            
            checkSum ^= cs;                      //Beräkna checksum

            if (checkSum == 0) {
                for (index = 0 ; index < packLen ; index++) {            
                    *(address++) = buffer[index];//Skriv databyte i minnet
                    c = buffer[index];
                }
                address--;                          //Senaste address för polling
                     
                cc = c & 0x01;     //Maska fram LSB ur senaste skrivna datat

                do {
                    c = *(address) & 0x01;          //Hämtar den senast inskrivna byten
                    WDT = 1;                        //Nollställer watchdog
                    SWDT = 1;        
                } while (c != cc);                  //Blir lika när minnet är klart
                SendRS232('F');                 //Sänder "proceed" till PC
            } else {
                SendRS232('R');                 //Sänder "resend" till PC
            }
        } else if (command != 'D') {
            SendRS232('?');                        //Känner inte igen kommandot. Sänder fråga till PC
        }
    } while (command != 'D');                   //Fortsätt tills kommando för låga minnet kommer
    
    SendStringRS232("D1D2");

//----------------------
//Debuggavdelning:
/*

    SendStringEOLRS232("From EEPROM 0x8000:");
    address = (char xdata *) 0x8000;
    for (index = 0 ; index < 64 ; index++ ) {
        c = *(address++);
        sprintf(msg, "At %X, Read: ", (int) address);
        SendStringRS232(msg);    
        sprintf(msg, "%X", c);
        SendStringEOLRS232(msg);    
    }
*/ 
//Slut på debuggavdelning
//----------------------

}


void    SendBackAll( void )
{

}  


