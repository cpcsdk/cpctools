/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2006  John Elliott <jce@seasip.demon.co.uk>            *
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
#include "utilopts.h"
#include "formname.h"
#include "apriboot.h"

#ifdef __MSDOS__
#define AV0 "APRIBOOT"
#else
#define AV0 argv[0]
#endif

static unsigned retries = 1;
static MODE md = SWAP;
static dsk_format_t format = -1;
static byte buffer[512];

int do_swapboot(int argc, char *outfile, char *outtyp, char *outcomp, 
		int forcehead);

int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                "      %s { options } dskimage { dskimage ... }\n\n"
		"Options are:\n"
		"      -pcdos:        Force PCDOS format\n"
	        "      -apricot:      Force Apricot format\n"
		"      -type <type>:  Type of disc image\n"
		"      -format <fmt>: Force a specified format name\n"
		"      -retry <nnn>:  Set number of retries on error\n"
		"      -side <side>:  Force side 0 or side 1\n", AV0);
	return 1;
}

static void report(const char *s)
{
	fprintf(stderr,"%s\r", s);
	fflush(stderr);
}

static void report_end(void)
{
	fprintf(stderr,"\r%-79.79s\r", "");
	fflush(stderr);
}

int main(int argc, char **argv)
{
	char *outtyp;
	char *outcomp;
	int forcehead;
	int n, err;

	if (argc < 2) return help(argc, argv);

        ignore_arg("-itype", 2, &argc, argv);
        ignore_arg("-iside", 2, &argc, argv);
        ignore_arg("-icomp", 2, &argc, argv);
        ignore_arg("-otype", 2, &argc, argv);
        ignore_arg("-oside", 2, &argc, argv);
        ignore_arg("-ocomp", 2, &argc, argv);

	outtyp    = check_type("-type", &argc, argv);
	outcomp   = check_type("-comp", &argc, argv);
	forcehead = check_forcehead("-side", &argc, argv);	
	retries   = check_retry("-retry", &argc, argv);
	format    = check_format("-format", &argc, argv);

        if (find_arg("--help",    argc, argv) > 0) return help(argc, argv);
        if (find_arg("--version", argc, argv) > 0) return version();
        if ((n = find_arg("--pcdos",   argc, argv) > 0)) 
	{
		md = FORCE_PCDOS;
		excise_arg(n, &argc, argv);
	}
        if ((n = find_arg("-pcdos",    argc, argv) > 0))
	{
		md = FORCE_PCDOS;
		excise_arg(n, &argc, argv);
	}
        if ((n = find_arg("--apricot", argc, argv) > 0))
	{
		md = FORCE_APRICOT;
		excise_arg(n, &argc, argv);
	}
        if ((n = find_arg("-apricot",  argc, argv) > 0))
	{
		md = FORCE_APRICOT;
		excise_arg(n, &argc, argv);
	}
	args_complete(&argc, argv);

	err = 0;
	for (n = 1; n < argc; n++)
	{
		if (do_swapboot(argc, argv[n], outtyp, outcomp, forcehead))
			++err;
	}
	return err;
}


int do_swapboot(int argc, char *outfile, char *outtyp, char *outcomp, 
		int forcehead)
{
	DSK_PDRIVER outdr = NULL;
	dsk_err_t e;
	DSK_GEOMETRY dg;

	dsk_reportfunc_set(report, report_end);	
	e = dsk_open(&outdr, outfile, outtyp, outcomp);
	if (!e) e = dsk_set_retry(outdr, retries);
	if (forcehead >= 0 && !e) e = dsk_set_forcehead(outdr, forcehead);
	if (!e) 
	{
		if (format == -1) e = dsk_getgeom(outdr, &dg);
		else		  e = dg_stdformat(&dg, format, NULL, NULL);
	}
	if (!e)
	{
		if (dg.dg_secsize != 512)
		{
			fprintf(stderr, "%s: Sector size is not 512 bytes",
					outfile);
			dsk_close(&outdr);
			return 1;
		}
		e = dsk_pread(outdr, &dg, buffer, 0, 0, 1);
	}
	if (!e)
	{
		if (transform(md, buffer, outfile, 1))
		{
			dsk_close(&outdr);
			return 1;
		}
		e = dsk_pwrite(outdr, &dg, buffer, 0, 0, 1);
	}
	if (outdr) dsk_close(&outdr);
	if (e)
	{
		fprintf(stderr, "%s: %s\n", outfile, dsk_strerror(e));
		return 1;
	}
	return 0;
}

