/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001,2005  John Elliott <jce@seasip.demon.co.uk>       *
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

/* DSKDUMP is meant to image a diskette 'blind' - something like the dump mode
 * of ANADISK. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdsk.h"
#include "utilopts.h"
#include "formname.h"
#include <errno.h>

#ifdef CPM
#define AV0 "DSKDUMP"
#else
#define AV0 argv[0]
#endif

static dsk_format_t format = -1;
static	char *intyp = NULL, *outtyp = NULL;
static	char *incomp = NULL, *outcomp = NULL;
static	int inside = -1, outside = -1;
static  int idstep =  0, odstep  =  0;
static  int retries = 1;
static	dsk_pcyl_t maxcyl;
static	dsk_phead_t maxhead = 2;
static  int first = -1, last = -1;

int do_copy(char *infile, char *outfile);
int dump_cyl(DSK_PDRIVER indr, DSK_GEOMETRY *dg, 
	     DSK_PDRIVER outdr, dsk_pcyl_t cyl, dsk_phead_t head);

int check_numeric(char *arg, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);
	unsigned nr;

	if (n < 0) return -1;	
	excise_arg(n, argc, argv);
	if (n >= *argc || atoi(argv[n]) == 0)
	{
		fprintf(stderr, "Syntax error: use '%s nnn' where nnn is nonzero\n", arg);
		exit(1);
	}
	nr = atoi(argv[n]);
	excise_arg(n, argc, argv);

	return nr;
}

static void report(const char *s)
{
        fprintf(stderr, "%s\r", s);
        fflush(stderr);
}

static void report_end(void)
{
        fprintf(stderr, "\r%-79.79s\r", "");
        fflush(stderr);
}


int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                       "      %s {options} in-image { out-image }\n",
			AV0);
	fprintf(stderr,"\nOptions are:\n"
		       "-itype <type>   type of input disc image\n"
                       "-otype <type>   type of output disc image\n"
                       "-iside <side>   Force side 0 or side 1 of input\n"
                       "-oside <side>   Force side 0 or side 1 of output\n"
		       "-retry <count>  Set number of retries on error\n"
		       "-idstep         Double-step when reading\n"
		       "-odstep         Double-step when writing\n"
		       "-format         Force a specified format name\n");
	fprintf(stderr,"\nDefault in-image type is autodetect."
		               "\nDefault out-image type is DSK.\n\n");
		
	fprintf(stderr, "eg: %s /dev/fd0 myfile1.DSK\n"
                        "    %s -side 1 /dev/fd0 myfile2.DSK\n" 
                        "    %s -md3 /dev/fd0 md3boot.dsk\n" 
                        "    %s -otype floppy myfile.DSK /dev/fd0\n", 
			AV0, AV0, AV0, AV0);
	valid_formats();
	return 1;
}


int main(int argc, char **argv)
{

	if (find_arg("--version", argc, argv) > 0) return version(); 
	if (argc < 2) return help(argc, argv);
	if (find_arg("--help",    argc, argv) > 0) return help(argc, argv);

	ignore_arg("-type", 2, &argc, argv);
	ignore_arg("-side", 2, &argc, argv);
	ignore_arg("-dstep", 2, &argc, argv);
	ignore_arg("-comp", 2, &argc, argv);

	intyp     = check_type("-itype", &argc, argv); 
        outtyp    = check_type("-otype", &argc, argv);
	incomp    = check_type("-icomp", &argc, argv); 
        outcomp   = check_type("-ocomp", &argc, argv);
        inside    = check_forcehead("-iside", &argc, argv);
        outside   = check_forcehead("-oside", &argc, argv);
	retries   = check_retry("-retry", &argc, argv);
	if (present_arg("-idstep", &argc, argv)) idstep = 1;
	if (present_arg("-odstep", &argc, argv)) odstep = 1;
	if (!outtyp) outtyp = "dsk";
        format    = check_format("-format", &argc, argv);
        first     = check_numeric("-first", &argc, argv);
        last      = check_numeric("-last", &argc, argv);
	args_complete(&argc, argv);
	if (argc < 3) return help(argc, argv);
	return do_copy(argv[1], argv[2]);
}



static char *op = "Opening";


int do_copy(char *infile, char *outfile)
{
	DSK_PDRIVER indr = NULL, outdr = NULL;
	dsk_err_t e;
	dsk_pcyl_t cyl;
	dsk_phead_t head;
	char *cmt = NULL;
	DSK_GEOMETRY dg;

        dsk_reportfunc_set(report, report_end);

	        e = dsk_open (&indr,  infile,  intyp, incomp);
	if (!e) e = dsk_set_retry(indr, retries);
	if (!e && inside >= 0) e = dsk_set_option(indr, "HEAD", inside);
	if (!e && idstep) e = dsk_set_option(indr, "DOUBLESTEP", 1);

	if (!e) e = dsk_creat(&outdr, outfile, outtyp, outcomp);
	if (!e && outside >= 0) e = dsk_set_option(outdr, "HEAD", outside);
	if (!e && odstep) e = dsk_set_option(outdr, "DOUBLESTEP", 1);
	if (!e) e = dsk_set_retry(outdr, retries); 

/* Try to guess geometry. If this fails the user has to specify an approximate
 * geometry manually. We need to do this to get a feel for the number of 
 * cylinders to scan; dsk_psecid() will fail if (eg) it tries to select a
 * 1.4Mb geometry on a 360k drive */
        if (format == -1)
        {
                op = "Identifying disc";
                if (!e) e = dsk_getgeom(indr, &dg);
        }
        else if (!e) e = dg_stdformat(&dg, format, NULL, NULL);

        op = "Scanning disc";
	if (!e)
	{
		maxcyl = dg.dg_cylinders + 4;
/* Head is being forced. Do only one side. */
		if (inside >= 0) maxhead = 1;
		dsk_get_comment(indr, &cmt);
		if (outdr) dsk_set_comment(outdr, cmt);
		if (first < 0) first = 0;
		if (last < 0)  last = maxcyl - 1;
		for (cyl = first; cyl <= (dsk_pcyl_t)last ; cyl++)
		{
			for (head = 0; head < maxhead; head++)
			{
				e = dump_cyl(indr, &dg, outdr, cyl, head);
				if (e) goto abort;
			}
		}
abort:		;
	}
	if (indr)  dsk_close(&indr);
	if (outdr) dsk_close(&outdr);
	printf("\r%-70.70s\n", "");
	if (e)
	{
		fprintf(stderr, "\n%s: %s\n", op, dsk_strerror(e));
		return 1;
	}
	return 0;
}



int dump_cyl(DSK_PDRIVER indr, DSK_GEOMETRY *dg, 
	     DSK_PDRIVER outdr, dsk_pcyl_t cyl, dsk_phead_t head)
{
	dsk_err_t err = DSK_ERR_OK;
	DSK_FORMAT sector_id;
	DSK_FORMAT *fmt_track;
	dsk_psect_t osec;
	dsk_psect_t sector_count;
	dsk_psect_t secnum;
	unsigned buflen = 128;

	dg->dg_noskip = 1;
	/* Guess data rate and recording mode */
	for (dg->dg_datarate = RATE_HD; dg->dg_datarate <= RATE_ED; ++dg->dg_datarate)
	{
		for (dg->dg_fm = 0; dg->dg_fm < 2; ++dg->dg_fm)
		{
			err = dsk_psecid(indr, dg, cyl, head, &sector_id);
			if (!err) break;	
		}	
		if (!err) break;
	}
	if (err)
	{
		return DSK_ERR_OK;
	}
/* Now scour the track and print any sector headers we find */
	err = dsk_ptrackids(indr, dg, cyl, head, &sector_count, &fmt_track);
	if (!err)
	{
		for (secnum = 0; secnum < sector_count; secnum++)
		{
			sector_id = fmt_track[secnum];
			if (sector_id.fmt_secsize > buflen)  
				buflen = sector_id.fmt_secsize; 
		}
	}
	if (outdr && sector_count)
	{
		unsigned char *buf = dsk_malloc(buflen);
		if (!buf) return DSK_ERR_NOMEM;

/* Format a track with the correct number and size of sectors */
		osec = dg->dg_sectors;
		dg->dg_sectors = sector_count;
		op = "Formatting";
		err = dsk_pformat(outdr, dg, cyl, head, fmt_track, 0xE5);
		dg->dg_sectors = osec;
		if (!err) for (secnum = 0; secnum < sector_count; secnum++)
		{
			int deleted = 0;
			op = "Reading";
			printf("Cylinder %02d/%2d Head %d/%d  "
				"Sector (%02d,%d,%03d)/%03d size %04d\r",
				cyl, last,
				head, maxhead-1,
				fmt_track[secnum].fmt_cylinder,
				fmt_track[secnum].fmt_head,
				fmt_track[secnum].fmt_sector,
				sector_count,
				(int)fmt_track[secnum].fmt_secsize);
			fflush(stdout);
			err = dsk_xread(indr, dg, buf, cyl, head,
					fmt_track[secnum].fmt_cylinder,
					fmt_track[secnum].fmt_head,
					fmt_track[secnum].fmt_sector,
					fmt_track[secnum].fmt_secsize, &deleted);
			/* XXX Need to be able to write sectors with errors,
			 * at least to CPCEMU DSK  */
			if (err == DSK_ERR_DATAERR) err = DSK_ERR_OK;
			if (err) break;
			op = "Writing";
			err = dsk_xwrite(outdr, dg, buf, cyl, head,
					fmt_track[secnum].fmt_cylinder,
					fmt_track[secnum].fmt_head,
					fmt_track[secnum].fmt_sector,
					fmt_track[secnum].fmt_secsize, deleted);
			if (err) break;	
		}
		dsk_free(buf);
		if (err) return err;
	}
	return DSK_ERR_OK;
}
