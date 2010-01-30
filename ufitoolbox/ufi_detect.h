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

#ifndef UFI_DETECT_H
#define UFI_DETECT_H

struct ufi_path_info {
    int host_id;
    char *sg_path;
    char *sd_path;
    struct ufi_path_info *next;
};

int convert_to_sg_path(const char *sd, char *sg, size_t len);
int convert_to_sd_path(const char *sg, char *sd, size_t len);
int is_usb_fdd(const char *dev);
int get_usb_fdds(struct ufi_path_info **info);
void free_ufi_path_info(struct ufi_path_info *info);

#endif

