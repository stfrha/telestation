/**************************************************************
Fil:            $Id: Commands.h,v 1.5 2003/11/24 22:44:35 Gemensam Exp $

Ändrad:         $Date: 2003/11/24 22:44:35 $

Revisionshistoria:
				$Log: Commands.h,v $
				Revision 1.5  2003/11/24 22:44:35  Gemensam
				Fixed PPD, not working yet,
				New command: speaker volume.
				Version 0301.
				
				Revision 1.4  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.3  2003/11/13 22:39:01  Gemensam
				Added watchdog reset at GotoLabel, FindLabel and InsertLabel.
				Added global variable to contain speaker volume, Default volume set to 50.
				Version 1.1
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				

***************************************************************/

#ifndef	_COMMANDS_INCLUDED_
#define _COMMANDS_INCLUDED_

//Module main
#define    CM_HELLO     "HELLO"
#define    CM_TESTHW    "TEST"
#define    CM_ID        "ID"
#define    CM_ERASE     "ERASE"

//Module Program
#define CM_PROGRAM      "PROGRAM"

//Module FileSystem
#define CM_READSPARE    "READSPARE"
#define CM_READSPARE2   "RS2"
#define CM_DIR          "DIR"
#define CM_WRITEFILE1   "WF1"
#define CM_WRITEFILE2   "WF2"
#define CM_WRITEFILE3   "WF3"
#define CM_READFILE1    "RF1"
#define CM_READFILE2    "RF2"
#define CM_READFILE3    "RF3"
#define CM_CREATELONG   "CL"
#define CM_DOWNLOAD     "DL" 
#define CM_UPLOAD       "UL" 
#define CM_TYPE         "TYPE" 
#define CM_TYPEA        "TYPEA" 
#define CM_CREATEDYNAMIC "CREATEDYNAMIC" 
#define CM_ERASEDYNAMIC "ERASEDYNAMIC" 
 


//Module WAV
#define CM_ECHO_MIC     "ECHOMIC"
#define CM_ECHO_PHONE   "ECHOPHONE"
#define CM_REC_MIC      "RECMIC"
#define CM_REC_PHONE    "RECPHONE"
#define CM_TONE         "TONE"
#define CM_PLAY         "PLAY" 
#define CM_REC_PORT     "RECPORT" 
 

//Module DTMF
#define CM_DTMF         "DTMF"
#define CM_WAIT_DTMF    "WAITDTMF"

//Module Phone
#define CM_HANG_UP      "PD"
#define CM_LIFT_PHONE   "PU"
#define CM_WAIT_RING    "WAITRING"
#define CM_READ_PB      "READPB"
#define CM_LOOKUP_PB    "LOOKUP"
 

//Module Timer
#define CM_WAIT_FOR     "WAITFOR"


//BAT-file handlning
#define CM_GOTO_LABEL   "GOTO"

//Calender funktioner
#define CM_READ_CALENDER_BYTE   "RC"
#define CM_WRITE_CALENDER_BYTE  "WC"
 
//PhoneAns funktioner
#define CM_RECMSG           "RECMSG"
#define CM_QUESTION         "QUESTION"
#define CM_WAITCALL         "WAITCALL"
#define CM_ANS_ON           "ANSON"
#define CM_ANS_OFF          "ANSOFF"
#define CM_CRE_LOG          "CREATELOG"
#define CM_ENTER_CODE       "ENTERCODE"
#define CM_PLAYNEW          "PLAYNEW"
#define CM_GOTO_CAT_EQUAL   "BRACATEQU"
#define CM_CATJMP           "CATJUMP"
#define CM_TESTCATJMP       "TCJ"
#define CM_PLAYMEMO         "PLAYMEMO"
#define CM_RECMEMO          "RECMEMO"
#define CM_SETSIGNALS       "SETSIGNALS"

//LCD funktioner
#define CM_LCD_SET      "SETLCD" 

//Dpot funktioner
#define CM_SETWIPER         "SETWIPER"
#define CM_SETDPREG         "SETDPREG"
#define CM_TRFTOWIPER       "TRFTOWIPER"
#define CM_READWIPER        "READWIPER"
#define CM_DATAH            "D1"
#define CM_DATAL            "D0"
#define CM_CLKH             "C1"
#define CM_CLKL             "C0"
#define CM_SPEAKVOL         "SPEAKVOL"
 
#endif