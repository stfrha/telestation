/*-------------------------------------------------------------------------
   _mululong.c - routine for multiplication of 32 bit unsigned long

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

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

unsigned long _mululong (unsigned long x, unsigned long y)
{
    unsigned long p = 0 ,s = 0 ;
    register unsigned short x0, i;
    
    i = 32;

 loop:
    x0 = (short)x & 1;
    x >>= 1;
    if (x0)
	s += y;
 
    p |= ((short)s & 1);

    s >>= 1;
    p = ( p >> 1) | ( p << 31);
    i--;
    if ( x ) goto loop;

    p |= s;
    while (i--)
	p = ( p >> 1) | ( p << 31);

    return p;
}
