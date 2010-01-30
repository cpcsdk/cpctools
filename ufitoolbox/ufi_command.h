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

#ifndef UFI_COMMAND_H
#define UFI_COMMAND_H

struct ufi_capacities {
    int blocks;
    int block_size;
    struct ufi_capacities *next;
};

struct ufi_product {
    char *vendor;
    char *product;
};

void ufi_set_verbose(int verbose);
int ufi_test_unit_ready(int fd);
struct ufi_capacities *ufi_read_format_capacities(int fd, int *type);
struct ufi_product *ufi_inquiry(int fd);
int ufi_mode_sense(int fd, int *write_protected);
int ufi_format_unit(int fd, int blocks, int block_size, int track, int head);

#define UFI_GOOD 0
#define UFI_ERROR -1
#define UFI_UNFORMATTED_MEDIA 1
#define UFI_FORMATTED_MEDIA 2
#define UFI_NO_MEDIA 3

#define UFI_PROTECTED 1
#define UFI_NOT_PROTECTED 0
#endif
