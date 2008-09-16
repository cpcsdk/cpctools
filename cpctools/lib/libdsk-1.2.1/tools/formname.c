/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001-2  John Elliott <jce@seasip.demon.co.uk>          *
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

/* Query the library for supported formats, and select one */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "libdsk.h"
#include "utilopts.h"

void valid_formats(void)
{
	dsk_format_t format;
	dsk_cchar_t fname, fdesc;

	fprintf(stderr, "\nValid formats are:\n");

	format = FMT_180K;
	while (dg_stdformat(NULL, format++, &fname, &fdesc) == DSK_ERR_OK)
	{
		fprintf(stderr, "   %-10.10s : %s\n", fname, fdesc);
	}
}

dsk_format_t check_format(char *arg, int *argc, char **argv)
{
	int fmt;
	int n = find_arg(arg, *argc, argv);
	dsk_cchar_t fname;
	char *argname;

	if (n < 0) return -1;
	excise_arg(n, argc, argv);
	if (n >= *argc) 
	{
		fprintf(stderr, "Syntax error: use '%s <format>'\n", arg);
		exit(1);
	}
	argname = argv[n];
	excise_arg(n, argc, argv);
	fmt = FMT_180K;
	while (dg_stdformat(NULL, fmt, &fname, NULL) == DSK_ERR_OK)
	{
		if (!strcmpi(argname, fname)) return fmt;
		++fmt;
	}
	fprintf(stderr, "Format name %s not recognised.\n", argname);
	exit(1);
	return FMT_180K;
}


