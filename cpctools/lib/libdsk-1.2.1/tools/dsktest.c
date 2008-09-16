/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2005  John Elliott <jce@seasip.demon.co.uk>            *
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

/* Test program for a LibDsk driver. This attempts to ensure that all 
 * 12 methods of a driver are called. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdsk.h"
#include "utilopts.h"

static unsigned retries = 1;
static int failures = 0;
static int warnings = 0;
static int unimpl   = 0;
static int rpcunimp = 0;
static int t80      = 0;

int do_test(char *outfile, char *outtyp, char *outcomp, int forcehead);

int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                "      %s { -type <type> } "
		"{ -retry <count> } { -side <side> } { -80 } dskimage \n",
			argv[0]);
	fprintf(stderr,"\nDefault type is DSK.\n\n");
		
	fprintf(stderr, "eg: %s myfile.DSK\n"
                        "    %s -type floppy -side 1 /dev/fd0\n", argv[0], argv[0]);
	fprintf(stderr, " The -80 option sets up an 80-cylinder 360k "
			"geometry; this may be needed to\n"
			"avoid 'unsuitable media' errors on "
			"80-track drives.\n");

	return 1;
}

static void report(const char *s)
{
	printf("%s\r", s);
	fflush(stdout);
}

static void report_end(void)
{
	printf("\r%-79.79s\r", "");
	fflush(stdout);
}

int main(int argc, char **argv)
{
	char *outtyp;
	char *outcomp;
	int forcehead;
	int np;

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
	np = find_arg("-80", argc, argv);
	if (np > 0) 
	{
		t80 = 1;
		excise_arg(np, &argc, argv);
	}
	args_complete(&argc, argv);

	if (!outtyp) outtyp = "dsk";
	return do_test(argv[1], outtyp, outcomp, forcehead);
}

static unsigned char spec180 [10] = { 0,    0, 40, 9, 2, 1, 3, 2, 0x2A, 0x52 };

unsigned char secbuf[512];
unsigned char trkbuf[8192];	


#define CHECKERR(fn) if (e) \
{ fprintf(stderr, "%s failed: %s\n", fn, dsk_strerror(e)); \
  ++failures; stats(e); }

#define CHECKERR2(fn) if (e) \
{ fprintf(stderr, "%s failed: %s\n", fn, dsk_strerror(e)); \
  stats(e); if (e != DSK_ERR_BADOPT) ++failures; else ++warnings; }

#define CHECKERR3(fn) if (e) \
{ fprintf(stderr, "%s failed: %s\n", fn, dsk_strerror(e)); \
  stats(e); ++warnings; }

static void stats(dsk_err_t err)
{
	if (err == DSK_ERR_NOTIMPL) ++unimpl;
	if (err == DSK_ERR_UNKRPC)  ++rpcunimp;
}


int do_test(char *outfile, char *outtyp, char *outcomp, int forcehead)
{
	DSK_PDRIVER outdr = NULL;
	dsk_err_t e;
	DSK_GEOMETRY dg, dg2;
	static DSK_FORMAT fmt[9] =
		{
			{ 0, 0, 1, 512 },
			{ 0, 0, 3, 512 },
			{ 3, 1, 5, 512 },
			{ 0, 0, 7, 512 },
			{ 0, 0, 9, 512 },
			{ 0, 1, 2, 512 },
			{ 0, 0, 4, 512 },
			{ 2, 0, 6, 512 },
			{ 0, 0, 8, 512 },
		};
	static DSK_FORMAT fmt2[9] =
		{
			{ 4, 0, 1, 512 },
			{ 4, 0, 3, 512 },
			{ 4, 0, 5, 512 },
			{ 4, 0, 7, 512 },
			{ 4, 0, 9, 512 },
			{ 4, 0, 2, 512 },
			{ 4, 0, 4, 512 },
			{ 4, 0, 6, 512 },
			{ 4, 0, 8, 512 },
		};
	DSK_FORMAT secid, *pfmt;
	char *comment;
	unsigned char status;
	dsk_psect_t count, n;
	char *optname;
	int value;

	dsk_reportfunc_set(report, report_end);	
	dg_stdformat(&dg, FMT_180K, NULL, NULL);
	if (t80) dg.dg_cylinders = 80;

	printf("Checking dsk_creat\n");
	e = dsk_creat(&outdr, outfile, outtyp, outcomp);
	CHECKERR("dsk_creat")
	else
	{
		dsk_set_option(outdr, "REMOTE:TESTING", 1);
		printf("Checking set_comment\n");
		e = dsk_set_comment(outdr, "Example comment");
		CHECKERR3("dsk_set_comment");
		printf("Checking pformat\n");
		e = dsk_pformat(outdr, &dg, 0, 0, fmt, 0xE5);
		CHECKERR("dsk_pformat")
		printf("Checking apform\n");
		e = dsk_apform(outdr, &dg, 1, 0, 0xF6);
		CHECKERR("dsk_apform")
		printf("Checking lformat\n");
		e = dsk_lformat(outdr, &dg, 2, fmt2, 0xD4);
		CHECKERR("dsk_pformat")
		printf("Checking ptrackids\n");
		e = dsk_ptrackids(outdr, &dg, 0, 0, &count, &pfmt);
		CHECKERR("dsk_ptrackids")
		else
		{
			dsk_psect_t n;
			printf("Cyl Head Sec Size\n");
			printf("-----------------\n");
			for (n = 0; n < count; n++)
			{
				printf("%3d  %3d %3d %4d\n",
					pfmt[n].fmt_cylinder,
					pfmt[n].fmt_head,
					pfmt[n].fmt_sector,
					pfmt[n].fmt_secsize);
			}
			printf("-----------------\n");
			dsk_free(pfmt);
		}
		memset(secbuf,       0, sizeof(secbuf));
		memcpy(secbuf, spec180, sizeof(spec180));
		printf("Checking dsk_lwrite\n");
		e = dsk_lwrite(outdr, &dg, secbuf, 0);
		CHECKERR("dsk_lwrite")
		strcpy(secbuf, "Cyl=3 Head=1 Sec=5");
		printf("Checking dsk_xwrite\n");
		e = dsk_xwrite(outdr, &dg, secbuf, 0, 0, 3, 1, 5, 512, 0);
		CHECKERR("dsk_xwrite")
		printf("Checking dsk_psecid\n");
		e = dsk_psecid(outdr, &dg, 1, 0, &secid);
		CHECKERR("dsk_psecid")
		else
		{
			printf("%3d  %3d %3d %4d\n",
				secid.fmt_cylinder,
				secid.fmt_head,
				secid.fmt_sector,
				secid.fmt_secsize);
			if (secid.fmt_cylinder != 1 || secid.fmt_head != 0 ||
			    secid.fmt_secsize != 512) 
			{
				++failures;
				printf("-- mismatch!\n");
			}
		}
	}	
	if (outdr) 
	{
		dsk_close(&outdr);
		CHECKERR("dsk_close")
	}
	printf("Checking dsk_open\n");
	e = dsk_open(&outdr, outfile, outtyp, outcomp);
	CHECKERR("dsk_open")
	else
	{
		dsk_set_option(outdr, "REMOTE:TESTING", 1);
		printf("Checking dsk_getgeom\n");
		e = dsk_getgeom(outdr, &dg2);
		if (t80 && dg2.dg_cylinders == 40) dg2.dg_cylinders = 80;
		CHECKERR("dsk_getgeom")
		else if (memcmp(&dg, &dg2, sizeof(dg)))
		{
			++failures;
			printf("-- mismatch!\n");
			printf("Cyls:  %3d %3d\n", dg.dg_cylinders, dg2.dg_cylinders);
			printf("Heads: %3d %3d\n", dg.dg_heads, dg2.dg_heads);
			printf("Secs:  %3d %3d\n", dg.dg_sectors, dg2.dg_sectors);
		}
		printf("Checking dsk_lread\n");
		e = dsk_lread(outdr, &dg, secbuf, 0);
		CHECKERR("dsk_lread")
		else if (memcmp(secbuf, spec180, sizeof(spec180)))
		{
			++failures;
			printf("-- mismatch!\n");
		}
		printf("Checking dsk_xread\n");
		e = dsk_xread(outdr, &dg, secbuf, 0, 0, 3, 1, 5, 512, NULL);
		CHECKERR("dsk_xread")
		else if (strcmp(secbuf, "Cyl=3 Head=1 Sec=5"))
		{
			++failures;
			printf("-- mismatch!\n");
		}
		printf("Checking dsk_lseek\n");
		e = dsk_lseek(outdr, &dg, 1);
		CHECKERR("dsk_lseek")
		printf("Checking dsk_drive_status\n");
		e = dsk_drive_status(outdr, &dg, 0, &status);
		CHECKERR("dsk_drive_status")
		else printf("-- status=0x%02x\n", status);

		printf("Checking dsk_tread\n");
		e = dsk_ptread(outdr, &dg, trkbuf, 1, 0);
		CHECKERR("dsk_ptread")
		else
		{
			for (n = 0; n < 4608; n++) if (trkbuf[n] != 0xF6)
			{
				printf("-- mismatch!\n");
				++failures;
				break;
			}
	
		}
		printf("Checking dsk_xtread\n");
		e = dsk_xtread(outdr, &dg, trkbuf, 2, 0, 4, 0);
		CHECKERR("dsk_xtread")
		else
		{
			for (n = 0; n < 4608; n++) if (trkbuf[n] != 0xD4)
			{
				printf("-- mismatch!\n");
				++failures;
				break;
			}
		}
		printf("Checking dsk_option_enum\n");
		e = dsk_option_enum(outdr, 0, &optname);
		CHECKERR("dsk_option_enum")
		else
		{
			printf("-- first option is %s\n", optname);
		}
		if (!optname) optname = "dummy";
		printf("Checking dsk_get_option(%s)\n", optname);
		e = dsk_get_option(outdr, optname, &value);
		CHECKERR2("dsk_get_option")
		else
		{
			printf("-- value=%d\n", value);
		}
		printf("Checking dsk_set_option(example, 2)\n");
		e = dsk_set_option(outdr, "example", 2);
		CHECKERR2("dsk_set_option")
		printf("Checking dsk_rtread\n");
		e = dsk_rtread(outdr, &dg, trkbuf, 0, 0, 0);
		CHECKERR3("dsk_rtread")
		printf("Checking dsk_get_comment\n");
		e = dsk_get_comment(outdr, &comment);
		CHECKERR3("dsk_get_comment")
		else if (comment) printf("-- comment=%s\n", comment);
	}
	if (outdr) 
	{
		dsk_close(&outdr);
		CHECKERR("dsk_close")
	}
	printf("\n");
	printf("Failed calls (failure important):   %2d\n", failures);
	printf("Failed calls (failure unimportant): %2d\n", warnings);
	printf("Unimplemented functions:            %2d\n", unimpl);
	printf("Unimplemented functions over RPC:   %2d\n", rpcunimp);
	return failures;
}


/* 

ok	dsk_err_t (*dc_open )(DSK_DRIVER *self, const char *filename);
ok	dsk_err_t (*dc_creat)(DSK_DRIVER *self, const char *filename);
ok	dsk_err_t (*dc_close)(DSK_DRIVER *self);
ok	dsk_err_t (*dc_read)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
ok	dsk_err_t (*dc_write)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
ok	dsk_err_t (*dc_format)(DSK_DRIVER *self, DSK_GEOMETRY *geom,
ok	dsk_err_t (*dc_getgeom)(DSK_DRIVER *self, DSK_GEOMETRY *geom);
ok	dsk_err_t (*dc_secid)(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
ok	dsk_err_t (*dc_xseek)(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
ok	dsk_err_t (*dc_status)(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
ok	dsk_err_t (*dc_xread)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
ok	dsk_err_t (*dc_xwrite)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
	dsk_err_t (*dc_tread)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
	dsk_err_t (*dc_xtread)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
	dsk_err_t (*dc_option_enum)(DSK_DRIVER *self, int idx, char **optname);
	dsk_err_t (*dc_set_option)(DSK_DRIVER *self, const char *optname, int value);
	dsk_err_t (*dc_get_option)(DSK_DRIVER *self, const char *optname, int *value);
ok	dsk_err_t (*dc_trackids)(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
	dsk_err_t (*dc_rtread)(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
*/
