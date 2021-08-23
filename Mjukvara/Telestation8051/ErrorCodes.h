/***********************************************
Fil:            $Id: ErrorCodes.h,v 1.3 2003/11/20 22:08:52 Gemensam Exp $

Ändrad:         $Date: 2003/11/20 22:08:52 $

Revisionshistoria:
				$Log: ErrorCodes.h,v $
				Revision 1.3  2003/11/20 22:08:52  Gemensam
				Various declarations changes.
				Still version 0200.
				
				Revision 1.2  2003/11/10 22:44:45  Gemensam
				Added header information
				


***********************************************/

#ifndef	_ERRORCODES_INCLUDED_
#define _ERRORCODES_INCLUDED_


//Felkoder

//Generellt
#define EM_NOERROR          0x00



//Filsystem
#define EM_FILEEXISTS       0x10
#define EM_FILENOTEXISTS    0x11
#define EM_OUTOFFILEMEMORY  0x12
#define EM_TOOMANYFILES     0x13
#define EM_FILEOPEN         0x14
#define EM_FILENOTOPEN      0x15
#define EM_FILEREADONLY     0x16
#define EM_FILEWRITEONLY    0x17
#define EM_WRITEFILEOPEN    0x18

//Phone
#define EM_TOOMANYTIMERS    0x19

//BAtfiles
#define EM_TOOMANYBATFILES  0x20
#define EM_REDEFLABELATTEMPT    0x21
#define EM_TOOMANYLABELS    0x22
#define EM_NOLABEL          0x23


//PhoneAns
#define EM_PARAMETER_SYNTAX    0x24

#endif