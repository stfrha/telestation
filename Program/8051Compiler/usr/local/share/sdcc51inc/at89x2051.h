
/*-------------------------------------------------------------------------
  Register Declarations for ATMEL 89c2051 Processors
  
   Written By - Bernd Bartmann 
    Bernd.Bartmann@picard.isdn.cs.tu-berlin.de (1999)
    based on reg51.h by Sandeep Dutta sandeep.dutta@usa.net
    KEIL C compatible definitions are included
    
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#ifndef AT892051_H
#define AT892051_H

/* BYTE addressable registers */
sfr at 0x81 SP          ;
sfr at 0x82 DPL         ;
sfr at 0x83 DPH         ;
sfr at 0x87 PCON        ;
sfr at 0x88 TCON        ;
sfr at 0x89 TMOD        ;
sfr at 0x8A TL0         ;
sfr at 0x8B TL1         ;
sfr at 0x8C TH0         ;
sfr at 0x8D TH1         ;
sfr at 0x90 P1          ;
sfr at 0x98 SCON        ;
sfr at 0x99 SBUF        ;
sfr at 0xA8 IE          ;
sfr at 0xB0 P3          ;
sfr at 0xB8 IP          ;
sfr at 0xD0 PSW         ;
sfr at 0xE0 ACC         ;
sfr at 0xE0 A           ;
sfr at 0xF0 B           ;


/* BIT addressable registers */
/* TCON */
sbit at 0x88 IT0        ;
sbit at 0x89 IE0        ;
sbit at 0x8A IT1        ;
sbit at 0x8B IE1        ;
sbit at 0x8C TR0        ;
sbit at 0x8D TF0        ;
sbit at 0x8E TR1        ;
sbit at 0x8F TF1        ;

/* P1 */
sbit at 0x90 P1_0       ;
sbit at 0x91 P1_1       ;
sbit at 0x92 P1_2       ;
sbit at 0x93 P1_3       ;
sbit at 0x94 P1_4       ;
sbit at 0x95 P1_5       ;
sbit at 0x96 P1_6       ;
sbit at 0x97 P1_7       ;

sbit at 0x90 AIN0       ;
sbit at 0x91 AIN1       ;

/* SCON */
sbit at 0x98 RI         ;
sbit at 0x99 TI         ;
sbit at 0x9A RB8        ;
sbit at 0x9B TB8        ;
sbit at 0x9C REN        ;
sbit at 0x9D SM2        ;
sbit at 0x9E SM1        ;
sbit at 0x9F SM0        ;

/* IE */
sbit at 0xA8 EX0        ;
sbit at 0xA9 ET0        ;
sbit at 0xAA EX1        ;
sbit at 0xAB ET1        ;
sbit at 0xAC ES         ;
sbit at 0xAD ET2        ;
sbit at 0xAF EA         ;

/* P3 */
sbit at 0xB0 P3_0       ;
sbit at 0xB1 P3_1       ;
sbit at 0xB2 P3_2       ;
sbit at 0xB3 P3_3       ;
sbit at 0xB4 P3_4       ;
sbit at 0xB5 P3_5       ;
sbit at 0xB7 P3_7       ;

sbit at 0xB0 RXD        ;
sbit at 0xB1 TXD        ;
sbit at 0xB2 INT0       ;
sbit at 0xB3 INT1       ;
sbit at 0xB4 T0         ;
sbit at 0xB5 T1         ;
sbit at 0xB6 AOUT       ;

/* IP */ 
sbit at 0xB8 PX0        ;
sbit at 0xB9 PT0        ;
sbit at 0xBA PX1        ;
sbit at 0xBB PT1        ;
sbit at 0xBC PS         ;

/* PSW */
sbit at 0xD0 P          ;
sbit at 0xD1 FL         ;
sbit at 0xD2 OV         ;
sbit at 0xD3 RS0        ;
sbit at 0xD4 RS1        ;
sbit at 0xD5 F0         ;
sbit at 0xD6 AC         ;
sbit at 0xD7 CY         ;


/* BIT definitions for bits that are not directly accessible */
/* PCON bits */
#define IDL             0x01
#define STOP            0x02
#define EWT             0x04
#define EPFW            0x08
#define WTR             0x10
#define PFW             0x20
#define POR             0x40
#define SMOD            0x80

#define IDL_            0x01
#define STOP_           0x02
#define EWT_            0x04
#define EPFW_           0x08
#define WTR_            0x10
#define PFW_            0x20
#define POR_            0x40
#define SMOD_           0x80

/* TMOD bits */
#define M0_0            0x01
#define M1_0            0x02
#define C_T0            0x04
#define GATE0           0x08
#define M0_1            0x10
#define M1_1            0x20
#define C_T1            0x40
#define GATE1           0x80

#define M0_0_           0x01
#define M1_0_           0x02
#define C_T0_           0x04
#define GATE0_          0x08
#define M0_1_           0x10
#define M1_1_           0x20
#define C_T1_           0x40
#define GATE1_          0x80

#define T0_M0           0x01
#define T0_M1           0x02
#define T0_CT           0x04
#define T0_GATE         0x08
#define T1_M0           0x10
#define T1_M1           0x20
#define T1_CT           0x40
#define T1_GATE         0x80

#define T0_M0_          0x01
#define T0_M1_          0x02
#define T0_CT_          0x04
#define T0_GATE_        0x08
#define T1_M0_          0x10
#define T1_M1_          0x20
#define T1_CT_          0x40
#define T1_GATE_        0x80

#define T0_MASK         0x0F
#define T1_MASK         0xF0

#define T0_MASK_        0x0F
#define T1_MASK_        0xF0


/* Interrupt numbers: address = (number * 8) + 3 */
#define IE0_VECTOR      0       /* 0x03 external interrupt 0 */
#define TF0_VECTOR      1       /* 0x0b timer 0 */
#define IE1_VECTOR      2       /* 0x13 external interrupt 1 */
#define TF1_VECTOR      3       /* 0x1b timer 1 */
#define SI0_VECTOR      4       /* 0x23 serial port 0 */
 
#endif


