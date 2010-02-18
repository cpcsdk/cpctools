/*
    ufiformat Version 0.9.6 2009/11/02

    Copyright (C) 2005-2009 Kazuhiro Hayashi <tedi@tedi.sakura.ne.jp>
    Copyright (C) 2005 John Floyd <jfloyd@bigpond.net.au>

    The method of formatting a floppy on USB-FDD used in this program
    is introduced by Bruce M Simpson.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <errno.h>
#include "ufi_command.h"

#define TIME_OUT (100 * 1000)	/* milliseconds */

// Command format
// Byte 0 : opcode
// Byte 1 : LUN (bits 7..5) (always 0)
// Byte 2..5 : LBA
// Byte 6 : reserved
// Byte 7..8 : Transfer/Param list/Alloc length
// Byte 9..11 : reserved
//
// Ext. Command Block
// 0 : opcode
// 1 : LUN (7..5) (always 0)
// 2..5 : LBA
// 6..9 : Transfer/ParamList/AllocLength
// 10..11 : reserved

// LBA = (((Track*HeadTrk)+Head)*SecTrk)+(Sector-1)
// HeadTrk = 1 or 2 heads
// Head : 0 or 1
// Track : 0..79
// SecTrk : 256
// Sector : 1..256

// Sector = LBA%SecTrk +1
// Head = (LBA/SecTrk)%HeadTrk
// Track = LBA/SecTrk/HeadTrk

// Error > REQUEST SENSE or SEND DIAG to get out
// Abort inprogress command : SEND DIAGNOSTICS or USB Reset

// Commands
// 0x00 Test unit ready
static const unsigned char TEST_UNIT_READY_CMD[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 0x01 Rezero Unit
// 0x03	Request sense
// 0x04	Format unit
static const unsigned char FORMAT_UNIT_CMD[] = {
    0x04,  0x17, 	// Command
	0x00,  			// Trac number
	0x00, 0x00, 	// Interleave (0 or 1 for 1:1, else ??)
	0x00, 0x00, 
	0x00, 0x0C, 	// Param list length
	0x00, 0x00, 0x00
};
static const unsigned char FORMAT_UNIT_DATA[] = {
    0x00,
	0xB0, // 101S000s ; S=Single Track ; s = side (0:bottom)
	0x00, 0x08,
	0x00, 0x00, 0x00, 0x00, // Num of blocks (should match result of 0x23)
	0x00, 					// (match 0x23)
	0x00, 0x00, 0x00 		// block length (match 0x23)
};
// Output :
// If ok, Sense key = NO SENSE
// Medium Error : SK = 03 > Request snese data to know where (LBA+Valid bit)

// 0x12	Inquiry / get dev info
static const unsigned char INQUIRY_CMD[] = {
    0x12, 0x00, 0x00, 0x00, 
	0x00, // Should be 36
	0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};
// Output :
// byte 0 : 0 = ok, 0x1F = no drive (wtf)
// byte 1 : 0x80
// byte 2 : 0
// byte 3 : 1
// byte 4 : 0x1F
// byte 5..7 : 0
// byte 8..15 : vendor ID (ASCII)
// 16..31 : product ASCII
// 32..35 : ascii rev. num.

// 0x1B	Start/Stop (load/unload media)
// 0x1D Send diagnostic
// 0x1E	Prevent/Allow medium removal
// 0x23 Read format capacity
static const unsigned char READ_FORMAT_CAPACITIES_CMD[] = {
    0x23, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

// 0x25	Read capacity
// 0x2A Write 10
// 0x28	Read 10
// 0x2B Seek 10
// 0x2E Write and Verify
// 0x2F Verify
// 0x55	Mode select
static const unsigned char MODE_SELECT_CMD[] = {
    0x55, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, // Adjust ? (Parameter List Length)
	0x00, 0x00, 0x00
};
// 0x5A	Mode sense
static const unsigned char MODE_SENSE_CMD[] = {
    0x5A, 0x00, 
	0x05, // bit 7..6: Page Control (0=current, 1=changeable, 2=default) / others : Page Code
		// 0 : get current values (set by mode sense)
		// 1 : get mask (what can be changed)
		// 2 : default values
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x40, // PLL
	0x00, 0x00, 0x00
};

// Param for 0x55 and 0x5A
// Page 0 : 8byte : Media Type and Write Protect
// 1		12		RW Error recovery
// 5		32		Flexible Disk
// 1B		12		Removable Block Access capacities
// 1C		8		Timer and protect
// 3F		72		Mode sense : return all pages

// 0xA8	Read 12
// 0xAA Write 12


static int verbose = 0;

void ufi_set_verbose(int v)
{
    verbose = v;
}

static int ufi_invoke(int fd, const char *cmd, size_t cmd_size, char *data, size_t data_size, int direction)
{
    sg_io_hdr_t sg_io_hdr;
    unsigned char sense_buffer[32];

    memset(&sg_io_hdr, 0, sizeof(sg_io_hdr));
    sg_io_hdr.interface_id = 'S';
    sg_io_hdr.cmdp = (char *)cmd;
    sg_io_hdr.cmd_len = cmd_size;
    sg_io_hdr.dxfer_direction = direction;
    sg_io_hdr.dxferp = data;
    sg_io_hdr.dxfer_len = data_size;
    sg_io_hdr.sbp = sense_buffer;
    sg_io_hdr.mx_sb_len = sizeof(sense_buffer);
    sg_io_hdr.timeout = TIME_OUT;

    if (ioctl(fd, SG_IO, &sg_io_hdr) < 0) {
        return UFI_ERROR;
    }
    if (cmd[0] == TEST_UNIT_READY_CMD[0]) {
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x6 && sense_buffer[12] == 0x28 && sense_buffer[13] == 0x00) {
	    /* media change */
	    if (ioctl(fd, SG_IO, &sg_io_hdr) < 0) {
		return UFI_ERROR;
	    }
	}
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x3 && sense_buffer[12] == 0x30 && sense_buffer[13] == 0x01) {
	    /* unformatted media */
	    return UFI_UNFORMATTED_MEDIA;
	}
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x2 && sense_buffer[12] == 0x3a && sense_buffer[13] == 0x00) {
	    /* no media */
	    return UFI_NO_MEDIA;
	}
    }
    if (sg_io_hdr.masked_status != GOOD) {
	if (verbose) {
	    int i;
	    fprintf(stderr, "SCSI error(command=%02x, status=%02x)\n", *cmd, sg_io_hdr.masked_status);
	    for (i = 0; i < sizeof(sense_buffer); i++) {
		printf("%02x ", sense_buffer[i]);
		if (i % 16 == 15) printf("\n");
	    }
	}
	errno = EPROTO;
	return UFI_ERROR;
    }
    return UFI_GOOD;
}

#define ufi_invoke_to(fd, cmd, data) \
  ufi_invoke(fd, cmd, sizeof(cmd), data, sizeof(data), SG_DXFER_TO_DEV)
#define ufi_invoke_from(fd, cmd, data) \
  ufi_invoke(fd, cmd, sizeof(cmd), data, sizeof(data), SG_DXFER_FROM_DEV)
#define ufi_invoke_no(fd, cmd) \
  ufi_invoke(fd, cmd, sizeof(cmd), NULL, 0, SG_DXFER_TO_DEV)

static char *dup_name(const char *src, int offset, int count)
{
    int i;
    char *name;
    
    for (i = count; i > 0; i--) {
	if (src[offset + i - 1] != ' ') {
	    break;
	}
    }
    name = malloc(i + 1);
    memcpy(name, src + offset, i);
    name[i] = '\0';
    return name;
}

#define TWO_BYTES_TO_INT(ptr) ((*(ptr) << 8) + *((ptr) + 1))
#define THREE_BYTES_TO_INT(ptr) ((TWO_BYTES_TO_INT(ptr) << 8) + *((ptr) + 2))
#define FOUR_BYTES_TO_INT(ptr) ((THREE_BYTES_TO_INT(ptr) << 8) + *((ptr) + 3))

int ufi_test_unit_ready(int fd)
{
    int result;
    result = ufi_invoke_no(fd, TEST_UNIT_READY_CMD);
    return result == UFI_GOOD ? UFI_FORMATTED_MEDIA : result;
}

struct ufi_capacities *ufi_read_format_capacities(int fd, int *type)
{
    unsigned char response[READ_FORMAT_CAPACITIES_CMD[8]];
    struct ufi_capacities *top = NULL;
    struct ufi_capacities *last = NULL;
    struct ufi_capacities *current;
    int i;

    if (ufi_invoke_from(fd, READ_FORMAT_CAPACITIES_CMD, response) != 0) {
	return NULL;
    }
    for (i = 0; i < response[3]; i += 8) {
	current = malloc(sizeof(struct ufi_capacities));
        current->blocks = FOUR_BYTES_TO_INT(response + 4 + i);
        current->block_size = THREE_BYTES_TO_INT(response + 4 + i + 5);
	current->next = NULL;
	if (last != NULL) {
	    last->next = current;
	}
	if (top == NULL) {
	    top = current;
	}
	last = current;
    }
    if (top == NULL) {
	errno = EPROTO;
	return NULL;
    }
    *type = response[4 + 4] & 0x3;
    return top;
}

struct ufi_product *ufi_inquiry(int fd)
{
    unsigned char response[INQUIRY_CMD[8]];
    struct ufi_product *product;

    if (ufi_invoke_from(fd, INQUIRY_CMD, response) != 0) {
	return NULL;
    }
    product = malloc(sizeof(struct ufi_product));
    product->vendor = dup_name(response, 8, 8);
    product->product = dup_name(response, 16, 16);
    return product;
}

int ufi_mode_sense(int fd, int *write_protected)
{
    unsigned char response[MODE_SENSE_CMD[8]];

    if (ufi_invoke_from(fd, MODE_SENSE_CMD, response) != 0) {
	return UFI_ERROR;
    }
    *write_protected = (response[3] & 0x80) ? UFI_PROTECTED : UFI_NOT_PROTECTED;
    return UFI_GOOD;
}

int ufi_format_unit(int fd, int blocks, int block_size, int track, int head)
{
    unsigned char command[sizeof(FORMAT_UNIT_CMD)];
    unsigned char data[sizeof(FORMAT_UNIT_DATA)];

    memcpy(command, FORMAT_UNIT_CMD, sizeof(command));
    memcpy(data, FORMAT_UNIT_DATA, sizeof(data));
    command[2] = track;
    data[1] |= head;
    data[4] = (blocks >> 24) & 0xff;
    data[5] = (blocks >> 16) & 0xff;
    data[6] = (blocks >> 8) & 0xff;
    data[7] = blocks & 0xff;
    data[9] = (block_size >> 16) & 0xff;
    data[10] = (block_size >> 8) & 0xff;
    data[11] = block_size & 0xff;

    if (ufi_invoke_to(fd, command, data) != 0) {
	return UFI_ERROR;
    }
    return UFI_GOOD;
}
