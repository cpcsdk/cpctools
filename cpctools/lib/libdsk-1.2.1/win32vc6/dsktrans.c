/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001-2,2005  John Elliott <jce@seasip.demon.co.uk>     *
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

/* Portable equivalent of PCWTRANS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "libdsk.h"
#include "utilopts.h"
#include "formname.h"

#ifdef CPM
#define AV0 "DSKTRANS"
#else
#define AV0 argv[0]
#endif

#define DSKTRANS_DEBUG 0

static int md3 = 0;
static int apricot = 0;
static int logical = 0;
static int noformat = 0;
static dsk_format_t format = -1;
static	char *intyp = NULL, *outtyp = NULL;
static	char *incomp = NULL, *outcomp = NULL;
static	int inside = -1, outside = -1;
static  int idstep =  0, odstep  =  0;
static  int retries = 1;
static int first = -1, last = -1;

int do_copy(char *infile, char *outfile);

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
        printf("%s\r", s);
        fflush(stdout);
}

static void report_end(void)
{
        printf("\r%-79.79s\r", "");
        fflush(stdout);
}


int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                       "      %s {options} in-image out-image\n",
			AV0);
	fprintf(stderr,"\nOptions are:\n"
		       "-itype <type>   type of input disc image\n"
                       "-otype <type>   type of output disc image\n"
                       "-iside <side>   Force side 0 or side 1 of input\n"
                       "-oside <side>   Force side 0 or side 1 of output\n"
                       "-first <cyl>    Copy starting from specified cylinder\n"
                       "-last <cyl>     Copy up to and including specified cylinder\n"
		       "-retry <count>  Set number of retries on error\n"
		       "-idstep         Double-step when reading\n"
		       "-odstep         Double-step when writing\n"
                       "-noformat       Do not format destination disc\n"
                       "-md3            Defeat MicroDesign 3 copy protection\n"
                       "-apricot        Convert Apricot superblock to PC-DOS format\n"
		       "-logical        Rearrange tracks in logical order\n"
                       "                (Only useful when out-image type is 'raw' and reading discs\n"
		       "                with non-IBM track layout (eg: 144FEAT 1.4Mb or ADFS 640k)\n"
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


/* A quick-and-dirty converter which replaces an Apricot MS-DOS 
 * boot block with a standard PCDOS one. Intended use is for 
 * loopback mounting:
 *
 * dsktrans -otype raw -apricot /dev/fd0 filename.ufi
 * mount -o loop -t msdos filename.ufi /mnt/floppy
 */

void apricot_bootsect(unsigned char *bootsect) 
{
	int n;
	int heads     = bootsect[0x16];
	int sectors   = bootsect[0x10] + 256 * bootsect[0x11];
	char label[8];

/* Check that the first 8 bytes are ASCII or all zeroes. */
	for (n = 0; n < 8; n++)
	{
		if (bootsect[n] != 0 && 
			(bootsect[n] < 0x20 || bootsect[n] > 0x7E)) return;
	}
	memcpy(label, bootsect, 8);
	bootsect[0] = 0xE9;	/* 80x86 jump */
	bootsect[1] = 0x40;
	bootsect[2] = 0x90;
	memcpy(bootsect + 3, label, 8);	/* OEM ID */
	memcpy(bootsect + 11, bootsect + 80, 13);	/* BPB */
	bootsect[24] = sectors & 0xFF;
	bootsect[25] = sectors >> 8;
	bootsect[26] = heads & 0xFF;
	bootsect[27] = heads >> 8;
	memset(bootsect + 28, 0, 512 - 28);
	bootsect[0x40] = 0x90;	/* Minimal boot code: INT 18, diskless boot */
	bootsect[0x41] = 0x90;  /* or ROM BASIC (delete as applicable) */
	bootsect[0x42] = 0xcd;
	bootsect[0x43] = 0x18;
}



int main(int argc, char **argv)
{

	if (find_arg("--version", argc, argv) > 0) return version(); 
	if (argc < 3) return help(argc, argv);
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
	first     = check_numeric("-first", &argc, argv);
	last      = check_numeric("-last", &argc, argv);
	if (present_arg("-idstep", &argc, argv)) idstep = 1;
	if (present_arg("-odstep", &argc, argv)) odstep = 1;
	if (present_arg("-md3", &argc, argv)) md3 = 1;
	if (present_arg("-apricot", &argc, argv)) apricot = 1;
	if (present_arg("-noformat", &argc, argv)) noformat = 1;
	if (present_arg("-logical", &argc, argv)) logical = 1;
	if (!outtyp) outtyp = "dsk";
        format    = check_format("-format", &argc, argv);
	args_complete(&argc, argv);
	return do_copy(argv[1], argv[2]);
}



int do_copy(char *infile, char *outfile)
{
	DSK_PDRIVER indr = NULL, outdr = NULL;
	dsk_err_t e;
	dsk_pcyl_t cyl;
	dsk_phead_t head;
	dsk_psect_t sec;
	char *buf = NULL;
	char *cmt = NULL;
	DSK_GEOMETRY dg;
	char *op = "Opening";

        dsk_reportfunc_set(report, report_end);

	        e = dsk_open (&indr,  infile,  intyp, incomp);
	if (!e) e = dsk_set_retry(indr, retries);
	if (!e && inside >= 0) e = dsk_set_option(indr, "HEAD", inside);
	if (!e) e = dsk_creat(&outdr, outfile, outtyp, outcomp);
	if (!e && outside >= 0) e = dsk_set_option(outdr, "HEAD", outside);
	if (!e && idstep) e = dsk_set_option(indr, "DOUBLESTEP", 1);
	if (!e && odstep) e = dsk_set_option(outdr, "DOUBLESTEP", 1);
	if (!e) e = dsk_set_retry(outdr, retries);
	if (format == -1)
	{
		op = "Identifying disc";
		if (!e) e = dsk_getgeom(indr, &dg);
	}
	else if (!e) e = dg_stdformat(&dg, format, NULL, NULL);
	if (!e)
	{	
		buf = dsk_malloc(dg.dg_secsize);
		if (!buf) e = DSK_ERR_NOMEM;
	}
	if (!e)
	{
		/* Copy comment, if any */
		dsk_get_comment(indr, &cmt);
		dsk_set_comment(outdr, cmt);
		printf("Input driver: %s\nOutput driver:%s\n%s",
			dsk_drvdesc(indr), dsk_drvdesc(outdr),
			logical ? "[tracks rearranged]\n" : "");
		if (first < 0) first = 0;
		if (last < 0) last = dg.dg_cylinders - 1;

		for (cyl = first; cyl <= (dsk_pcyl_t)last; ++cyl)
		{
		    for (head = 0; head < dg.dg_heads; ++head)
		    {
                        if (md3)
                        {
			// MD3 discs have 256-byte sectors in cyl 1 head 1
			    if (cyl == 1 && head == 1) dg.dg_secsize = 256;
			    else		       dg.dg_secsize = 512;
                        }
			// Format track!
                        if (e == DSK_ERR_OK && (!noformat)) 
			{
				op = "Formatting";
				e = dsk_apform(outdr, &dg, cyl, head, 0xE5);
			}
			if (!e) for (sec = 0; sec < dg.dg_sectors; ++sec)
			{
				printf("Cyl %02d/%02d Head %d/%d Sector %03d/%03d\r", 
					cyl +1, dg.dg_cylinders,
				 	head+1, dg.dg_heads,
					sec+dg.dg_secbase, dg.dg_sectors + dg.dg_secbase - 1); 
				fflush(stdout);
			
				op = "Reading";	
				if (logical)
				{
					dsk_lsect_t ls;
					dsk_sides_t si;

	/* Convert sector to logical using SIDES_ALT sidedness. Raw DSKs 
	 * are always created so that the tracks are stored in SIDES_ALT 
	 * order. */
					si = dg.dg_sidedness;
					dg.dg_sidedness = SIDES_ALT;
					dg_ps2ls(&dg, cyl, head, sec, &ls);
					dg.dg_sidedness = si;
					e = dsk_lread(indr, &dg, buf, ls);
				}
				else e = dsk_pread(indr, &dg, buf, cyl,head,sec + dg.dg_secbase);
				// MD3 discs have deliberate bad sectors in cyl 1 head 1
                                if (md3 && e == DSK_ERR_DATAERR && dg.dg_secsize == 256) e = DSK_ERR_OK;
				if (e) break;
				op = "Writing";
#if DSKTRANS_DEBUG
{ 
 int xn, yn;
 char xbuf[20];
 for (xn = 0; xn < sizeof(xbuf)/2; xn++)
 {
  if (isprint(buf[xn])) xbuf[xn] = buf[xn]; else xbuf[xn] = '.';
 }
 yn = dg.dg_secsize - (sizeof(xbuf)-1) + xn;
 for (; xn < sizeof(xbuf)-1; xn++, yn++)
 {
  if (isprint(buf[yn])) xbuf[xn] = buf[yn]; else xbuf[xn] = '.';
 }
 xbuf[xn] = 0;

 printf("WR: c%02d h%d s%d %02x %02x %02x %02x %s\n", 
         cyl, head, sec, 
	 buf[0] & 0xFF, buf[1] & 0xFF, buf[2] & 0xFF, buf[3] & 0xFF, xbuf);
}
#endif
				if (apricot && cyl == 0 && head == 0 && 
						sec == 0)
					apricot_bootsect((unsigned char *)buf);
				e = dsk_pwrite(outdr,&dg,buf,cyl,head, sec + dg.dg_secbase);
				if (e) break;
			}
			if (e) break;
		    }
		    if (e) break;
		}
	
	}
	printf("\r                                     \r");
	if (indr)  dsk_close(&indr);
	if (outdr) dsk_close(&outdr);
	if (buf) dsk_free(buf);
	if (e)
	{
		fprintf(stderr, "\n%s: %s\n", op, dsk_strerror(e));
		return 1;
	}
	return 0;
}

