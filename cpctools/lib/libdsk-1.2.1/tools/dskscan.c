/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2004  John Elliott <jce@seasip.demon.co.uk>            *
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

/* DSKSCAN is meant to read a diskette 'blind' - something like the scan mode
 * of ANADISK. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdsk.h"
#include "utilopts.h"
#include "formname.h"
#include <errno.h>

#ifdef CPM
#define AV0 "DSKSCAN"
#else
#define AV0 argv[0]
#endif

static dsk_format_t format = -1;
static	char *intyp = NULL;
static	char *incomp = NULL;
static	int inside = -1;
static  int idstep =  0;
static  int retries = 1;
static  int xml = 0;
static int first = -1, last = -1;

int do_scan(char *infile);
int scan_cyl(DSK_PDRIVER indr, DSK_GEOMETRY *dg, 
	     dsk_pcyl_t cyl, dsk_phead_t head);

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
                       "      %s {options} image\n",
			AV0);
	fprintf(stderr,"\nOptions are:\n"
		       "-type <type>   type of input disc image\n"
                       "-side <side>   Force side 0 or side 1 of input\n"
		       "-retry <count> Set number of retries on error\n"
		       "-first <cyl>   Start at specified cylinder\n"
		       "-last <cyl>    Scan up to specified cylinder\n"
		       "-dstep         Double-step\n"
		       "-xml           Output as XML\n"
		       "-format        Force a specified format name\n");
	fprintf(stderr,"\nDefault type is autodetect.\n\n");
		
	fprintf(stderr, "eg: %s /dev/fd0\n"
                        "    %s -xml /dev/fd1\n",
			AV0, AV0);
	valid_formats();
	return 1;
}


int main(int argc, char **argv)
{

	if (find_arg("--version", argc, argv) > 0) return version(); 
	if (argc < 2) return help(argc, argv);
	if (find_arg("--help",    argc, argv) > 0) return help(argc, argv);

	ignore_arg("-itype", 2, &argc, argv);
	ignore_arg("-iside", 2, &argc, argv);
	ignore_arg("-idstep", 2, &argc, argv);
	ignore_arg("-icomp", 2, &argc, argv);
	ignore_arg("-otype", 2, &argc, argv);
	ignore_arg("-oside", 2, &argc, argv);
	ignore_arg("-oostep", 2, &argc, argv);
	ignore_arg("-ocomp", 2, &argc, argv);

	intyp     = check_type("-type", &argc, argv); 
	incomp    = check_type("-comp", &argc, argv); 
        inside    = check_forcehead("-side", &argc, argv);
	retries   = check_retry("-retry", &argc, argv);
	if (present_arg("-dstep", &argc, argv)) idstep = 1;
	if (present_arg("-xml", &argc, argv)) xml = 1;
        format    = check_format("-format", &argc, argv);
	first     = check_numeric("-first", &argc, argv);
	last      = check_numeric("-last", &argc, argv);
 	args_complete(&argc, argv);
	return do_scan(argv[1]);
}



static char *op = "Opening";


int do_scan(char *infile)
{
	DSK_PDRIVER indr = NULL;
	dsk_err_t e;
	dsk_pcyl_t cyl;
	dsk_phead_t head;
	char *cmt = NULL;
	DSK_GEOMETRY dg;
	dsk_pcyl_t maxcyl;
	dsk_phead_t maxhead = 2;

        dsk_reportfunc_set(report, report_end);

	        e = dsk_open (&indr,  infile,  intyp, incomp);
	if (!e) e = dsk_set_retry(indr, retries);
	if (!e && inside >= 0) e = dsk_set_option(indr, "HEAD", inside);
	if (!e && idstep) e = dsk_set_option(indr, "DOUBLESTEP", 1);

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
		if (xml)
		{
			printf("<?xml version=\"1.0\" ?>\n");
			printf("<disc>\n");
		}
		dsk_get_comment(indr, &cmt);
		if (cmt)
		{
			if (xml) printf("  <comment><![CDATA[%s]]></comment>\n", cmt); 
			else	printf("Comment: %s\n", cmt);
		}
		if (first < 0) first = 0;
		if (last < 0)  last = maxcyl - 1;
		for (cyl = first; cyl <= (dsk_pcyl_t)last; cyl++)
		{
			for (head = 0; head < maxhead; head++)
			{
				if (xml) printf("  <track cylinder=\"%d\" head=\"%d\">\n", cyl, head);
				else	 printf("Cylinder %2d Head %d:\n",
						cyl, head);
				e = scan_cyl(indr, &dg, cyl, head);
				if (xml) printf("  </track>\n");
				if (e) goto abort;
			}
		}
abort:
		if (xml) printf("</disc>\n");
	}
	if (indr)  dsk_close(&indr);
	if (e)
	{
		fprintf(stderr, "\n%s: %s\n", op, dsk_strerror(e));
		return 1;
	}
	return 0;
}


/* Number of times a specified sector can be seen before we are certain that the
 * disc has made a complete rotation and nothing has been missed. */
#define PASSES 2

int scan_cyl(DSK_PDRIVER indr, DSK_GEOMETRY *dg, 
	     dsk_pcyl_t cyl, dsk_phead_t head)
{
	dsk_err_t err = DSK_ERR_OK;
	DSK_FORMAT sector_id;
	DSK_FORMAT *fmt_track;
	dsk_psect_t sector_count;
	dsk_psect_t secnum;

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
		if (xml) printf("    <!-- Found nothing -->\n");
		else	 printf("    Found nothing\n");
		return DSK_ERR_OK;
	}
	if (xml) printf("    <datarate>"); else printf("    Data rate: ");
	switch(dg->dg_datarate)
	{
		case RATE_HD: printf("500"); break;
		case RATE_DD: printf("250"); break;
		case RATE_SD: printf("300"); break;
		case RATE_ED: printf("1000"); break;
	}
	if (xml) printf("</datarate>\n"); else printf("\n");
	if (xml) printf("    <encoding>"); 
	else	 printf("    Encoding: ");
	printf("%s", dg->dg_fm ? "fm" : "mfm");
	if (xml) printf("</encoding\n");
	else printf("\n");
/* Now scour the track and print any sector headers we find */
	err = dsk_ptrackids(indr, dg, cyl, head, &sector_count, &fmt_track);
	if (!err)
	{
		for (secnum = 0; secnum < sector_count; secnum++)
		{
			sector_id = fmt_track[secnum];
			if (xml)
			{ 
				printf("    <sector>\n"); 
				printf("      <cyl>%d</cyl>",   sector_id.fmt_cylinder); 
				if (sector_id.fmt_cylinder != cyl) 
					printf(" <!-- Does not match location on disc -->"); 
				printf("\n");
				printf("      <hd>%d</hd>",     sector_id.fmt_head);
				if (sector_id.fmt_head != head)
					printf(" <!-- Does not match location on disc -->");
				printf("\n"); 
				printf("      <sec>%d</sec>\n", sector_id.fmt_sector);
				printf("      <size>%d</size>\n", (int)sector_id.fmt_secsize);
				printf("    </sector>\n"); 
			}
			else 
			{
				printf("    Cyl %02d%s Head %d%s Sec %3d "
					    "size %4d\n",
					sector_id.fmt_cylinder,
			(cyl != sector_id.fmt_cylinder ? "<!>" : "   "),
			       		sector_id.fmt_head,
			(head != sector_id.fmt_head ? "<!>" : "   "),
		 			sector_id.fmt_sector,
					(int)sector_id.fmt_secsize);			
			}
		}
		dsk_free(fmt_track);
	}
	return DSK_ERR_OK;
}

