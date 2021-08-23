echo off

rem *******************************************************************
rem File:			$Id: Make.bat,v 1.5 2003/12/26 16:17:01 Gemensam Exp $
rem
rem Description:	Make file for telestation8051
rem
rem Latest check in:$Date: 2003/12/26 16:17:01 $
rem
rem $Log: Make.bat,v $
rem Revision 1.5  2003/12/26 16:17:01  Gemensam
rem Reinstate 500 ms wait for hang up detection to settle (if not PPD is used).
rem Fixed PPD detection by decreasing phone volume from 63 to 15.
rem Version 0304.
rem
rem Revision 1.4  2003/11/12 23:19:21  Gemensam
rem All compile controlling files now produce exactly the same output as IAR project.
rem
rem Revision 1.3  2003/11/12 22:16:48  Gemensam
rem Created with IAR project as model.
rem
rem Revision 1.2  2003/11/12 20:27:05  Gemensam
rem Fixed header
rem
rem Revision 1.1  2003/11/12 20:26:10  Gemensam
rem First checkin
rem
rem
rem *******************************************************************

echo on
rem --------------------
rem Compiling files, please wait...
echo off
icc8051 Calender.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 DPot.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 DTMF.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 FileSystem.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 Keyboard.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 LCD.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 LogFile.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 main.c	-S -ml -v0 -e -K -u -s9 -RHIGHCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 Phone.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    -D__PPD_ON__=1 
rem -D__DEBUG_PHONE__=1
icc8051 PhoneAns.c	-S -ml -v0 -e -K -u -s9 -RHIGHCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 rs232.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 Timer.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x    
icc8051 wav.c	-S -ml -v0 -e -K -u -s9 -RCODE -gA -IC:\IAR\EW\8051\inc\ -ORelease\Obj\ -LRelease\List\ -q -i -t8 -x   -D__WAW_DEBUG__=1
echo on
rem --------------------
rem Linking, please wait...
echo off
XLINK -f Tele.xcl		-S
