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

int do_test(void);

int main(int argc, char **argv)
{
	if (do_test()) return 1;

	return 0;
}



int do_test(void)
{
	DSK_GEOMETRY dg;
	DSK_PDRIVER dr;
	dsk_err_t e;
	char buf[20000];

	/* e = dsk_open(&dr, "/dev/fd0H720", "raw");  */
	e = dsk_open(&dr, "/dev/fd0", NULL, NULL);
	if (!e) e = dsk_getgeom(dr, &dg);
	if (!e) e = dsk_ptread(dr, &dg, buf, 0, 1);
	if (!e) e = dsk_close(&dr);
	if (e) 
	{
		fprintf(stderr, "%s\n", dsk_strerror(e));
		return 1;
	}
	else
	{
		int n;
		for (n = 0; n < 20000; n++)
		{
			if (buf[n] >= ' ' && buf[n] <= '~') putchar(buf[n]);
			else putchar('.');
			if ((n % 32) == 31) putchar('\n');
		}
	}
	return 0;
}

