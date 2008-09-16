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

/* LibDsk slave for the 'fork' driver. Expects to be launched by rpcfork.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libdsk.h"


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

unsigned char okay[2] = {0, 0};
unsigned char pkt_in[9000];
unsigned char pkt_out[9000];
unsigned char tmp[2];

void chkread(unsigned char *data, int count)
{
	if (read(0, data, count) < count)
	{
		fprintf(stderr, "read() failed.\n");
		exit(1);
	}
}


void chkwrite(unsigned char *data, int count)
{
	if (write(1, data, count) < count)
	{
		fprintf(stderr, "write() failed.\n");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int nRefCount = 0;
	unsigned pkt_len;
	int out_len, n;
	dsk_err_t err;

	fprintf(stderr, "forkslave launched\n");
	/* Start by sending DSK_ERR_OK to say that we were launched OK. */
	chkwrite(okay, 2);
	dsk_reportfunc_set(report, report_end);
	/* Wait for an incoming packet */
	do
	{
		chkread(tmp, 2);
		pkt_len = (tmp[0] * 256) + tmp[1];
		chkread(pkt_in, pkt_len);
		out_len = sizeof(pkt_out);
		err = dsk_rpc_server(pkt_in, pkt_len, pkt_out, &out_len, 
				&nRefCount);
		if (err)
		{
			fprintf(stderr, "forkslave error: %s\n",
					dsk_strerror(err));
			return 1;
		}
		out_len = sizeof(pkt_out) - out_len;
		tmp[0] = (out_len >> 8) & 0xFF;
		tmp[1] = (out_len     ) & 0xFF;
		chkwrite(tmp, 2);
		chkwrite(pkt_out, out_len);
	}
	while (nRefCount);	
	fprintf(stderr, "forkslave terminating\n");
	return 0;
}



