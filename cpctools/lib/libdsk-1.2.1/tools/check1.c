/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001  John Elliott <jce@seasip.demon.co.uk>            *
 *                                                                         *
 *    This library is free software; you can redistribute it and/or        *
 *    modify it under the terms of the GNU Library General Public          *
 *    License as published by the Free Software Foundation; either         *
 *    version 2 of the License, or (at your option) any later version.     *
 *                                                                         *
 *    This library is distributed in the hope that it will be useful,      *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
 *    Library General Public License for more details.                     *
 *                                                                         *
 *    You should have received a copy of the GNU Library General Public    *
 *    License along with this library; if not, write to the Free           *
 *    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,      *
 *    MA 02111-1307, USA                                                   *
 *                                                                         *
 ***************************************************************************/


#include <stdio.h>
#include "libdsk.h"

int do_test(DSK_GEOMETRY *dg);

int main(int argc, char **argv)
{
	DSK_GEOMETRY dg;

	dg.dg_sidedness = SIDES_ALT;	
	dg.dg_cylinders = 80;
	dg.dg_heads     = 2;
	dg.dg_sectors   = 9;
	dg.dg_secbase   = 1;
	dg.dg_secsize   = 512;

	if (do_test(&dg)) return 1;

	return 0;
}



int do_test(DSK_GEOMETRY *dg)
{
	dsk_pcyl_t c, c1;
	dsk_phead_t h, h1;
	dsk_psect_t s, s1;
	dsk_lsect_t l;
	dsk_err_t e;

	for (c = 0; c < dg->dg_cylinders; c++)
		for (h = 0; h < dg->dg_heads; h++)
			for (s = dg->dg_secbase; s < dg->dg_sectors + dg->dg_secbase; s++)
		{
			e = dg_ps2ls(dg, c, h, s, &l);
			if (!e) e = dg_ls2ps(dg, l, &c1, &h1, &s1);
			if (e)
			{
				fprintf(stderr, "%s\n", dsk_strerror(e));
				return 1;
			}

			if (c != c1 || h != h1 || s != s1) 
			{
				fprintf(stderr, "%03d %d %d : %04ld : %03d %d %d\n",c,h,s,l,c1,h1,s1);
				return 1;
			}
		}
	return 0;
}

