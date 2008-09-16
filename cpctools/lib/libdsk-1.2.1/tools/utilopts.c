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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "utilopts.h"
#include "libdsk.h"

int version(void)
{
	printf("libdsk version %s\n", LIBDSK_VERSION);
	return 0;
}

void excise_arg(int na, int *argc, char **argv)
{	
	int n;

	--(*argc);
	for (n = na; n < *argc; n++)
	{
		argv[n] = argv[n+1];
	}
}

int find_arg(char *arg, int argc, char **argv)
{
	int n;
	
	for (n = 1; n < argc; n++)
	{
		if (!strcmpi(argv[n], "--")) return -1;
		if (!strcmpi(argv[n], arg)) return n;
	}
	return -1;
}


int check_forcehead(char *arg, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);
	int fh;

	if (n < 0) return -1;	
	/* Remove the "-head" */
	excise_arg(n, argc, argv);
	if (n >= *argc || argv[n][0] < '0' || argv[n][0] > '1') 
	{
		fprintf(stderr, "Syntax error: use '%s 0' or '%s 1'\n", arg, arg);
		exit(1);
	}
	fh = argv[n][0] - '0';
	excise_arg(n, argc, argv);

	return fh;
}


unsigned check_retry(char *arg, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);
	unsigned nr;

	if (n < 0) return 1;	
	/* Remove the "-retryy" */
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


char *check_type(char *arg, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);
	char *v;

	if (n < 0) return NULL;
	/* Remove the "-type" */
	excise_arg(n, argc, argv);
	if (n >= *argc)
	{
		fprintf(stderr, "Syntax error: use '%s <type>'\n", arg);
		exit(1);
	}
	v = argv[n];
	/* Remove the argument */
	excise_arg(n, argc, argv);

	if (!strcmpi(v, "auto")) return NULL;
	return v;
}


int present_arg(char *arg, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);

	if (n < 0) return 0;

	excise_arg(n, argc, argv);
	return 1;
}


int ignore_arg(char *arg, int count, int *argc, char **argv)
{
	int n = find_arg(arg, *argc, argv);

	if (n < 0) return 0;

	fprintf(stderr, "Warning: option '");
	while (count)
	{
		fprintf(stderr, "%s ", argv[n]);
		excise_arg(n, argc, argv);
		--count;
		if (n >= *argc) break;	
	}
	fprintf(stderr, "\b' ignored.\n");
	return 1;
}

void args_complete(int *argc, char **argv)
{
	int n;
	for (n = 1; n < *argc; n++)
	{
		if (!strcmpi(argv[n], "--")) 
		{
			excise_arg(n, argc, argv);
			return;
		}
		if (argv[n][0] == '-')
		{
			fprintf(stderr, "Unknown option: %s\n", argv[n]);
			exit(1);
		}
	}
}

