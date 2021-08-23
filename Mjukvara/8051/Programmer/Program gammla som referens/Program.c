/*************************************************************
Fil:            Program.c

Beskrivning:    Sj�lvst�ndig applikation f�r att programmera 
                det l�ga programminnet. K�rs genom att PLD 
                st�lls in f�r programmering genom en strapp 
                (ev en switch p� "burken").

Skapad:         99-09-19

�gare:          Fredrik Hoffman
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
    EAL = 0;                                                //St�nger av alla avbrott
    
    S1CON = 0x98;                                //St�ller in mode B = 8-bit UART, nollar flaggor och enablar mottagning
    S1RELH = 0x03;                                //St�ller in baudrate 19200 bytes/s                                
    S1RELL = 0xEE;
    ADCON0 = 0x80;                                //Startar baudrate generering
}    


void    SendStringRS232(const char *str)
//Skickar en 0-terminerad str�ng p� RS232 porten.
{
    int   i, l;
    
    l = strlen(str);
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //S�nder tecken     
        WDT = 1;
        SWDT = 1;
    }
}        

void    SendStringEOLRS232(const char *str)
//Skickar en 0-terminerad str�ng p� RS232 porten med "Carrige Return" och "Line Feed" efterr�t.
{
    int   i, l;
                                                        
    l = strlen(str);                                    //L�ngden p� str�ngen
    for (i = 0 ; i < l ; i++) {
        SendRS232(*(str + i));                          //S�nder tecken     
        WDT = 1;
        SWDT = 1;
    }
    SendRS232(13);                                      //"Carrige Return"
    SendRS232(10);                                      //"Line Feed"
}        


char   WaitAndReceiveRS232( void )
//V�ntar tills det finns ett tecken p� RS232 porten redo f�r mottagning. Retunerar det tecknet.
{
    char    tecken;

    while(!(S1CON & 1)) {                               //V�ntar p� mottagning
        WDT = 1;
        SWDT = 1;
    }
    tecken = S1BUF;                                     //L�ser mottaget tecken   
    S1CON &= ~(1);                                   //Nollar mottagnings-interrupt-flagga          

    return tecken;
}

void    SendRS232(char tecken)
//S�nder ett tecken p� RS232-porten och v�ntar tills det �r s�nt.
{
    S1BUF = tecken;                                     //S�nder tecken 
    while(!(S1CON & 2));                                //V�ntar p� att alla bitar s�nts
    S1CON &= ~(2);                                      //Nollar s�ndnings-interrupt-flagga      
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
            c = WaitAndReceiveRS232();          //V�ntar p� adress (MSB) fr�n PC
            
            adrTmp = (c * 256);                  //MSB av adrespekaren
 
            c = WaitAndReceiveRS232();          //V�ntar p� adress (LSB) fr�n PC
            
            adrTmp += c;
            address = (char xdata *) adrTmp;
 
            packLen = WaitAndReceiveRS232();          //V�ntar p� paketl�ngd fr�n PC
                                    
            checkSum = 0xFF;
             
            for (index = 0 ; index < packLen ; index++) {            
                c = WaitAndReceiveRS232();        //V�ntar p� databyte fr�n PC
                checkSum ^= c;                  //Ber�kna checksum
                buffer[index] = c;
            }
 
            cs = WaitAndReceiveRS232();          //V�ntar p� checksum fr�n PC
            
            checkSum ^= cs;                      //Ber�kna checksum

            if (checkSum == 0) {
                for (index = 0 ; index < packLen ; index++) {            
                    *(address++) = buffer[index];//Skriv databyte i minnet
                    c = buffer[index];
                }
                address--;                          //Senaste address f�r polling
                     
                cc = c & 0x01;     //Maska fram LSB ur senaste skrivna datat

                do {
                    c = *(address) & 0x01;          //H�mtar den senast inskrivna byten
                    WDT = 1;                        //Nollst�ller watchdog
                    SWDT = 1;        
                } while (c != cc);                  //Blir lika n�r minnet �r klart
                SendRS232('F');                 //S�nder "proceed" till PC
            } else {
                SendRS232('R');                 //S�nder "resend" till PC
            }
        } else if (command != 'D') {
            SendRS232('?');                        //K�nner inte igen kommandot. S�nder fr�ga till PC
        }
    } while (command != 'D');                   //Forts�tt tills kommando f�r l�ga minnet kommer
    
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
//Slut p� debuggavdelning
//----------------------

}


void    SendBackAll( void )
{

}  


