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

/* Simple wrapper around dsk_getgeom() */

#include <stdio.h>
#include "libdsk.h"
#include "utilopts.h"

#ifdef CPM
#define AV0 "DSKID"
#else
#define AV0 argv[0]
#endif

static unsigned retries = 1;

int do_login(int argc, char *outfile, char *outtyp, char *outcomp, int forcehead);

int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                "      %s { -type <type> } "
		"{ -retry <count> } { -side <side> } dskimage \n",
			AV0);
	fprintf(stderr,"\nDefault type is autodetect.\n\n");
		
	fprintf(stderr, "eg: %s myfile.DSK\n"
                        "    %s -type floppy -side 1 /dev/fd0\n", AV0, AV0);
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

        if (find_arg("--help",    argc, argv) > 0) return help(argc, argv);
        if (find_arg("--version", argc, argv) > 0) return version();
	args_complete(&argc, argv);

	err = 0;
	for (n = 1; n < argc; n++)
	{
		if (do_login(argc, argv[n], outtyp, outcomp, forcehead))
			++err;
	}
	return err;
}


int do_login(int argc, char *outfile, char *outtyp, char *outcomp, int forcehead)
{
	DSK_PDRIVER outdr = NULL;
	dsk_err_t e;
	DSK_GEOMETRY dg;
	unsigned char drv_status;
	const char *comp;
	char *comment;
	int indent = 0;

	dsk_reportfunc_set(report, report_end);	
	e = dsk_open(&outdr, outfile, outtyp, outcomp);
	if (!e) e = dsk_set_retry(outdr, retries);
	if (forcehead >= 0 && !e) e = dsk_set_forcehead(outdr, forcehead);
	if (!e) e = dsk_getgeom(outdr, &dg);
	if (!e)
	{
		if (argc > 1)
		{
			printf("%s:\n", outfile);
			indent = 2;
		}
                printf(          "%-*.*sDriver:      %s\n", 
				indent, indent, "", dsk_drvdesc(outdr));
		comp = dsk_compname(outdr);
                if (comp) 
		{
			printf("%-*.*sCompression: %s\n", 
				indent, indent, "", dsk_compdesc(outdr));
		}

		if (forcehead >= 0) 
		{
			printf("%-*.*s[Forced to read from side %d]\n", 
					indent, indent, "", forcehead);
		}

		printf("%-*.*sSidedness:     %2d\n"
                       "%-*.*sCylinders:     %2d\n"
		       "%-*.*sHeads:          %d\n"
                       "%-*.*sSectors:      %3d\n"
                       "%-*.*sFirst sector: %3d\n"
                       "%-*.*sSector size: %4ld\n"
		       "%-*.*sData rate:      %d\n"
		       "%-*.*sRecord mode:  %s\n"
		       "%-*.*sR/W gap:     0x%02x\n"
		       "%-*.*sFormat gap:  0x%02x\n",
			indent, indent, "", dg.dg_sidedness, 
			indent, indent, "", dg.dg_cylinders,
			indent, indent, "", dg.dg_heads, 
			indent, indent, "", dg.dg_sectors, 
			indent, indent, "", dg.dg_secbase,
			indent, indent, "", (long)dg.dg_secsize, 
			indent, indent, "", dg.dg_datarate,
			indent, indent, "", (dg.dg_fm ? " FM" : "MFM"), 
			indent, indent, "", dg.dg_rwgap,   
			indent, indent, "", dg.dg_fmtgap);
		e = dsk_drive_status(outdr, &dg, 0, &drv_status);
		if (!e)
		{	
			printf("\n%-*.*sDrive status:  0x%02x\n", 
				indent, indent, "", drv_status);
		}
		e = dsk_get_comment(outdr, &comment);
		if (!e && comment)  
		{
			char *c;
			printf("%-*.*sComment:       ", indent, indent, "");
			c = comment;
			while (*c)
			{
				putchar(*c);
				if (*c == '\n') printf("%-*.*s               ", 
					indent, indent, "");
				c++;
			}
		}	
		putchar('\n');
	}
	if (outdr) dsk_close(&outdr);
	if (e)
	{
		fprintf(stderr, "%s: %s\n", outfile, dsk_strerror(e));
		return 1;
	}
	return 0;
}

