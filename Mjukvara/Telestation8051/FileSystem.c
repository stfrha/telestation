/*************************************************************
Fil:            $Id: FileSystem.c,v 1.4 2003/11/20 22:08:52 Gemensam Exp $

Beskrivning:    Procedurer och funktioner f�r hantering av
                FLASH-minnen och filhantering

Skapad:         99-09-20

�ndrad:         $Date: 2003/11/20 22:08:52 $

�gare:          Fredrik Hoffman

Revisionshistoria:
				$Log: FileSystem.c,v $
				Revision 1.4  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/18 19:17:36  Gemensam
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
#include <string.h>
#include "main.h"
#include "ErrorCodes.h"
#include "FileSystem.h"
#include "RS232.h"



//////////////////////
// Globala variabler

extern xdata int	error;				//Error-kod
extern xdata char	timeout;			//Tidsm�tningsvariabel
extern xdata char	msg[100];
extern xdata char	commandLine[CM_PARSIZE][CM_NRPAR];	//H�r lagras kommandot och alla parametrar
										//Varje kommando/parameter f�r vara max cmParSize tecken
										//Till varje kommando kan cmNrPar-1 kopplas

xdata long          writeAddress;		//Inneh�ller den address som en skriv-operation kommer att anv�nda.
xdata int           FLASHWriteState;	//H�ller reda p� i vilket tillst�nd skriv-sekvensen befinner sig enl:
										// OFF = SetupWriteSeq ej k�rd, skrivning ej m�jlig.
										// READY = SetupWriteSeq har k�rts, data kan l�ggas in i registret.
										// NEMPTY = Registret inneh�ller data, skrivning till celler kan g�ras.
										// WRITING = Programmeringen har startats (250 us). Inf�r n�sta 
										//           skrivning m�ste SetupWriteSeq k�ras.


//////////////////////
// Lokala variabler

xdata FILE          fs_files[FS_MAXFILES];
xdata FILE*         fs_currFilePtr;
xdata char          FLASHKey;			//Nyckel som m�ste inneh�lla 0xAA f�r att det skall g� att radera
										//flashminnet. Denna s�tts till noll av EraseBlock().         
xdata int           dataFieldCounter;	//H�ller reda p� var i en sida man skriver. Anv�nds f�r att man
										//skall veta n�r man skall utf�ra skrivningen vid sid-byte.
xdata long          readAddress;		//Inneh�ller den address som en l�s-operation kommer att anv�nda.
xdata long          eraseAddress;		//Det 8Kbyte-block som inneh�ller adressen som denna pekar p�
										//kommer att raderas med EraseBlock() 
xdata char          writeInProgress;	//Indikerar att n�gon fil �r �ppen f�r skrivning, vilket skall
										//hindra andra filer fr�n att �ppnas f�r skrivning

//////////////////////
// Kod

void    InitializeFileSystem( void )
{
    xdata char    i;
    
    for (i=0 ; i<FS_MAXFILES ; i++) {
        fs_files[i].fileName[0] = 0;
    }
    
    fs_currFilePtr = NULL;
    writeInProgress = 0;
}
        


void InitializeFlash( void )
{
    SE = 1;             //Endast dataf�lten anv�nds i en sida, inte det extra f�ltet (spare-field).
                        //Detta ger 512 bytes per sida.
                
    CLE = 0;            //Ej i kommandoskrivningsmod
    ALE = 0;            //Ej i addressskrivningsmod 
}                                                                        

    
            
        

/////////////////////////////////////////////////////////////////////
// L�sning fr�n FLASH-minnet

void SetupReadSeq( void )
//S�tter upp FLASH-minnet p� adressen som finns i variabeln readAddress s� att ReadFLASH() 
//kan anv�ndas f�r att l�sa en byte.
{
    SE = 1;             //Endast dataf�lten anv�nds i en sida, inte det extra f�ltet (spare-field).
 
//    FREG = 8 + 0;
    FREG = ((readAddress >> 22) & 0x03) + 0x08;            //V�ljer r�tt FLASH-chip och s�tter dess CE till 0
    ALE = 0;                                            //S�tter minnet i Kommandomod
    CLE = 1;
    
    FLASH = ((readAddress >> 8) & 1);                  //Adresserar f�rsta eller andra sidh�lften med 
                                                        //kommandokod 0x00 eller 0x01, dvs kommando = bit 8 i adressen
    
    CLE = 0;                                            //S�tter minnet i adresseringsmod
    ALE = 1;
    
    FLASH = (readAddress & 0xFF);                      // A0 - A7 OBS: A8 skriven i kommandot ovan
    FLASH = ((readAddress >> 9) & 0xFF);               // A9 - A16
    FLASH = ((readAddress >> 17) & 0xFF);              //A17 - A21

    ALE = 0;                                            //S�tter minnet i datamod
    
 
//OBS: V�ntan p� att FLASH-minnet skall f�ra �ver data fr�n FLASH-arean till registerarean g�rs
//inte h�r! Endast v�ntan tills BUSY aktiveras (g�r l�g) g�rs h�r. Processorn kan sedan jobba 
//under tiden som FLASH-minnet jobbar. Detta g�r att innan varje l�sning m�ste BUSY-flaggan kollas.

    while (FBUSY);                                      //V�ntar tills busy g�r l�g (max 100 ns enligt datablad)
 
}

void SetupSpareReadSeq( void )
//S�tter upp FLASH-minnet p� "spare"-arean vid adressen som finns i variabeln readAddress s� att ReadFLASH() 
//kan anv�ndas f�r att l�sa en byte.
{
    SE = 0;             //B�de dataf�lten och det extra f�ltet (spare-field) anv�nds.
 
//    FREG = 8 + 0;
    FREG = ((readAddress >> 22) & 0x03) + 0x08;            //V�ljer r�tt FLASH-chip och s�tter dess CE till 0
    ALE = 0;                                            //S�tter minnet i Kommandomod
    CLE = 1;
    
    FLASH = 0x50;                                       //Adresserar "spare" arean
    
    CLE = 0;                                            //S�tter minnet i adresseringsmod
    ALE = 1;
    
    FLASH = (readAddress & 0xFF);                      // A0 - A7 OBS: A8 skriven i kommandot ovan
    FLASH = ((readAddress >> 9) & 0xFF);               // A9 - A16
    FLASH = ((readAddress >> 17) & 0xFF);              //A17 - A21

    ALE = 0;                                            //S�tter minnet i datamod
    
 
//OBS: V�ntan p� att FLASH-minnet skall f�ra �ver data fr�n FLASH-arean till registerarean g�rs
//inte h�r! Endast v�ntan tills BUSY aktiveras (g�r l�g) g�rs h�r. Processorn kan sedan jobba 
//under tiden som FLASH-minnet jobbar. Detta g�r att innan varje l�sning m�ste BUSY-flaggan kollas.

    while (FBUSY);                                      //V�ntar tills busy g�r l�g (max 100 ns enligt datablad)
 
}

unsigned char ReadFLASH( void )
//H�mtar en byte ifr�n FLASH-minnet. Kan endast kallas efter att SetupReadSeq() har k�rts.
{      
    xdata char   c;

    while (!FBUSY);                                     //V�ntar tills busy g�r h�g...
    readAddress++;                                     //...(vid sidbyte max 10 us enl. datablad, annars 0)
//    return FLASH;
    c = FLASH;
    if ((readAddress & 0x3FFFFF) == 0) SetupReadSeq();    //Detta kommer endast att g�ras efter
                                                        //att adress 0x3FFFFF har skrivits
    return c;
}

void TerminateReadSeq( void )
//Avslutar en l�ssekvens. Efter detta m�ste SetupReadSeq() k�ras innan ReadFLASH() kan anv�ndas igen.
{
    FREG = 0;                                           //FLASH-minnet �r inte l�ngre valt
}





/////////////////////////////////////////////////////////////////////
// Skrivning till FLASH-minnet

//Kommenarer till skrivning av minnet:
//FLASH minnet har en register area och en cell area. Registerarean inneh�ller 512 bytes och 
//best�r av vanliga register som allts� inte beh�ller informationen efter str�mavslag. Cellarean
//�r sj�lva FLASH-minnet (4 Mbyte). Cellarean �r uppdelad i sidor (page) om 512 bytes. N�r man 
//skriver till minnet skriver (programmerar) man en sida i taget. F�rst startar man skrivkommandot
//och anger vilken adress man vill skriva till. Man m�ste inte b�rja p� f�rsta adressen i en sida.
//Sedan skriver man datat som man vill lagra till registerarean. N�r man �r klar utf�r man ett kommando 
//som f�r �ver data fr�n registerarean till cellarean. N�r det �r gjort m�ste man starta om skrivkommandot.
//Oavsett var p� sidan man b�rjar m�ste man starta �verf�ringen n�r man n�r slutet
//registerarean.
//Implementationen h�r automatiserar detta n�got. En global variabel (writeAddress) inneh�ller den 
//adress som man skall skriva till. Man kallar p� SetupWriteSeq() som st�ller in minnet f�r skrivning.
//Sedan anv�nder man WriteFLASH(byte) f�r att l�gga in data i minnet. WriteFLASH(byte) kollar om 
//man n�r slutet p� en sida och startar d� en �verf�ring av data (register till cell) automatiskt.
//WriteFLASH(byte) kollar ocks� om en �verf�ring h�ller p�. Om den g�r det s� v�ntar funktionen
//tills den �r klar, k�r SetupWriteSeq() f�r att starta om skrivningsf�rfarandet p� nytt och l�gger
//in det �nskade datat p� f�rsta platsen i registerarean. Allts� beh�ver man inte kalla p� n�got annat 
//�n SetupWriteSeq() en g�ng, och sedan WriteFLASH() tills man inte vill skriva n�got till 
//minnet l�ngre. D� kallar man p� PerformWriteSeq() som startar �verf�ringen och sedan 
//TerminateWriteSeq() som avslutar skrivningen. Om man inte vill g�ra n�got annat med FLASH-minnet
//kan man ha skrivsekvensen ig�ng under hela applikationsk�rningen. 

void SetupWriteSeq( void )
//Initierar ett skrivkommando till FLASH-minnet. WriteFLASH(byte) anv�nds sedan f�r att skriva data till
//minnet. Datat hamnar p� adressen angiven i writeAddress.
{
    if (FLASHWriteState == OCCUPIED) {
    
        error = EM_OUTOFFILEMEMORY;

    } else {
    
        if (writeAddress >= FS_FLASHSIZE) {
            FLASHWriteState = OCCUPIED;
            error = EM_OUTOFFILEMEMORY;
        } else {
            SE = 1;             //Endast dataf�lten anv�nds i en sida, inte det extra f�ltet (spare-field).

        //    FREG = 8 + 0;
            FREG = ((writeAddress >> 22) & 0x03) + 0x08;        //V�ljer r�tt FLASH-chip och s�tter dess CE till 0
            ALE = 0;                                            //S�tter minnet i Kommandomod
            CLE = 1;
    
            FLASH = ((writeAddress >> 8) & 1);                 //Adresserar f�rsta eller andra sidh�lften med 
                                                                //kommandokod 0x00 eller 0x01, 
                                                                //dvs kommando = bit 8 i adressen
    
            FLASH = 0x80;                                       //S�tter minnet i programmeringsmod

            CLE = 0;                                            //S�tter minnet i adresseringsmod
            ALE = 1;
    
            FLASH = (writeAddress & 0xFF);                     // A0 - A7 OBS: A8 skriven i kommandot ovan
            FLASH = ((writeAddress >> 9) & 0xFF);              // A9 - A16
            FLASH = ((writeAddress >> 17) & 0xFF);             //A17 - A21

            ALE = 0;                                            //S�tter minnet i datamod
    
            dataFieldCounter = (writeAddress & 0x1FF);
    
            FLASHWriteState = READY;
        }
    }
}

void SetupSpareWriteSeq( void )
//Initierar ett skrivkommando till FLASH-minnet. WriteFLASH(byte) anv�nds sedan f�r att skriva data till
//minnet. Datat hamnar p� adressen angiven i writeAddress.
{
    if (FLASHWriteState == OCCUPIED) {
    
        error = EM_OUTOFFILEMEMORY;

    } else {
        if (FLASHWriteState != OCCUPIED) {
            SE = 0;             //B�de dataf�lten och det extra f�ltet (spare-field) anv�nds.

        //    FREG = 8 + 0;
            FREG = ((writeAddress >> 22) & 0x03) + 0x08;        //V�ljer r�tt FLASH-chip och s�tter dess CE till 0
            ALE = 0;                                            //S�tter minnet i Kommandomod
            CLE = 1;
    
            FLASH = 0x50;                                       //Adresserar "spare"-arean 
    
            FLASH = 0x80;                                       //S�tter minnet i programmeringsmod

            CLE = 0;                                            //S�tter minnet i adresseringsmod
            ALE = 1;
    
            FLASH = (writeAddress & 0xFF);                     // A0 - A7 OBS: A8 skriven i kommandot ovan
            FLASH = ((writeAddress >> 9) & 0xFF);              // A9 - A16
            FLASH = ((writeAddress >> 17) & 0xFF);             //A17 - A21

            ALE = 0;                                            //S�tter minnet i datamod
    
            dataFieldCounter = (writeAddress & 0x1FF);
    
            FLASHWriteState = READY;
        }
    }
}

void WriteFLASH(unsigned char byte)
//Skriver en byte till FLASH-minnet. Datat hamnar p� addressen angiven i writeAddress. Denna �kas 
//automatiskt varje g�ng.
{                              
    if (FLASHWriteState == OCCUPIED) {
        error = EM_OUTOFFILEMEMORY;
    } else {
        if (FLASHWriteState == WRITING) {             //Programmering utf�rs, m�ste v�nta tills det �r klart
            while (!FBUSY);
            SetupWriteSeq();
        }
 
        FLASH = byte;
        dataFieldCounter++;
        writeAddress++;
        FLASHWriteState = NEMPTY;
        
        if (dataFieldCounter == 512) {
            PerformWriteSeq();
        }
    }
}

void TerminateWriteSeq( void )
//Avslutar en skrivsekvens. 
{
    if (FLASHWriteState == WRITING) {                 //Programmering utf�rs, m�ste v�nta tills det �r klart
        while (!FBUSY);
    }

//    CE = 1;                                             //FLASH-minnet �r inte l�ngre valt
    FREG = 0;                                           //FLASH-minnet �r inte l�ngre valt
    FLASHWriteState = OFF;
}


void PerformWriteSeq( void )
//Utf�r en skrivsekvens. Om det inte finns n�got inskrivet i registerarean s� g�rs ingenting.
{
    if (FLASHWriteState == NEMPTY) {                  //Skall endast utf�ras om det har skrivits saker till registret
        CLE = 1;                                        //S�tter minnet i kommandomod
        FLASH = 0x10;                                   //Startar programmering av FLASH-celler. Tar ca 250 us enl. datablad.
        FLASHWriteState = WRITING;
        CLE = 0;                                        //S�tter minnet i datamod
    }
}

/////////////////////////////////////////////////////////////////////
// Radering av FLASH-minnet
 
void EraseBlock( void )
{
    if (FLASHKey == 0xAA) {                             //Nyckeln m�ste vara satt till 0xAA f�r att 
                                                        //radering skall utf�ras
        FLASHKey = 0;                                   //Nollst�ller nyckeln

        FREG = ((eraseAddress >> 22) & 0x03) + 0x08;        //V�ljer r�tt FLASH-chip och s�tter dess CE till 0
//        CE = 0;                                         //FLASH-minnet �r valt
        ALE = 0;                                        //S�tter minnet i Kommandomod
        CLE = 1;
    
        FLASH = 0x60;                                   //"Block erase"-kommando

        CLE = 0;                                        //S�tter minnet i adresseringsmod
        ALE = 1;
    
        FLASH = ((eraseAddress >> 9) & 0xFF);          // A9 - A16
        FLASH = ((eraseAddress >> 17) & 0xFF);         //A17 - A21

        ALE = 0;                                        //S�tter minnet i kommandomod
        CLE = 1;
    
        FLASH = 0xD0;                                   //Raderingskommando
        
        CLE = 0;
        
        while (!FBUSY) {
            WDT = 1;
            SWDT = 1;
        }
        FREG = 0;
    }
}

/////////////////////////////////////////////////////////////////////
// �vrig FLASH-minnes hantering
 

/*
void    SeekWriteEnd( void )
//Letar upp den f�rsta lediga adress i FLASH-minnet. Om datorn har f�tt en ofrivillig reset
//kan man forts�tta att skriva d�r man slutade.
{
    xdata long          mask;
    xdata long          seek_address;
    xdata unsigned char empty;
    
    seek_address = 0;
    mask = 0x200000;

    do {
        readAddress = (seek_address | mask);
        empty = 1;                                      //F�ruts�tt oskrivet paket
        SetupReadSeq();  
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (ReadFLASH() != 0xFF) empty = 0;
        if (!empty) {
            seek_address = readAddress - 8;
        }
        mask = mask >> 1;
    } while (mask > 0x4);                               //seek_address pekar p� sista skrivna paketet
    if ((seek_address == 0) && (empty)) {
         writeAddress = 0;
    } else {
        writeAddress = seek_address + 8;               //writeAddress pekar p� f�rsta lediga plats
    }
}
*/
 
char    GetFileHead(FILE_HEAD* fh)
//Tittar p� "spare" arean tillh�rande readAddress. Om det �r ett filhuvud fylls fh i och 
//'1' returneras. Om det inte �r ett filhuvud retuneras '0' och  fh skall ses som odefinierad.
{   
    xdata char    c;
    xdata char    i, j;
                  
    fs_currFilePtr = NULL;

    SetupSpareReadSeq();
    if (ReadFLASH() != 0xAA) {                  //�r denna "spare"-area ett filhuvud??
        TerminateReadSeq();
        return 0;
    }                      
    fh->length =  (ReadFLASH() * 0x1000000) & 0xFF000000;                 //H�mta length MSB
    fh->length += (ReadFLASH()  * 0x10000) & 0xFF0000;
    fh->length += (ReadFLASH() * 0x100) & 0xFF00;
    fh->length += (ReadFLASH() & 0xFF);                          //H�mta length LSB

    j = 0;
    for (i=0 ; i<8 ; i++) {
        c = ReadFLASH();
        if (c != 0) fh->name[j++] = c;
    }
    c = ReadFLASH();
    if (c != 0) {
        fh->name[j++] = '.';
        fh->name[j++] = c;
    }
    c = ReadFLASH();
    if (c != 0) fh->name[j++] = c;
    c = ReadFLASH();
    if (c != 0) fh->name[j++] = c;

    fh->name[j] = 0;

    TerminateReadSeq();
    
    return 1;        
}

FILE*   FindEmptyFileStruct( void )
{
    xdata char    i;
    
    for (i=0 ; i<FS_MAXFILES ; i++) {
        if (fs_files[i].fileName[0] == 0) return &(fs_files[i]);
    }
    error = EM_TOOMANYFILES;
    return NULL;
}

char    FileOpen(const char* name)
//Retunerar 1 om filen �r �ppen, annars 0
{
    xdata char    i;
    
    for (i=0 ; i<FS_MAXFILES ; i++) {
        if (!myStrCmpNoCase(fs_files[i].fileName, name)){
            return 1;
        }
    }
    return 0;
}

FILE*   fopen(const char* name, const char* def)
{
    xdata FILE*       tmpFile;
    xdata FILE_HEAD   fh;
    xdata char        done;
 
    if (FileOpen(name)) {
        error = EM_FILEOPEN;
        return NULL;
    }

    tmpFile = FindEmptyFileStruct();
    if (tmpFile == NULL) {
        error = EM_TOOMANYFILES;
        return NULL;
    }
            
    if (def[0] == 'r') {

        readAddress = 0;
                     
        fs_currFilePtr = NULL;                                  //Antag att det blir fel nedan. Detta m�ste
                                                                //g�ras eftersom FLASH-pekaren flyttats
        done = 0;
         
        do {
            if (!GetFileHead(&fh)) {
                error = EM_FILENOTEXISTS;
                return NULL;
            }
            if (readAddress >= FS_FLASHSIZE) {
                error = EM_FILENOTEXISTS;
                return NULL;
            }
            if (myStrCmpNoCase(name, fh.name)) {
                readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
            } else {
                done = 1;
            }
        } while (!done);

        myStrCpy(tmpFile->fileName, name);
        tmpFile->startAddress = readAddress & 0xFFFFFE00;
        tmpFile->offset = 0;
        tmpFile->length = fh.length;
        tmpFile->write = 0;
        tmpFile->append = 0;
        
        readAddress = tmpFile->startAddress;
        SetupReadSeq();
        fs_currFilePtr = tmpFile;            
        
        return tmpFile;
        
    } else if (def[0] == 'w') {
        if (writeInProgress) {
            error = EM_WRITEFILEOPEN;
            return NULL;
        }

        readAddress = 0;
                     
        fs_currFilePtr = NULL;                                  //Antag att det blir fel nedan. Detta m�ste
                                                                //g�ras eftersom FLASH-pekaren flyttats

        while (GetFileHead(&fh)) {
            readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
            if (readAddress >= FS_FLASHSIZE) {
                error = EM_OUTOFFILEMEMORY;
                return NULL;
            }
            if (!myStrCmpNoCase(fh.name, name)) {
                error = EM_FILEEXISTS;
                return NULL;
            }
        }
        
        myStrCpy(tmpFile->fileName, name);
        tmpFile->startAddress = readAddress & 0xFFFFFE00;
        tmpFile->length = 0;
        tmpFile->offset = 0;
        tmpFile->write = 1;
        tmpFile->append = 0;

        writeAddress = tmpFile->startAddress;
        SetupWriteSeq();
        fs_currFilePtr = tmpFile;            
        writeInProgress = 1; 
        
        return tmpFile;
    } else {
        //Illegal fopen call
    }
    return NULL;
}

void    fputc(FILE* filePtr, char c)
{
    if (filePtr->fileName[0] == 0) {
        error = EM_FILENOTOPEN;
    } else if ((!filePtr->write) && (!filePtr->append)) {
        error = EM_FILEREADONLY;
    } else {
        if (fs_currFilePtr != filePtr) {

            ReleaseCurrFile();

            if (filePtr->write) {
                writeAddress = filePtr->startAddress + filePtr->length;
            } else {
                writeAddress = filePtr->startAddress + filePtr->offset;
            }
            SetupWriteSeq();
            fs_currFilePtr = filePtr;
            
        }
        if (filePtr->write) {
            filePtr->length++;
            WriteFLASH(c);
        } else {
            filePtr->offset++;
            if (filePtr->offset < filePtr->length) {
                WriteFLASH(c);
            }
        }
    }
}

/*

void    fputLine(FILE* filePtr, const char* str)
{
    int     i, l;
                
    l = myStrLen(str);
    for (i=0 ; i<l ; i++) {
        fputc(filePtr, str[i]);
    }
    fputc(filePtr, '\r');
    fputc(filePtr, '\n');
}

*/

int     fgetc(FILE* filePtr)
{
//    SendRS232('1');
 
    if (filePtr->fileName[0] == 0) {
        error = EM_FILENOTOPEN;
    } else if (filePtr->write) {
        error = EM_FILEWRITEONLY;
    } else {
        if (fs_currFilePtr != filePtr) {

            ReleaseCurrFile();

            readAddress = filePtr->startAddress + filePtr->offset;
            SetupReadSeq();
            fs_currFilePtr = filePtr;
        }
//        SendRS232('2');
        if (filePtr->offset >= filePtr->length) {
            return EOF;
        } else {
//            SendRS232('3');
            filePtr->offset++;
//            SendRS232('4');
            return ReadFLASH();
        }
    }
    return EOF;
}

void    fgetLine(FILE* filePtr, char* str, int maxLen)
//L�gger alla tecken fr�n filen i "str" tills antingen maxLen tecken h�mtats eller
//'\r'-tecknet har h�mtats. "str" nolltermineras (utan '\r'-teckent) och tecknet efter
//'\r'-tecknet glufsas (antas vara '\n'-tecknet. Icke synliga tecken (mindre �n 32 i 
//ASCII-tabellen) �ts upp men lagras inte i str�ngen. Denna funktion �r till f�r att l�sa text.
{
    xdata int     i = 0;
    xdata char    c;

    do {
        c = fgetc(filePtr);
        if (c >= ' ') str[i++] = c;
        
    } while ((i < maxLen) && (c != '\r'));
    if (c == '\r') c = fgetc(filePtr);              //Glufsa det f�rmodade '\n'-tecknet
    str[i] = 0;
}

void    ReleaseCurrFile( void )
{
    if (fs_currFilePtr->write) {
        if (FLASHWriteState != OFF) {
            PerformWriteSeq();
            TerminateWriteSeq();
        }
    } else {
        TerminateReadSeq();
    }
    fs_currFilePtr = NULL;
}

void    fclose(FILE* filePtr)
{               
    if (filePtr->fileName[0] == 0) {
        error = EM_FILENOTOPEN;
    } else {
        if (fs_currFilePtr == filePtr) {
            if ((filePtr->write) || (filePtr->append)) {
                if (FLASHWriteState != OFF) {
                    PerformWriteSeq();
                    TerminateWriteSeq();
                }
                if (filePtr->write) {
                    writeAddress = filePtr->startAddress;
                    WriteFileHead(filePtr->fileName, filePtr->length);
                }

                writeInProgress = 0;
            } else {
                TerminateReadSeq();
            }
            fs_currFilePtr = NULL;
        }
        filePtr->fileName[0] = 0;
    }
}

long    ftell(FILE* filePtr)
{
    if (filePtr->write) {
        return filePtr->length;
    } else {
        return filePtr->offset;
    }
}


/*
void    Rewind(FILE* filePtr)
{
    if ((filePtr->write) || (filePtr->append)) {
        error = EM_FILEWRITEONLY;
    } else {
        filePtr->offset = 0;
        if (fs_currFilePtr == filePtr) {
            TerminateReadSeq();
            readAddress = filePtr->startAddress;
            SetupReadSeq();
        }
    }
}
*/



void     fSetPos(FILE* filePtr, long offset)
{
    if ((filePtr->write) || (filePtr->append)) {
        error = EM_FILEWRITEONLY;
    } else {
        if (offset < filePtr->length){
            filePtr->offset = offset;
            if (fs_currFilePtr == filePtr) {
                TerminateReadSeq();
                readAddress = filePtr->startAddress + filePtr->offset;
                SetupReadSeq();
            }
        }
    }
}


void    fupLoad( void )
{
    xdata FILE*       fp;
    xdata char        c;
    xdata char        l;
    xdata int         i;
    xdata char        buffer[256];
    xdata char        lerror = 0;
    xdata char        command;
    xdata char        cs;
    

    SendRS232('O');

    c = WaitAndReceiveRS232();

    if (c != 'F') {
        lerror = 1;
        SendRS232('1');         //Debug
    } else {
        i = 0;
        do {                                          
            c = WaitAndReceiveRS232();
            if (timeout) {
                lerror = 1;
                SendRS232('2');         //Debug
            }
            buffer[i++] = c;
        } while ((c != 0x00) && (i < 13) && (!lerror));
        if (!lerror) {
            buffer[i] = 0;
            fp = fopen(buffer, "w");
            if (!fp) {
                SendRS232('I');
            } else {
                SendRS232('O');
                do {
                    command = WaitAndReceiveRS232();
                    if (command == 'L') {
                        l = WaitAndReceiveRS232();
                        if (timeout) {
                            lerror = 1;
//                            SendRS232('3');         //Debug
                        } else {
                            SendRS232('O');
                            cs = 0xFF;
                            for (i=0 ; i<l+1 ; i++) {
                                c = WaitAndReceiveRS232();
                                if (timeout) {
                                    lerror = 1;
//                                    SendRS232('4');         //Debug
                                }
                                buffer[i] = c;
                                cs ^= c;
                            }
                            if (!lerror) {
                                c = WaitAndReceiveRS232();
                                if (timeout) {
                                    lerror = 1;
//                                    SendRS232('5');         //Debug
                                } else {
                                    cs ^= c;
                                    if (cs == 0x00) {
                                        for (i=0 ; i<l+1 ; i++) {
                                            fputc(fp, buffer[i]);
                                        }                            
                                        SendRS232('O');
                                    } else {
                                        SendRS232('R');
                                    }
                                    if (FLASHWriteState == OCCUPIED) {
                                        lerror = 1;
//                                        SendRS232('6');         //Debug
                                    }
                                }
                            }
                        }
                    } else if (command != 'D') {
                        lerror = 1;
//                        SendRS232('7');         //Debug
                    }
                } while ((command == 'L') && (!lerror));
                if (command != 'D') {
                    lerror = 1;
//                    SendRS232('8');         //Debug
                }
                fclose(fp);
            }    
        }
    }    
    if (lerror) {
        SendRS232('E');
    }
}

void    fdownLoad( void )
{   
    xdata char        c;
    xdata char        lerror = 0;
    xdata FILE_HEAD   fh;
    xdata int         index;
    xdata int         i;
    xdata char        okHead;
    xdata long        count;
    xdata FILE*       fp;
    xdata char        cs;
    
    SendRS232('O');
    
    readAddress = 0;
 
    while ((GetFileHead(&fh)) && (!lerror)) {
        c = WaitAndReceiveRS232();
        if (c != 'F') {
            lerror = 1;
        } else {
            SendRS232('F');
            SendStringRS232(fh.name);
            SendRS232(0x00);
            readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
        }
    }
    if (!lerror) {
        c = WaitAndReceiveRS232();
        if (c != 'F') {
            lerror = 1;
        }
    }    
    if (!lerror) {
        SendRS232('D');
        c = WaitAndReceiveRS232();                          //OBS Skall inte vara timeout!!!!!!!!!
        if (c != 'S') {
            if (c == 'C') {
                SendRS232('C');
            } else {
                lerror = 1;
            }
        } else {
            c =  WaitAndReceiveRS232();
            if (!timeout) {
                index = c * 256;
                c =  WaitAndReceiveRS232();
                if (!timeout) {
                    index += c;
//                    SendRS232('W');
//                    SendRS232((index >> 8) & 0xFF);
//                    SendRS232(index & 0xFF);
                    i = 0;
                    readAddress = 0;
                    do {
                        okHead = GetFileHead(&fh);
                        i++;
                        readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
                    } while ((i <= index) && (okHead));
                    if (okHead) {
                        fp = fopen(fh.name, "r");
                    }
                    if ((!fp) || (!okHead)) {
                        SendRS232('O');
                    } else {
                        SendRS232('L');
                        SendRS232((fp->length >> 24) & 0xFF);
                        SendRS232((fp->length >> 16) & 0xFF);
                        SendRS232((fp->length >> 8) & 0xFF);
                        SendRS232(fp->length & 0xFF);
                        c =  WaitAndReceiveRS232();
                        if (c != 'O') {
                            lerror = 1;
                        } else {
                            cs = 0xFF;
                            for (count=0 ; count < fh.length ; count++) {
                                c = fgetc(fp);
                                cs ^= c;
                                SendRS232(c);
                                WDT = 1;
                                SWDT = 1;
                            }
                            SendRS232(cs);
                            SendRS232('D');
                        }
                        fclose(fp);
                    }
                } else {
                    lerror = 1;
                }
            } else {
                lerror = 1;
            }
        }
    }
    if (lerror) {
        SendRS232('E');
    }
}   


void Dir( void )
{
    xdata FILE_HEAD   fh;
//    int         l;
    
//    SendStringEOLRS232("Searching for files...");
 
    readAddress = 0;
        
    while (GetFileHead(&fh)) {
        SendStringRS232(fh.name);      
        SendStringRS232("\t\t"); 

        myLong2HexStr(fh.length, msg);
        SendStringEOLRS232(msg);
/*
        l = (fh.length >> 16) & 0xFFFF;
        sprintf(msg, "0x%04X", l);
        SendStringRS232(msg);
        l = fh.length & 0xFFFF;
        sprintf(msg, "%04X", l);
        SendStringEOLRS232(msg);
*/
        readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
    }
}
 
 
// Startaddress skall bli 1F0000 enl ovan



void Type( void )
{
    xdata FILE*   fp;
    xdata long    i;
    
    fp = fopen(commandLine[1], "r");
    if (fp) {
        for (i=0 ; i<fp->length ; i++) {
            SendRS232(fgetc(fp));
            WDT = 1;
            SWDT = 1;
        }
        fclose(fp);
//    } else {
//        SendStringEOLRS232("Unable to open file");
    }
}


 

/******************************************************************************
En fil av typen A (append) �ppnas f�r skrivning med kommandot fopenA. Vill man
l�sa i filen skall den �ppnas med kommandot fopen(...., "r"); Det g�r att �ppna
filen f�r skrivning efter att den har varit �ppen f�r l�sning.
N�r fopenA har k�rts kan fputc, fputLine och fclose anv�ndas som vanligt. Om filen
har �ppnats med fopen(...., "w"); anv�nds fgetc, fgetLine och fclose som vanligt.
Om f�rsta lediga filpos �r k�nd kan den skickas med parametern offset. Annars, om
-1 skickas i offset, hittas skrivstart-positionen automatiskt, men det kan ta tid 
f�r stora filer
*/

    
FILE*   fopenA(const char* name, long maxSize, long offset)
{   
    xdata FILE*       tmpFile;
    xdata FILE_HEAD   fh;
    xdata char        done;
    xdata char        exists;
    xdata char        endFound;
    xdata long        fileEnd;
    xdata char        i;
 
    if (FileOpen(name)) {
        error = EM_FILEOPEN;
        return NULL;
    }

    tmpFile = FindEmptyFileStruct();
    if (tmpFile == NULL) {
        error = EM_TOOMANYFILES;
        return NULL;
    }

    readAddress = 0;
                     
    fs_currFilePtr = NULL;                                  //Antag att det blir fel nedan. Detta m�ste
                                                                //g�ras eftersom FLASH-pekaren flyttats
    done = 0;
    exists = 1;
         
    do {
        if (!GetFileHead(&fh)) {
            exists = 0;
        }
        if (readAddress >= FS_FLASHSIZE) {
            exists = 0;
        }
        if (exists) {
            if (myStrCmpNoCase(name, fh.name)) {
                readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
//                SendStringEOLRS232("Found one file");
//                SendStringEOLRS232(fh.name);
                
            } else {
                done = 1;
            }
        }
    } while ((!done) && (exists));

    myStrCpy(tmpFile->fileName, name);
    tmpFile->startAddress = readAddress & 0xFFFFFE00;

//    SendStringRS232("Startaddress: ");
//    myLong2HexStr(tmpFile->startAddress, msg);
//    SendStringEOLRS232(msg);

    if (exists) {
        tmpFile->length = fh.length;
        tmpFile->write = 0;
        tmpFile->append = 1;
        if (offset == -1) {
       
            readAddress = tmpFile->startAddress;
        
            fileEnd = readAddress + fh.length;
        
            SetupReadSeq();  
        
            endFound = 0;
        
            while ((!endFound) && (readAddress < fileEnd)) {
                while ((ReadFLASH() != 0xFF) && (readAddress < fileEnd)) {
                    WDT = 1;
                    SWDT = 1;
                }
                if (readAddress < fileEnd) {
                    for (i=0 ; (i<7) && (ReadFLASH() == 0xFF) ; i++);
                    if (i == 7) endFound = 1;
                }
            }

            writeAddress = readAddress - 8;
            tmpFile->offset = writeAddress - tmpFile->startAddress;
        } else {       
            writeAddress = offset + tmpFile->startAddress;
            readAddress  = writeAddress - 1;
            SetupReadSeq();
//            ReadFLASH();
            tmpFile->offset = offset;
        } 

/*        myLong2HexStr(tmpFile->offset, msg);
        SendStringRS232("Offset: ");
        SendStringEOLRS232(msg);
         
        myLong2HexStr(tmpFile->startAddress, msg);
        SendStringRS232("Start address: ");
        SendStringEOLRS232(msg);
 
        myLong2HexStr(writeAddress, msg);
        SendStringRS232("Write Address: ");
        SendStringEOLRS232(msg);
*/
    } else {
/*    
        SendStringEOLRS232("New append file");
        SendStringEOLRS232(name);
        SendStringRS232("Size: ");
        myLong2HexStr(maxSize, msg);
        SendStringEOLRS232(msg);
*/  
        writeAddress = tmpFile->startAddress;
        
        WriteFileHead(name, maxSize);

        tmpFile->length = maxSize;
        tmpFile->offset = 0;
        tmpFile->write = 0;
        tmpFile->append = 1;

        writeAddress = tmpFile->startAddress;
    }        

    SetupWriteSeq();
    fs_currFilePtr = tmpFile;            
    writeInProgress = 1; 
    
    return tmpFile;
}



void    WriteFileHead(const char* name, long size)
{
    xdata char    i, c, j;

    SetupSpareWriteSeq();

    WriteFLASH(0xAA);                                   //Huvud
    WriteFLASH((size >> 24) & 0xFF);         //Length MSB
    WriteFLASH((size >> 16) & 0xFF);
    WriteFLASH((size >> 8) & 0xFF);
    WriteFLASH(size & 0xFF);                 //Length LSB
 
    i = 0;                                                //Filtitel
    j = 0;
    while(((c = name[j++]) != 0) && (c != '.') && (i < 8)) {
        i++;
        WriteFLASH(c);
    }
    for ( ; i<8 ; i++) {
        WriteFLASH(0);
    }
    if (c != 0) {                                             //Filext
        i = 9; 
        while(((c = name[j++]) != 0) && (i < 12)) {
            WriteFLASH(c);
        }
    }
    for ( ; i<12 ; i++) {
        WriteFLASH(0);
    }
    PerformWriteSeq();
    TerminateWriteSeq();
}    
    
    

void    StartDynamic( void )
{
    xdata FILE_HEAD   fh;
    xdata FILE*       fp;
    xdata long        lastStart;
    xdata long        lastLength;
    xdata long        dynamicBreak;
    xdata long        breakLen;
    
    fs_currFilePtr = NULL;
    readAddress = 0;
    lastStart = 0;
        
        
    while (GetFileHead(&fh)) {
        lastStart = readAddress;
        lastLength = fh.length;
        readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
        
    }
    dynamicBreak = ((lastStart + lastLength) & 0xFFFFE000) + 0x2000;
    breakLen = dynamicBreak - lastStart;

//    SendStringRS232("Creating file with size: ");
//    myLong2HexStr(breakLen, msg);
//    SendStringEOLRS232(msg);
 
    fp = fopenA("StrtDymk.xxx", breakLen, -1);
    if (!fp) {
        SendStringEOLRS232("Could not create dynamic break");
    } else {
        fputc(fp, 'A');
        fclose(fp);
    }
}

void    EraseDynamic( void )
{    
    xdata FILE*   fp;
    
    fp = fopen("StrtDymk.xxx", "r");
    if (fp) {
        eraseAddress = fp->startAddress + fp->length;
        for ( ; eraseAddress < 0x800000 ; eraseAddress += 0x2000) {
            FLASHKey = 0xAA;
            EraseBlock();
        }
        fclose(fp);
    }
}    

void    EraseAll( void )
{
    for (eraseAddress = 10 ; eraseAddress < 0x800000 ; eraseAddress += 0x2000){
        FLASHKey = 0xAA;
        EraseBlock();
    }
}



void TypeA( void )
{
    xdata FILE*   fp;
    xdata long    i;
    xdata char    c;
    
    fp = fopen(commandLine[1], "r");
    if (fp) {
        for (i=0 ; i<fp->length ; i++) {
            c = fgetc(fp);
            if (c == 0xFF) {
                i = fp->length;
            } else {
                SendRS232(c);
            }
            WDT = 1;
            SWDT = 1;
        }
        fclose(fp);
    } else {
        SendStringEOLRS232("Unable to open file");
    }
}


char MemUsage( void )
//Retunerar 0 till 16 som andel av anv�nt minne, dvs 0 = inget minne anv�nt, 16 = minnet fullt
{
    xdata FILE_HEAD   fh;
    
    readAddress = 0;
        
    while (GetFileHead(&fh)) {
        readAddress = (readAddress & 0xFFFFFE00) + ((fh.length & 0xFFFFFE00) + 0x200);
    }
    
    return (readAddress >> 19) & 0xF;
}

    
    
