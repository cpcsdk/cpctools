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

/* LibDsk slave for serial ports. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "libdsk.h"
#include "crc16.h"

#ifdef HAVE_WINDOWS_H
# include <windows.h>
#endif

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

unsigned char okay[2] = {0, 0};
unsigned char pkt_in[20000];
unsigned char pkt_out[20000];
unsigned char tmp[2];
unsigned char pkt_h[2];
unsigned char pkt_t[2];

static long baud;
static int crtscts;
static dsk_err_t read_bytes(int count, unsigned char *c);
static dsk_err_t write_bytes(int count, unsigned char *c);
static unsigned char crc16tab[512];
#define SOH 1
#define STX 2
#define ACK 6
#define NAK 21

/* Code specific to the termios library */

#ifdef HAVE_TERMIOS_H
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <termios.h>
#include <sys/time.h>
#include <errno.h>


static int infd, outfd;

static void set_params (struct termios *termios_p);
static dsk_err_t wrbyte(unsigned char c);


static dsk_err_t serial_open(const char *name)
{	
	char *sep, *filename;
	struct termios t;

	filename = dsk_malloc(strlen(name) + 1);
	if (!filename) return DSK_ERR_NOMEM;
	strcpy(filename, name);
	sep = strchr(filename, ',');
	if (sep) *sep = 0;
/* We have a filename. Let's open it up... */
	infd = open(filename, O_RDONLY | O_NONBLOCK);
	if (infd < 0) 
	{
		dsk_free(filename);
		filename = NULL;
		return DSK_ERR_SYSERR;
	}	
	outfd = open(filename, O_WRONLY | O_APPEND | O_NONBLOCK);
	if (outfd < 0) 
	{
		close(infd);
		dsk_free(filename);
		filename = NULL;
		return DSK_ERR_SYSERR;
	}	
/* Comms channels open. See if the user provided a baud rate */	
	baud = 9600;
	sep = strchr(name, ',');
/* If the filename has a comma, then the bit after the comma is options. 
 *
 * Option syntax is:
 * {baud}{+crtscts|-crtscts}
 *
 */ 
	if (sep)
	{
		char *opt1, *opt2, *opte;

		++sep;
		opt1 = strstr(sep, "+crtscts");
		opt2 = strstr(sep, "-crtscts");
		opte = strchr(sep, ',');
		if (!opte) opte = sep + strlen(sep);
		if      (opt1 && opt1 < opte) crtscts = 1;
		else if (opt2 && opt2 < opte) crtscts = 0;
		else                          crtscts = 1;
		baud = atol(sep);
		name = sep;
	}
	tcgetattr(infd, &t);
	set_params(&t);
	tcsetattr(infd, TCSADRAIN, &t);
	tcgetattr(outfd, &t);
	set_params(&t);
	tcsetattr(outfd, TCSADRAIN, &t);
	CRC_Init(crc16tab);
	dsk_free(filename);
	return DSK_ERR_OK;
}


static dsk_err_t serial_close()
{
	if (close(outfd)) return DSK_ERR_SYSERR;
	if (close(infd)) return DSK_ERR_SYSERR;
	return DSK_ERR_OK;
}



static void set_params(struct termios *termios_p)
{
	termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|
			IGNCR|ICRNL|IXON);
	termios_p->c_oflag &= ~OPOST;
	termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	/* Mask out parity and bits */
	termios_p->c_cflag &= ~(CSIZE|PARENB);
	/* Enable hardware handshaking */
	if (crtscts) termios_p->c_cflag |= CRTSCTS;
	else		   termios_p->c_cflag &= ~CRTSCTS;
	/* 1 stop bit */
	termios_p->c_cflag &= ~CSTOPB;
	/* 8 bits */
	termios_p->c_cflag |=  CS8;
	switch(baud)
	{
		case 50:     cfsetispeed(termios_p, B50); break;
		case 110:    cfsetispeed(termios_p, B110); break;
		case 134:    cfsetispeed(termios_p, B134); break;
		case 150:    cfsetispeed(termios_p, B150); break;
		case 300:    cfsetispeed(termios_p, B300); break;
		case 600:    cfsetispeed(termios_p, B600); break;
		case 1200:   cfsetispeed(termios_p, B1200); break;
		case 1800:   cfsetispeed(termios_p, B1800); break;
		case 2400:   cfsetispeed(termios_p, B2400); break;
		case 4800:   cfsetispeed(termios_p, B4800); break;
		case 19200:  cfsetispeed(termios_p, B19200); break;
		case 38400:  cfsetispeed(termios_p, B38400); break;
		case 57600:  cfsetispeed(termios_p, B57600); break;
		case 115200: cfsetispeed(termios_p, B115200); break;
		default:     fprintf(stderr, "Unrecognised bitrate, using 9600\n");
		case 9600:   cfsetispeed(termios_p, B9600); break;
	}

}



static dsk_err_t wrbyte(unsigned char c)
{
	int tries, err;
	unsigned char var;

	for (tries = 0; tries < 30; tries++)
	{
		var = c;
		err = write(outfd, &var, 1);
		if (err == 1) 
		{
	/*		printf("Wrote %02x\n", c); fflush(stdout); */
			return DSK_ERR_OK;
		}
		if (err == -1 && errno == EAGAIN)
		{
			sleep(1);
			continue;
		}
		return DSK_ERR_SYSERR;
	}
	return DSK_ERR_TIMEOUT;	/* Timeout! */
}


static dsk_err_t read_bytes(int count, unsigned char *c)
{
	int tries, err, count0;
	unsigned char *c0 ;

	c0 = c;
	count0 = count;
/*	printf("Reading %d bytes\n", count); fflush(stdout); */
/* Wait for 5 minutes for something to happen */
	for (tries = 0; tries < 300;)
	{
		err = read(infd, c, count);
		if (err > 0) 
		{
			count -= err;
			c     += err;
/*
			printf("Got %d that time leaving %d\n", err, count);
			fflush(stdout);
*/
			if (count <= 0)
			{
/*				int n;
				printf("Read ");
				for (n = 0; n < count0; n++) printf("%02x ",
						c0[n]);
				printf("\n");
				fflush(stdout); */
				return DSK_ERR_OK;
			}
			continue;
		}
		sleep(1);
		++tries;
	}
	return DSK_ERR_TIMEOUT;
}



static dsk_err_t write_bytes(int count, unsigned char *c)
{
	int n;
	dsk_err_t err;

	for (n =0; n < count; n++)
	{
		err = wrbyte(c[n]);
		if (err) return err;
	}
	return DSK_ERR_OK;
}
#elif defined(HAVE_WINDOWS_H) && defined(_WIN32)
/* Win32 serial driver */

static HANDLE hComm;
static DCB dcb;
static COMMTIMEOUTS timeouts = { 0, 1000, 300000, 1000, 30000 };
static dsk_err_t serial_open(const char *name)
{	
	char *sep, *filename;
       
	filename = dsk_malloc(strlen(name) + 1);
	if (!filename) return DSK_ERR_NOMEM;
	strcpy(filename, name);
	sep = strchr(filename, ',');
	if (sep) *sep = 0;
/* We have a filename. Let's open it up... */
	hComm = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
			0,	/* Exclusive access */
			NULL,	/* No security */
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if (hComm == INVALID_HANDLE_VALUE) return DSK_ERR_SYSERR;

/* Comms channels open. See if the user provided a baud rate */	
	baud = 9600;
	sep = strchr(name, ',');
/* If the filename has a comma, then the bit after the comma is options. 
 *
 * Option syntax is:
 * {baud}{+crtscts|-crtscts}
 *
 */ 
	if (sep)
	{
		char *opt1, *opt2, *opte;

		++sep;
		opt1 = strstr(sep, "+crtscts");
		opt2 = strstr(sep, "-crtscts");
		opte = strchr(sep, ',');
		if (!opte) opte = sep + strlen(sep);
		if      (opt1 && opt1 < opte) crtscts = 1;
		else if (opt2 && opt2 < opte) crtscts = 0;
		else                          crtscts = 1;
		baud = atol(sep);
		name = sep;
	}
	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	dcb.Parity   = NOPARITY;
	dcb.fParity  = 0;
	dcb.fBinary  = 1;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.BaudRate = baud;
	if (crtscts)
	{
		dcb.fOutxCtsFlow = 1;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
	SetCommState(hComm, &dcb);
	SetCommTimeouts(hComm, &timeouts);
	CRC_Init(crc16tab);
	dsk_free(filename);
	return DSK_ERR_OK;
}


static dsk_err_t serial_close()
{
	if (!CloseHandle(hComm)) return DSK_ERR_SYSERR;
	return DSK_ERR_OK;
}



static dsk_err_t read_bytes(int count, unsigned char *c)
{
	BOOL b;
	DWORD nread;

	b = ReadFile(hComm, c, count, &nread, NULL);
	if (b == TRUE) 
	{
		return DSK_ERR_OK;
	}
	return DSK_ERR_TIMEOUT;
}

static dsk_err_t write_bytes(int count, unsigned char *c)
{
	BOOL b;
	DWORD nwritten;

	b = WriteFile(hComm, c, count, &nwritten, NULL);
	if (b == TRUE) 
	{
		return DSK_ERR_OK;
	}
	return DSK_ERR_SYSERR;
}

#elif defined(HAVE_DOS_H)	/* default implementation: Do nothing */
#include <dos.h>

#define MAXTRIES 30

static int portno;

static dsk_err_t read_bytes(int count, unsigned char *c);
static dsk_err_t write_bytes(int count, unsigned char *c);

static void sleep1()
{
	int n;
	unsigned cx, dx;	
	union REGS rg;

	rg.h.ah = 0;
	int86(0x1A, &rg, &rg);
	cx = rg.x.cx;
	dx = rg.x.dx;
	for (n = 0; n < 18; n++)
	{
/* Busywait for one system clock tick */
		do
		{
			rg.h.ah = 0;
			int86(0x1A, &rg, &rg);
		}
		while (rg.x.cx == cx && rg.x.dx == dx);
		cx = rg.x.cx;
		dx = rg.x.dx;
	}
}

static dsk_err_t wrbyte(unsigned char c)
{
	union REGS rg;
	int tries;

	for (tries = 0; tries < MAXTRIES; tries++)
	{
		rg.h.ah = 3;
		rg.x.dx = portno;
/*		printf("W:INT 0x14: ah=0x03 dx=0x%04x\n", portno); */
		int86(0x14, &rg, &rg);
/*		printf("W:       -> ah=0x%02x\n", rg.h.ah); */
		if (rg.h.ah & 0x20)	/* Can output data */ 
		{
			rg.h.ah = 1;
			rg.h.al = c;
			rg.x.dx = portno;
			int86(0x14, &rg, &rg);
			return DSK_ERR_OK;
		}
		sleep1();
	}
	return DSK_ERR_TIMEOUT;
}


static dsk_err_t rdbyte(unsigned char *c)
{
	union REGS rg;
	int tries;

	for (tries = 0; tries < MAXTRIES; tries++)
	{
		rg.h.ah = 3;
		rg.x.dx = portno;
/*		printf("R:INT 0x14: ah=0x03 dx=0x%04x\n", portno); */
		int86(0x14, &rg, &rg);
/*		printf("R:       -> ah=0x%02x\n", rg.h.ah); */
		if (rg.h.ah & 1) 	/* Input available */
		{
			rg.h.ah = 2;
			rg.x.dx = portno;
			int86(0x14, &rg, &rg);
			*c = rg.h.al;
			return DSK_ERR_OK;
		}
		sleep1();
	}
	return DSK_ERR_TIMEOUT;
}

#ifdef HAVE_SYS_FARPTR_H
/* DPMI / far-pointer version; has to check for FOSSIL using farpeek 
 * functions.  */
#include <go32.h>
#include <sys/farptr.h>

dsk_err_t check_fossil(void)
{
	unsigned short int14off = _farpeekl(_dos_ds, 4 * 0x14);
	unsigned short int14seg = _farpeekl(_dos_ds, 4 * 0x14 + 2);
	unsigned long  int14    = int14seg * 16 + int14off;

/* Before we proceed any further, let's see if there's a FOSSIL driver 
 * present. */
	if (_farpeekb(_dos_ds, int14 + 6) != 0x54 || 
	    _farpeekb(_dos_ds, int14 + 7) != 0x19)
	{
		return DSK_ERR_NOTME;
	}
	return DSK_ERR_OK;
}


#else
typedef unsigned char far *LPBYTE;
typedef LPBYTE far *LPLPBYTE;

dsk_err_t check_fossil(void)
{
	LPLPBYTE int14vec = MK_FP(0, 4 * 0x14);
	LPBYTE int14 = int14vec[0];	
/* Before we proceed any further, let's see if there's a FOSSIL driver 
 * present. */
	if (int14[6] != 0x54 || int14[7] != 0x19)
	{
		return DSK_ERR_NOTME;
	}
	return DSK_ERR_OK;
}

#endif /* def HAVE_SYS_FARPTR_H */

static dsk_err_t serial_open(const char *name)
{	
	char *sep, *filename;
	union REGS rg;

	if (check_fossil())
	{
		fprintf(stderr, "No FOSSIL driver loaded.\n"); 
		return DSK_ERR_NOTME;
	}

	filename = dsk_malloc(strlen(name) + 1);
	if (!filename) return DSK_ERR_NOMEM;
	strcpy(filename, name);
	sep = strchr(filename, ',');
	if (sep) *sep = 0;
	portno = atol(filename);
/* We have the port number. Open the port. */

	rg.h.ah = 4;			/* Initialise driver */
	rg.x.dx = portno;		/* Port no. */
	int86(0x14, &rg, &rg);
	if (rg.x.ax != 0x1954)		/* FOSSIL init failed */
	{
		dsk_free(filename);
		filename = NULL;
		return DSK_ERR_NOTME;
	}	
/* Comms channels open. See if the user provided a baud rate */	
	baud = 9600;
	sep = strchr(name, ',');
/* If the filename has a comma, then the bit after the comma is options. 
 *
 * Option syntax is:
 * {baud}{+crtscts|-crtscts}
 *
 */ 
	if (sep)
	{
		char *opt1, *opt2, *opte;

		++sep;
		opt1 = strstr(sep, "+crtscts");
		opt2 = strstr(sep, "-crtscts");
		opte = strchr(sep, ',');
		if (!opte) opte = sep + strlen(sep);
		if      (opt1 && opt1 < opte) crtscts = 1;
		else if (opt2 && opt2 < opte) crtscts = 0;
		else                          crtscts = 1;
		baud = atol(sep);
		name = sep;
	}
	rg.h.ah = 0;	/* Set baud rate */
	rg.x.dx = portno;
	switch((long)baud)
	{
/* Set up framing -- no parity, 1 stop bit, 8 bits, and specified baud rate */
		case 19200L: rg.h.al = 0x03; break;
		case 38400L: rg.h.al = 0x23; break;
		case   300L: rg.h.al = 0x43; break;
		case   600L: rg.h.al = 0x63; break;
		case  1200L: rg.h.al = 0x83; break;
		case  2400L: rg.h.al = 0xA3; break;
		case  4800L: rg.h.al = 0xC3; break;
		default:
			    fprintf(stderr, "Baud defaulting to 9600\n");
		case  9600L: rg.h.al = 0xE3; break;
	}
	int86(0x14, &rg, &rg);

/* Set handshaking */
	rg.h.ah = 0x0F;
	rg.x.dx = portno;
	rg.h.al = crtscts ? 2 : 0;
	int86(0x14, &rg, &rg);

	CRC_Init(crc16tab);
	return DSK_ERR_OK;
}


static dsk_err_t serial_close(void)
{
	union REGS rg;

	/* FOSSIL close */
	rg.h.ah = 5;
	rg.x.dx = portno;
	int86(0x14, &rg, &rg);

	return DSK_ERR_OK;
}



static dsk_err_t write_bytes(int count, unsigned char *c)
{
	int n;
	dsk_err_t err;

	for (n = 0; n < count; n++)
	{
		err = wrbyte(c[n]);
		if (err) return err;
	}
	return DSK_ERR_OK;
}

static dsk_err_t read_bytes(int count, unsigned char *c)
{
	int n;
	dsk_err_t err;

	for (n = 0; n < count; n++)
	{
		err = rdbyte(&c[n]);
		if (err) return err;
	}
	return DSK_ERR_OK;
}

#else	/* default implementation: Do nothing */


static dsk_err_t serial_open(const char *name)
{	
	return DSK_ERR_NOTIMPL;
}

static dsk_err_t serial_close(const char *name)
{	
	return DSK_ERR_NOTIMPL;
}


static dsk_err_t read_bytes(int count, unsigned char *c)
{
	return DSK_ERR_NOTIMPL;
}

static dsk_err_t write_bytes(int count, unsigned char *c)
{
	return DSK_ERR_NOTIMPL;
}

#endif /* End of system-specific code */



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



/* Read an incoming packet */
dsk_err_t read_packet(byte  *pkt, int *length)
{
	byte ch;
	unsigned len, n, crc;
	dsk_err_t err;

	while(1)
	{
		CRC_Clear();
		do
		{
			err = read_bytes(1, &ch);
			if (err) return err;
		}
		while (ch != SOH);
		err = read_bytes(2, pkt_h);
		if (err) return err;
		len = pkt_h[0];
		len = (len << 8) | pkt_h[1];
/*		printf("\nlen=%d\n", len); fflush(stdout);  */
		err = read_bytes(len, pkt);
		if (err) return err;
		err = read_bytes(2, pkt_t);
		if (err) return err;
		crc = pkt_t[0];
		crc = (crc << 8) | pkt_t[1];
		for (n = 0; n < len; n++)
		{
			CRC_Update(pkt[n]);
		}
/*		printf("\ncrc=%x\n", crc); fflush(stdout);  */
		if (crc == CRC_Done())
		{
			ch = ACK;
			*length = len;
			return write_bytes(1, &ch);
		}
		ch = NAK;
		err = write_bytes(1, &ch);
		if (err) return err;
	}
	return DSK_ERR_UNKNOWN;	/* Should never happen */
}

/* Write a packet */
dsk_err_t write_packet(byte *pkt, unsigned int len)
{
	byte ch;
	unsigned n, crc;
	dsk_err_t err;

	while(1)
	{
		CRC_Clear();
		pkt_h[0] = (len >> 8);
		pkt_h[1] = (len & 0xFF);
		for (n = 0; n < len; n++)
		{
			CRC_Update(pkt[n]);
		}
		crc = CRC_Done();
		pkt_t[0] = crc >> 8;
		pkt_t[1] = crc & 0xFF;

		ch = STX;	/* Start of return packet */
		err           = write_bytes(1,   &ch);
		if (!err) err = write_bytes(2,   pkt_h);
		if (!err) err = write_bytes(len, pkt);
	        if (!err) err = write_bytes(2,   pkt_t);
		if (!err) err = read_bytes( 1,   &ch);
		if (err) return err;

		if (ch == ACK) 
		{
			return DSK_ERR_OK;
		}
		/* If ch isn't NAK, we're in trouble... */
	}
	return DSK_ERR_UNKNOWN;
}






int main(int argc, char **argv)
{
	int nRefCount = 0;
	int pkt_len;	
	int out_len;
	dsk_err_t err;

	if (argc < 2)
	{
		fprintf(stderr, "Syntax: %s port,speed\n", argv[0]);
		return 1;
	}

	err = serial_open(argv[1]);
	if (err)
	{
		fprintf(stderr, "serslave error: %s\n",
				dsk_strerror(err));
		return 1;
	}
	fprintf(stderr, "serslave launched\n");
	dsk_reportfunc_set(report, report_end);
	/* Wait for an incoming packet */
	do
	{
		err = read_packet(pkt_in, &pkt_len);
		if (err)
		{
			fprintf(stderr, "serslave error: %s\n",
					dsk_strerror(err));
			return 1;
		}

		out_len = sizeof(pkt_out);
		err = dsk_rpc_server(pkt_in, pkt_len, pkt_out, &out_len, 
				&nRefCount);
		if (err)
		{
			fprintf(stderr, "serslave error: %s\n",
					dsk_strerror(err));
			return 1;
		}
		out_len = sizeof(pkt_out) - out_len;
		err = write_packet(pkt_out, out_len);
	}
	while (nRefCount);	
	fprintf(stderr, "serslave terminating\n");
	serial_close();
	return 0;
}



