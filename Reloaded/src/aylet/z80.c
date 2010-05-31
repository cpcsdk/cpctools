/* Emulation of the Z80 CPU with hooks into the other parts of aylet.
 * Copyright (C) 1994 Ian Collier. aylet changes (C) 2001-2002 Russell Marks.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "main.h"
#include "z80.h"

#define parity(a) (partable[a])

unsigned char partable[256]={
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0,
      4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4
   };


void z80loop(unsigned char *data,unsigned char *stacketc)
{
unsigned char a, f, b, c, d, e, h, l;
unsigned char r, a1, f1, b1, c1, d1, e1, h1, l1, i, iff1, iff2, im;
unsigned short pc;
unsigned short ix, iy, sp;
unsigned int radjust;
unsigned int ixoriy, new_ixoriy;
unsigned int intsample;
unsigned char op;
int interrupted=0;

a=f=b=c=d=e=h=l=a1=f1=b1=c1=d1=e1=h1=l1=i=r=iff1=iff2=im=0;
ixoriy=new_ixoriy=0;
ix=iy=sp=pc=0;
tstates=0;
radjust=0;

a=a1=b=b1=d=d1=h=h1=data[8];
f=f1=c=c1=e=e1=l=l1=data[9];
ix=iy=hl;

sp=stacketc[0]*256+stacketc[1];

while(1)
  {
  if(tstates>=tsmax)
    {
    do
      {
      if(!do_interrupt())
        {
        /* they want to exit */
        return;
        }
      }
    while(playing && paused);	/* when it's paused, we'd better wait here */
    
    tstates-=tsmax;
    interrupted=1;
    }
  
  ixoriy=new_ixoriy;
  new_ixoriy=0;
  intsample=1;
  op=fetch(pc);
  pc++;
  radjust++;
  switch(op)
    {
#include "z80ops.c"
    }
  
  if(interrupted && intsample && iff1)
    {
    interrupted=0;
    if(fetch(pc)==0x76)pc++;
    iff1=iff2=0;
    tstates+=5; /* accompanied by an input from the data bus */
    switch(im)
      {
      case 0: /* IM 0 */
      case 1: /* undocumented */
      case 2: /* IM 1 */
        /* there is little to distinguish between these cases */
        tstates+=7; /* perhaps */
        push2(pc);
        pc=0x38;
        break;
      case 3: /* IM 2 */
        tstates+=13; /* perhaps */
        {
        int addr=fetch2((i<<8)|0xff);
        push2(pc);
        pc=addr;
        }
      }
    }
  }
}
