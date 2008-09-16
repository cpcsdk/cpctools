/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2005,6  John Elliott <jce@seasip.demon.co.uk>          *
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

/* DSKUTIL is the start of a console-mode disc editor, modelled after the 
 * CP/M utility DU90. Eventually it should allow interactive access to 
 * all of the features of LibDsk. At the moment its reportoire is rather
 * more restricted. 
 *
 * TODO: It really oughtn't all to be in one huge source file. At the
 * very least, the system-dependent bits (such as the calls to sleep() and
 * the line input) should be in a separate file.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "config.h"
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include "libdsk.h"
#include "utilopts.h"
#include "formname.h"

#ifdef CPM
#define AV0 "DSKUTIL"
#else
#define AV0 argv[0]
#endif

static dsk_format_t format = -1;	/* Format for disc image */
static	char *intyp = NULL;		/* LibDsk driver to use */
static	char *incomp = NULL;		/* LibDsk compression to use */
static	int inside = -1;		/* Force one or other side */
static  int idstep =  0;		/* Force double-stepping */
static  int retries = 1;		/* Retry count */

/* DSKUTIL copies DU90 in using track,sector rather than cylinder,head,sector
 * addresses. This puts us in the strange situation of using logical tracks
 * but physical sectors. */
static  dsk_ltrack_t track = 0;		/* Current track */
static  dsk_psect_t sector = 0;		/* Current sector */

static DSK_PDRIVER indr = NULL;		/* LibDsk driver for disc/image */
static DSK_GEOMETRY dg;			/* LibDsk geometry for disc/image */
static unsigned char *secbuf = NULL;	/* Buffer for current sector */
static unsigned char *clipbuf = NULL;	/* Buffer for clipboard copy */
static char *infile;			/* Filename of disc/image */

int do_util(void);			/* Utility main loop */
dsk_err_t obey(char *command);		/* Obey a dskutil command */
void duhelp(void);			/* Display help screen */
int get_decimal(const char *s);		/* Parse a decimal number; returns 
					  -1 if the passed string isn't 
					  a decimal number. If the number
					  starts '#' it is instead parsed as
					  hex.*/
int get_hex(const char *s);		/* As get_decimal(), but parses hex
					   and uses # to switch to decimal */
void show_geometry(void);		/* Display the current DSK_GEOMETRY */
int inc_sector(void);			/* Increment current sector; returns
					   0 if at end of disc, else 1 */
int dec_sector(void);			/* Decrement current sector; returns
					   0 if at start of disc, else 1 */
dsk_err_t search(const char *arg);	/* Search forward for ASCII string */
dsk_err_t parse_search(const char *arg, unsigned char **buf, int *buflen);
dsk_err_t parse_hex(const char *arg, unsigned char **buf, int *buflen);
dsk_err_t change(const char *arg, int ascii);
dsk_err_t yank(void);
void yank_free(void);
dsk_err_t new_secsize(size_t newsize);

typedef struct buffer
{
	struct buffer *next;
	size_t length;
	unsigned char data[1];
} BUFFER;

BUFFER *yank_chain = NULL;

/* Variables that can be got and set by the '$' command. Currently these 
 * are all members of DSK_GEOMETRY. We start by enumerating their types: */
typedef enum
{
	VT_SIDES,
	VT_CYLS,
	VT_HEADS,
	VT_SECTORS,
	VT_SIZE,
	VT_RATE,
	VT_GAP,
	VT_BOOLEAN
} TYPE;

/* And this describes one variable */
typedef struct variable
{
	TYPE type;
	void *data;
	const char *name;
} VARIABLE;

/* All current variables */
static VARIABLE vars[] = 
{
	{ VT_SIDES,   &dg.dg_sidedness, "sides" },	
	{ VT_CYLS,    &dg.dg_cylinders, "cylinders" },
	{ VT_HEADS,   &dg.dg_heads,     "heads" },
	{ VT_SECTORS, &dg.dg_sectors,   "sectors" },
	{ VT_SECTORS, &dg.dg_secbase,   "secbase" },
	{ VT_SIZE,    &dg.dg_secsize,   "secsize" },
	{ VT_RATE,    &dg.dg_datarate,  "datarate" },
	{ VT_GAP,     &dg.dg_rwgap,     "rwgap" },
	{ VT_GAP,     &dg.dg_fmtgap,    "fmtgap" },
	{ VT_BOOLEAN, &dg.dg_fm,        "fm" },
	{ VT_BOOLEAN, &dg.dg_nomulti,   "nomulti" },
	{ VT_BOOLEAN, &dg.dg_noskip,    "noskip" },
};

#define MAXVAR ((int)((sizeof(vars)/sizeof(vars[0]))))

int check_digit(const char *s)
{
	if (!isdigit(s[0]))
	{
		printf("'%s' is not numeric\n", s);
		return 0;
	}
	return 1;
}


void set_var(VARIABLE *var, const char *value)
{
	int hexv, r;

	switch(var->type)
	{
		case VT_SIDES: 
			if (!check_digit(value)) return;
			*((dsk_sides_t *)(var->data)) = atol(value);
			break;
		case VT_CYLS:
			if (!check_digit(value)) return;
			*((dsk_pcyl_t *)(var->data)) = atol(value);
			break;
		case VT_HEADS:
			if (!check_digit(value)) return;
			*((dsk_phead_t *)(var->data)) = atol(value);
			break;
		case VT_SECTORS:
			if (!check_digit(value)) return;
			*((dsk_psect_t *)(var->data)) = atol(value);
			break;
		case VT_SIZE:
			if (!check_digit(value)) return;
			if (!atol(value))
			{
				printf("Size cannot be zero\n");
				return;
			}
			if (var->data == &dg.dg_secsize)
			{
				int err = new_secsize(atoi(value));
				if (err) return;
			}
			*((size_t *)(var->data)) = atoi(value);
			break;
		case VT_RATE:
			switch(toupper(value[0]))
			{
				case 'H': *((dsk_rate_t *)(var->data))=RATE_HD; 
					  break;
				case 'D': *((dsk_rate_t *)(var->data))=RATE_DD; 
					  break;
				case 'S': *((dsk_rate_t *)(var->data))=RATE_SD; 
					  break;
				case 'E': *((dsk_rate_t *)(var->data))=RATE_ED; 
					  break;
				default:  printf("Data rate must be HD/DD/SD/ED\n");
					  break;
			}
			break;
		case VT_GAP:
			r = sscanf(value, "%x", &hexv);
			if (!r)
			{
				printf("Gaps must be specified in hex\n");
				return;
			}
			*((dsk_gap_t *)(var->data)) = hexv;
			break;
		case VT_BOOLEAN:
			switch(toupper(value[0]))
			{
				case '1': case 'Y':
					*((int *)(var->data)) = 1;
					break;
				case '0': case 'N':
					*((int *)(var->data)) = 0;
					break;
			}
			break;
		default:
			printf("Internal error: unsupported variable type %d\n",
				var->type);
	}
}

void print_var(VARIABLE *var)
{
	switch(var->type)
	{
		case VT_SIDES:
			printf("%u", *((dsk_sides_t *)(var->data))); break;
		case VT_CYLS:
			printf("%u", *((dsk_pcyl_t *)(var->data))); break;
		case VT_HEADS:
			printf("%u", *((dsk_phead_t *)(var->data))); break;
		case VT_SECTORS:
			printf("%u", *((dsk_psect_t *)(var->data))); break;
		case VT_SIZE:
			printf("%lu", (long)*((size_t *)(var->data))); break;
		case VT_RATE:
			switch(*((dsk_rate_t *)(var->data)))
			{
				case RATE_HD: printf("HD (500 kbit)"); break;
				case RATE_DD: printf("DD (300 kbit)"); break;
				case RATE_SD: printf("SD (250 kbit)"); break;
				case RATE_ED: printf("ED (1000 kbit)"); break;
				default:      printf("Unknown %d",
						*((dsk_rate_t *)(var->data)));
					      break;
			}
			break;
		case VT_GAP:
			printf("%x", *((dsk_gap_t *)(var->data))); break;
		case VT_BOOLEAN:
			printf("%c", *((int *)(var->data)) ? 'Y' : 'N'); break;
		default:
			printf("Internal error: unsupported variable type %d\n",
				var->type);
	}
}

int find_var(const char *s)
{
	int n, m, match;

	for (n = 0; n < MAXVAR; n++)
	{
		match = 1;
		for (m = 0; vars[n].name[m]; m++)
		{
			/* Treat = as end of variable name */
			if (s[m] == '=' && vars[n].name[m] == 0) break;
			if (tolower(s[m] != vars[n].name[m]))
			{
				match = 0; break;
			}
		}
		if (match) return n;
	}
	return -1;
}

void handle_vars(const char *cmd)
{
	const char *p = strchr(cmd, '=');
	int n;

	if (!cmd[0])
	{
		for (n = 0; n < MAXVAR; n++)
		{
			printf("%s=", vars[n].name);
			print_var(&vars[n]);
			putchar('\n');
		}
		return;
	}
	n = find_var(cmd);
	if (n == -1)
	{
		printf("Variable not found: ");
		for (n = 0; cmd[n]; n++)
		{
			if (cmd[n] == '=') break;
			putchar(cmd[n]);
		}
		return;
	}
	if (p)
	{
		set_var(&vars[n], p + 1);
	}
	printf("%s=", vars[n].name);
	print_var(&vars[n]);
	putchar('\n');
}



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
		       "-dstep         Double-step\n"
		       "-format        Force a specified format name\n");
	fprintf(stderr,"\nDefault type is autodetect.\n\n");
		
	fprintf(stderr, "eg: %s /dev/fd0\n"
                        "    %s -format pcw720 /dev/fd1\n",
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
        format    = check_format("-format", &argc, argv);
 	args_complete(&argc, argv);
	infile = dsk_malloc(1 + strlen(argv[1]));
	if (infile) strcpy(infile, argv[1]);
	else
	{
		fprintf(stderr, "Out of memory; could not allocate %ld bytes\n",
				(long)(1 + strlen(argv[1])));
		return 1;
	}	
	strcpy(infile, argv[1]);
	return do_util();
}



static char *op = "Opening";


int do_util(void)
{
	dsk_err_t e;
	int done = 0, repeat, n;
	char buffer[200], bufcopy[200], *c;
	char *slash;

        dsk_reportfunc_set(report, report_end);
	e = dsk_open (&indr,  infile,  intyp, incomp);
	if (!e) e = dsk_set_retry(indr, retries);
	if (!e && inside >= 0) e = dsk_set_option(indr, "HEAD", inside);
	if (!e && idstep) e = dsk_set_option(indr, "DOUBLESTEP", 1);

/* Try to guess geometry. If this fails the user has to specify an approximate
 * geometry manually. */
        if (format == -1)
        {
                op = "Identifying disc";
                if (!e) e = dsk_getgeom(indr, &dg);
        }
        else if (!e) e = dg_stdformat(&dg, format, NULL, NULL);
	if (!e) e = new_secsize(dg.dg_secsize);

	if (e)
	{
		if (indr)  dsk_close(&indr);
		fprintf(stderr, "\n%s: %s\n", op, dsk_strerror(e));
		return 1;
	}
	/* Sign on */
	printf("LibDsk Disk Utility v" LIBDSK_VERSION "\n\n"
	       "Type ? for help\n"
	       "Type X to exit\n\n");
	do
	{
		repeat = 1;
		printf(":"); 
		fflush(stdout);
		if (!fgets(buffer, sizeof(buffer), stdin))
		{
			strcpy(buffer, "x\n");
		}
		slash = strstr(buffer, ";/");
		if (slash)
		{
			*slash = 0;
			slash += 2;
			if (isdigit(*slash)) repeat = atoi(slash);
/*			else repeat = -1; */
		}
		strcpy(bufcopy, buffer);
		for (n = 0; n < repeat; n++)
		{
			for (c = strtok(buffer, ";\n"); 
				c != NULL; 
				c = strtok(NULL, ";\n"))
			{
				int e = DSK_ERR_OK;

				while (c[0] == ' ' || c[0] == '\t') ++c;
				if (c[0] == 'x' || c[0] == 'X') done = 1;
				else e = obey(c);
				if (e)
				{
					printf("Error in '%s': %s\n",
						c, dsk_strerror(e));
					break;
				}
			}
			strcpy(buffer, bufcopy);
			if (done) break;
		}
	} while (!done);
	if (indr)  dsk_close(&indr);
	if (clipbuf) dsk_free(clipbuf);
	if (secbuf) dsk_free(secbuf);
	yank_free();
	if (e)
	{
		fprintf(stderr, "\n%s: %s\n", op, dsk_strerror(e));
		return 1;
	}
	return 0;
}

static void showts(void)
{
	printf("Track=%u Sector=%u\n", track, sector);
}

static void dump(char *cmd, int showhex, int showascii)
{
	int n, m;
	int from = 0;
	int upto = dg.dg_secsize;
	char *t = NULL;

	t = strchr(cmd, ',');
	if (t == NULL)
	{
		from = get_hex(cmd);
		if (from == -1) from = 0;
	}
	else
	{
		*t = 0;
		t++;
		from = get_hex(cmd);
		if (from == -1) from = 0;
		upto = 1 + get_hex(t);
	}
	if (from < 0  || from >= dg.dg_secsize) from = 0;
	if (upto <= 0 || upto >  dg.dg_secsize) upto = dg.dg_secsize;
	if (from > upto) { n = from; from = upto-1; upto = n+1; }
	for (n = from; n < upto; n = ((n / 16)+1)*16)
	{
		int base = (n/16)*16;
		int c = 0;

		printf("%04x: ", n);
		if (showhex)
		{
			for (m = 0; m < (n%16); m++)
			{
				putchar(' ');
				putchar(' ');
				if ((c & 3) == 3) putchar(' ');
				c++;
			}
			for (m = (n % 16); m < 16; m++)
			{
				printf("%02x", secbuf[base+m]);
				if ((c & 3) == 3) putchar(' ');
				c++;
				if ((n + m + 1) >= upto) break;
			}
			for (; c < 16; c++)
			{
				putchar(' ');
				putchar(' ');
				if ((c & 3) == 3) putchar(' ');
			}
		}
		if (showascii)
		{
			printf(" *");
			for (m = 0; m < (n%16); m++) putchar(' ');
		
			for (m = (n % 16); m < 16; m++)
			{
				if (isprint(secbuf[base+m])) 
					putchar(secbuf[base+m]);
				else    putchar('.');
				if ((n + m + 1) >= upto) break;
			}
			putchar('*');
		}
		putchar('\n');
	}
}


int get_decimal(const char *s)
{
	if (s[0] == '#') return get_hex(s+1);
	if (!isdigit(s[0])) return -1;
	return atoi(s);
}

int get_hex(const char *s)
{
	int v, c;

	if (s[0] == '#') return get_decimal(s+1);
	c = sscanf(s, "%x", &v);
	if (!c) return -1;
	return v;	
}

dsk_err_t yank(void)
{
	BUFFER *b, *b0;
	size_t count;
       
	b = dsk_malloc(sizeof(BUFFER) + dg.dg_secsize);
	if (!b) return DSK_ERR_NOMEM;

	memcpy(b->data, secbuf, dg.dg_secsize);
	b->length = dg.dg_secsize;
	b->next = NULL;

	if (!yank_chain)
	{
		yank_chain = b;
	}
	else
	{
		for (b0 = yank_chain; b0->next != NULL; b0 = b0->next); 
		b0->next = b;
	}
	for (b0 = yank_chain, count = 0; b0 != NULL; b0 = b0->next)
		count += b0->length;
	printf("Total bytes yanked: %lx\n", (long)count);
	return DSK_ERR_OK;
}


dsk_err_t new_secsize(size_t newsize)
{
	if (newsize != dg.dg_secsize || !secbuf || !clipbuf)
	{
		unsigned char *sb1 = dsk_malloc(newsize);
		unsigned char *cb1 = dsk_malloc(newsize);

		if (!sb1 || !cb1)
		{
			fflush(stdout);
			fprintf(stderr, "Out of memory: Cannot dsk_malloc %ld bytes\n", (long)newsize);
			if (sb1) dsk_free(sb1);
			if (cb1) dsk_free(cb1);
			return DSK_ERR_NOMEM;	
		}
		if (secbuf) dsk_free(secbuf);
		if (clipbuf) dsk_free(clipbuf);
		secbuf = sb1;
		clipbuf = cb1;
		memset(secbuf, 0, newsize);
		memset(clipbuf, 0, newsize);
	}
	return DSK_ERR_OK;
}



dsk_err_t new_geometry(const char *cmd)
{
	int fmt = FMT_180K;
	dsk_cchar_t fname;
	dsk_err_t err;
	int found = 0;
	DSK_GEOMETRY newdg;

	while (cmd[0] == ' ' || cmd[0] == '\t') ++cmd;
	if (cmd[0]) 
	{
		while (dg_stdformat(&newdg, fmt, &fname, NULL) == DSK_ERR_OK)
		{
			if (!strcmpi(cmd, fname))
			{
				found = 1;
				break;
			}
			++fmt;
		}
		if (!found)
		{
			printf("Unknown format name: '%s'\n", cmd);
			return DSK_ERR_OK;
		}
	}
	else
	{
		err = dsk_getgeom(indr, &newdg);
		if (err) return err;	
	}
	if (new_secsize(newdg.dg_secsize))
	{
		return DSK_ERR_OK;
	}
	memcpy(&dg, &newdg, sizeof(dg));
	return DSK_ERR_OK;
}

dsk_err_t do_login(const char *cmd)
{
	DSK_PDRIVER pdrv;
	DSK_GEOMETRY newdg;
	char *buf = dsk_malloc(1 + strlen(cmd));
	char *name;
	char *type;
	char *compr;
	dsk_err_t err;

	if (!buf) return DSK_ERR_NOMEM;
	strcpy(buf, cmd);
	name = strtok(buf, ",");
	type = strtok(NULL, ",");
	compr = strtok(NULL, ",");
	if (name == NULL)
	{
		dsk_free(buf);
		return new_geometry("");
	}
	err = dsk_open(&pdrv, name, type, compr);
	if (err) return err;
	err = dsk_getgeom(pdrv, &newdg);
	if (err)
	{
		printf("Could not probe geometry for '%s' (%s).\n"
		       "Using existing geometry; if this is wrong use N or $ to change it\n", name, dsk_strerror(err));
		dsk_close(&indr);	
		indr = pdrv;
		dsk_free(infile);
		infile = dsk_malloc(strlen(name) + 1);
		if (!infile) infile = "<unknown>";
		else strcpy(infile, name);
		dsk_free(buf);
		return DSK_ERR_OK;
	}
	if (new_secsize(newdg.dg_secsize))
	{
		dsk_free(buf);
		return DSK_ERR_OK;
	}
	dsk_close(&indr);	
	indr = pdrv;
	memcpy(&dg, &newdg, sizeof(dg));
	dsk_free(infile);
	infile = dsk_malloc(strlen(name) + 1);
	if (!infile) infile = "<unknown>";
	else strcpy(infile, name);
	dsk_free(buf);
	return DSK_ERR_OK;
	
}


void yank_free(void)
{
	BUFFER *b, *b0;

	for (b = yank_chain; b != NULL; )
	{
		b0 = b;
		b = b->next;
		dsk_free(b0);
	}
	yank_chain = NULL;
}


dsk_err_t save_yank(const char *s)
{
	FILE *fp;
	BUFFER *b0;
	size_t count;

	if (!yank_chain)
	{
		printf("Nothing to save!\n");
		return DSK_ERR_OK;
	}
	fp = fopen(s, "wb");
	if (!fp)
	{
		fflush(stdout);
		perror(s);
		return DSK_ERR_OK;
	}
	count = 0;
	for (b0 = yank_chain; b0 != NULL; b0 = b0->next)
	{
		int res = fwrite(b0->data, 1, b0->length, fp);
		if (res < b0->length)
		{
			printf("Write error on file: %s\n", s);
			fclose(fp);
			return DSK_ERR_OK;
		}
		count += b0->length;
	}
	if (fclose(fp))
	{
		fflush(stdout);
		perror(s);
		return DSK_ERR_OK;
	}
	yank_free();
	printf("%lx bytes written to file: %s\n", (long)count, s);
	return DSK_ERR_OK;
}


void do_sleep(int secs)
{
	/* Try to use system sleep functions if at all possible. */
#if defined(HAVE_WINDOWS_H) && defined(_WIN32)
	Sleep(1000*secs);
#else
#ifdef HAVE_SLEEP
	sleep(secs);
#else
	/* If all else fails, busywait */
	int n;
	time_t t1, t2;
	for (n = 0; n < secs; n++)
	{
		time(&t1);
		do
		{
			time(&t2);
		} while (t1 == t2);
	}
#endif
#endif
}

dsk_err_t obey(char *cmd)
{
	int value;
	dsk_pcyl_t cyl;
	dsk_phead_t head;
	dsk_lsect_t lsect;
	dsk_err_t err;

	switch(toupper(cmd[0]))
	{
		case '#':
			show_geometry();
			return DSK_ERR_OK;
		case '$':
			handle_vars(cmd + 1);
			return DSK_ERR_OK;
		case '<':
			memcpy(clipbuf, secbuf, dg.dg_secsize);
			return DSK_ERR_OK;
		case '=':
			return search(cmd+1);
		case '>':
			memcpy(secbuf, clipbuf, dg.dg_secsize);
			return DSK_ERR_OK;
		case '?':
			duhelp();
			return DSK_ERR_OK;
		case 'A':
			dump(cmd+1, 0, 1);
			return DSK_ERR_OK;
		case 'C':
			switch(toupper(cmd[1]))
			{
				case 'A': return change(cmd+2, 1); 
				case 'H': return change(cmd+2, 0); 
			}
			printf("C command must be CA or CH\n");
			return DSK_ERR_OK;
		case 'D':
			dump(cmd+1, 1, 1);
			return DSK_ERR_OK;
		case 'G':
			value = get_hex(cmd+1);
			if (value >= 0)
			{
				lsect = value;
				err= dg_ls2ps(&dg, lsect, &cyl, &head, &sector);
				if (err) return err;
				err = dg_pt2lt(&dg, cyl, head, &track);
				if (err) return err;
			}
			showts();
			goto doread;
		case 'H':
			dump(cmd+1, 1, 0);
			return DSK_ERR_OK;
		case 'K':
			return save_yank(cmd+1);
		case 'L':
			return do_login(cmd+1);
		case 'N':
			return new_geometry(cmd+1);
		case 'O':
			for (value = 0; value < dg.dg_secsize; value++)
			{
				secbuf[value] ^= 0xFF;
			}
			return DSK_ERR_OK;
		case 'T':
			value = get_decimal(cmd+1);
			if (value >= 0) track = value;
			showts();
			return DSK_ERR_OK;
		case 'S':
			value = get_decimal(cmd+1);
			if (value >= 0) sector = value;
			showts();
			return DSK_ERR_OK;
		case '-':
			value = get_decimal(cmd+1);
			if (value < 0) value = 1;
			while (value > 0)
			{
				dec_sector();
				--value;
			}
			showts();
			goto doread;
		case '+':
			value = get_decimal(cmd+1);
			if (value < 0) value = 1;
			while (value > 0)
			{
				inc_sector();
				--value;
			}
			showts();
			/* FALL THROUGH */
		case 'R':
		doread:
			if (sector < dg.dg_secbase || 
			    sector >= (dg.dg_sectors + dg.dg_secbase))
			{
				printf("Cannot read: Sector not set or out of range\n");
				return DSK_ERR_BADVAL;
			}
			dg_lt2pt(&dg, track, &cyl, &head);
			return dsk_pread(indr, &dg, secbuf, cyl, head, sector);
		case 'V':
			if (sector < dg.dg_secbase || 
			    sector >= (dg.dg_sectors + dg.dg_secbase))
			{
				printf("Cannot read: Sector not set or out of range\n");
				return DSK_ERR_BADVAL;
			}
			dg_lt2pt(&dg, track, &cyl, &head);
			return dsk_pcheck(indr, &dg, secbuf, cyl, head, sector);
		case 'W':
			if (sector < dg.dg_secbase || 
			    sector >= (dg.dg_sectors + dg.dg_secbase))
			{
				printf("Cannot read: Sector not set or out of range\n");
				return DSK_ERR_BADVAL;
			}
			dg_lt2pt(&dg, track, &cyl, &head);
			return dsk_pwrite(indr, &dg, secbuf, cyl, head, sector);
		case 'Y':
			return yank();		
		case 'Z':
			value = get_decimal(cmd+1);
			if (value < 0) value = 1;
			do_sleep(value);
			return DSK_ERR_OK;	
	}
	printf("Unknown command: '%s'\n", cmd);
	return DSK_ERR_OK;
}



void show_geometry(void)
{
	unsigned tracks = dg.dg_cylinders * dg.dg_heads;
	unsigned spt = dg.dg_heads * dg.dg_sectors;

	printf("Statistics for %s:\n", infile);
	printf("Cylinders:\t%4d\t  %02x\n", dg.dg_cylinders, dg.dg_cylinders);
	printf("Heads:\t\t%4d\t  %02x\n", dg.dg_heads, dg.dg_heads);
	printf("Tracks:\t\t%4d\t  %02x\n", tracks, tracks);
	printf("Sec/cyl:\t%4d\t  %02x\n", dg.dg_sectors, dg.dg_sectors);
	printf("Sec/track:\t%4d\t  %02x\n", spt, spt);
	printf("1st sec:\t%4d\t  %02x\n", dg.dg_secbase, dg.dg_secbase);
	printf("Sec size:\t%4ld\t%04lx\n", (long)dg.dg_secsize, (long)dg.dg_secsize);
	printf("Data rate:\t");
	switch(dg.dg_datarate)
	{
		case RATE_SD: printf(" 250\n"); break;
		case RATE_DD: printf(" 300\n"); break;
		case RATE_HD: printf(" 500\n"); break;
		case RATE_ED: printf("1000\n"); break;
	}
	printf("Encoding:\t %s\n\n", dg.dg_fm ? " FM" :"MFM");
}


int inc_sector(void)
{
	dsk_psect_t  os = sector;
	dsk_ltrack_t ot = track;
	dsk_pcyl_t cyl;
	dsk_phead_t head;

	++sector;
	if (sector >= (dg.dg_sectors + dg.dg_secbase))
	{
		sector = dg.dg_secbase;
		++track;
		dg_lt2pt(&dg, track, &cyl, &head);
		if (cyl >= dg.dg_cylinders || 
		    head >= dg.dg_heads) --track;
	}
	return (os != sector || ot != track);
}


int dec_sector(void)
{
	dsk_psect_t  os = sector;
	dsk_ltrack_t ot = track;

	--sector;
	if (sector < dg.dg_secbase || 
	    sector >= (dg.dg_sectors + dg.dg_secbase))
	{
		sector = dg.dg_secbase + dg.dg_sectors - 1;
		if (track > 0) --track;
	}
	return (os != sector || ot != track);
}

dsk_err_t parse_search(const char *arg, unsigned char **buf, int *buflen)
{
	int hex;
	int len;
	unsigned char *ptr;

	*buf = (unsigned char *)dsk_malloc(strlen(arg));
	len = 0;
	if (!(*buf)) return DSK_ERR_NOMEM;

	ptr = *buf;
	while (*arg)
	{
		if (*arg == '<')
		{
			hex = get_hex(arg+1);
			while (*arg != '>' && *arg != 0) ++arg;
			if (*arg == '>') ++arg;
			ptr[len++] = hex;
			continue;
		}
		ptr[len++] = *arg;
		++arg;
	}
	*buflen = len;
	return DSK_ERR_OK;
}


dsk_err_t parse_hex(const char *arg, unsigned char **buf, int *buflen)
{
	int hex;
	int len;
	unsigned char *ptr, *t;

	*buf = (unsigned char *)dsk_malloc(strlen(arg));
	len = 0;
	if (!(*buf)) return DSK_ERR_NOMEM;

	ptr = *buf;
	hex = get_hex(arg);
	if (hex == -1) 
	{
		*buflen = 0;
		return DSK_ERR_OK;
	}
	ptr[len++] = hex;
	while (  (t = (unsigned char *)strchr(arg, ',')) )
	{
		arg = (char *)t + 1;
		hex = get_hex(arg);
		if (hex == -1) break;
		ptr[len++] = hex;
	}
	*buflen = len;
	return DSK_ERR_OK;
}



dsk_err_t search(const char *arg)
{
	unsigned char *buf;
	int buflen = 0;
	int n;
	dsk_err_t err;
	dsk_pcyl_t cyl;
	dsk_phead_t head;

	err = parse_search(arg, &buf, &buflen);
	if (err) return err;
	if (!buflen)
	{
		printf("Nothing to search for\n");
		return DSK_ERR_OK;
	}

/* buf now holds the search pattern */
	if (sector < dg.dg_secbase || 
	    sector >= (dg.dg_sectors + dg.dg_secbase))
	{
		sector = dg.dg_secbase;
	}
	do
	{
		dg_lt2pt(&dg, track, &cyl, &head);
		err = dsk_pread(indr, &dg, secbuf, cyl, head, sector);
		if (err) 
		{
			dsk_free(buf);
			showts();
			return err;
		}
		for (n = 0; n < dg.dg_secsize - buflen; n++)
		{
			if (!memcmp(secbuf + n, buf, buflen))
			{
				dsk_free(buf);
				printf("Match at %04x\n", n);
				showts();
				return DSK_ERR_OK;
			}
		}
	} while (inc_sector());
	printf("Reached end of disk; match not found.\n");
	dsk_free(buf);
	showts();
	return DSK_ERR_OK;

}

int more(void)
{
	char c;

	printf("[Press Return to continue]\r");
	fflush(stdout);
	c = getchar();
	printf("                          \r");
	if (c == 'c' || c == 'C') return 0;
	return 1;
}

int get_range(const char *arg, int *from, int *upto)
{
	char *t = strchr(arg, '-');
	if (t)
	{
		*from = get_hex(arg);
		*upto = get_hex(t+1);
	}
	else
	{
		*from = *upto = get_hex(arg);
	}
	if (*from < 0 || *upto < 0)
	{
		printf("Cannot parse as xx or xx-xx: %s\n", arg);
		return 0;
	}
	if (*from >= dg.dg_secsize) *from = dg.dg_secsize - 1;
	if (*upto >= dg.dg_secsize) *upto = dg.dg_secsize - 1;
	if (*from > *upto)
	{
		int tmp = *from; *from = *upto; *upto = tmp;
	}
	return 1;
}

dsk_err_t change(const char *arg, int ascii)
{
	int from, upto, n;
	unsigned char *buf = NULL;
	int buflen = 0;
	dsk_err_t err;
	char ch = ascii ? 'A' : 'H';

	if (!get_range(arg, &from, &upto)) return DSK_ERR_OK;	
	arg = strchr(arg, ',');
	if (!arg)
	{
		printf("C%c command must be formed C%cxx,values or "
			"C%cxx-yy,value\n", ch, ch, ch);
		return DSK_ERR_OK;
	}
	arg++;
	if (ascii) err = parse_search(arg, &buf, &buflen);
	else	   err = parse_hex(arg, &buf, &buflen);
	if (err) return err;
	if (!buflen)
	{
		printf("C%c command must be formed C%cxx,values or "
			"C%cxx-yy,value\n", ch, ch, ch);
		return DSK_ERR_OK;
	}

	if (from == upto) 
	{
		for (n = 0; n < buflen; n++)
		{
			if (n + from >= dg.dg_secsize) break;
			secbuf[n+from] = buf[n];
		}
	}
	else for (n = from; n <= upto; n++)
	{
		if (n >= dg.dg_secsize) break;
		secbuf[n] = buf[ (n-from) % buflen ];
	}
	dsk_free(buf);
	return DSK_ERR_OK;
}


void duhelp(void)
{
	printf("Operands in brackets [...] are optional\n"
	       "Numeric values: 'n' are decimal, 'x' hex\n"
	       "Prefixing numeric entry with '#' accepts the alternate notation"
	       "\n\n"
	       "+[x]\tstep in [x] sectors\n"
	       "-[x]\tstep out [x] sectors\n"
	       "#\tprint disk parameters for current drive\n"
	       "=Abc\tsearch for ASCII Abc from current sector\n"
	       "\t\tThis is a case-sensitive search.\n"
	       "\t\tUse <xx> for hex, eg: =NEW<D><A>LINE\n"
	       "<\tsave current sector into memory buffer\n"
	       ">\trestore saved sector\n"
	       "?\thelp (displays this guide)\n"
	       "A[from,to] ASCII dump (from/to are in hex)\n"
/*	       "Bnn\tSet number of sectors in boot track\n" */
	       "\n\n\n");
	if (!more()) return;
	printf(
	       "CHaddress,byte,byte... (hex)\n"
	       "CAaddress,data... (ASCII, with <xx> for embedded hex)\n"
	       "CHfrom-to,byte,byte... (eg: CH0-7F,E5 )\n"
	       "CAfrom-to,data...\n"
	       "D[from,to] Hex + ASCII dump (from/to are in hex)\n"
/*	        F was find in DU90 - a CP/M specific function */
	       "Gxx\tGo to logical sector xx\n"
	       "H[from,to] Hex dump (from/to are in hex)\n"
	       "Kfilename  Dump yanked sectors to a file\n" 
	       "Lfilename[,type[,compression]]  Open new disc / image\n" 
/*	        M was map in DU90 - a CP/M specific function */
	       "\n\n\n");
	if (!more()) return;
	printf(
	       "N[geometry]\tNew fixed geometry; or just N to use auto geometry\n" 
	       "O\tOnes complement current sector\n"
/*              P was printer echo */
/*              Q was quiet mode */
	       "R\tRead current sector\n"
	       "Snn\tSector nn\n"
	       "Tnn\tTrack nn\n"
/*              U was set user number - a CP/M specific function */
	       "V\tVerify current sector against buffer\n"
	       "W\tRead current sector\n"
	       "X\tExit program\n"
	       "Y\tYank sector into sequential memory\n" 
	       "$\tPrint geometry variables\n"
	       "$variable  Print one geometry variable\n"
	       "$variable=value  Set value of geometry variable\n"
	       "Z[xx]\tSleep [xx seconds]\n" 
	       "/nn: Repeat command line nn times\n"
	       "\n\n\n");
/*	if (!more()) return; */  /* General usage page followed */
}

