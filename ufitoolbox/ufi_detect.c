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
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <linux/major.h>
#include <fcntl.h>
#include <scsi/scsi.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "ufi_detect.h"

#ifndef SCSI_DISK_MAJOR
#define SCSI_DISK_MAJOR(M) ((M) == SCSI_DISK0_MAJOR || \
        ((M) >= SCSI_DISK1_MAJOR && (M) <= SCSI_DISK7_MAJOR) || \
        ((M) >= SCSI_DISK8_MAJOR && (M) <= SCSI_DISK15_MAJOR))
#endif

#define HOST_ID(idlun) ((idlun >> 24) & 0xff)

static char *pathcpy(char *dst, const char *src, size_t len)
{
    if (strlen(src) >= len) {
	errno = ENAMETOOLONG;
	return NULL;
    }
    return strcpy(dst, src);
}

static int is_sd_device(const char *dev)
{
    struct stat st;

    if (stat(dev, &st) < 0) {
	return -1;
    }
    return S_ISBLK(st.st_mode) && SCSI_DISK_MAJOR(major(st.st_rdev)) && (minor(st.st_rdev) & 0xf) == 0 ? 1 : 0;
}

static int is_sg_device(const char *dev)
{
    struct stat st;

    if (stat(dev, &st) < 0) {
	return -1;
    }
    return S_ISCHR(st.st_mode) && major(st.st_rdev) == SCSI_GENERIC_MAJOR ? 1 : 0;
}

static int get_id_lun(const char *dev)
{
    int fd;
    int idlun[2];

    if ((fd = open(dev, O_RDONLY | O_NONBLOCK)) < 0) {
	return -1;
    }
    if (ioctl(fd, SCSI_IOCTL_GET_IDLUN, idlun) < 0) {
	close(fd);
	return -1;
    }
    close(fd);
    return idlun[0];
}

static int get_sd_path(int idlun, char *path, size_t len, int host_id_only)
{
    char tmp[PATH_MAX];
    int i;
    int eacces;
    int idlun2;

    eacces = 0;
    /* BUG: only single usb fdd is supported for one host_id */
    if (host_id_only) {
	idlun = (idlun & 0xff) << 24;
    }
    for (i = 0; i < 256; i++) {
	if (i < 26) {
	    sprintf(tmp, "/dev/sd%c", 'a' + i);
	} else {
	    sprintf(tmp, "/dev/sd%c%c", 'a' + (i / 26), 'a' + (i % 26));
	}
	if ((idlun2 = get_id_lun(tmp)) == -1) {
	    if (errno == EACCES) {
		eacces = 1;
	    }
	} else if (idlun == idlun2) {
	    if (pathcpy(path, tmp, len) == NULL) {
		return -1;
	    }
 	    return 0;
	}
    }
    errno = eacces ? EACCES : ENOENT;
    return -1;
}

static int get_sg_path(int idlun, char *path, size_t len, int host_id_only)
{
    char tmp[PATH_MAX];
    int i;
    int eacces;
    int idlun2;

    eacces = 0;
    /* BUG: only single usb fdd is supported for one host_id */
    if (host_id_only) {
	idlun = (idlun & 0xff) << 24;
    }
    for (i = 0; i < 256; i++) {
	sprintf(tmp, "/dev/sg%d", i);
	if ((idlun2 = get_id_lun(tmp)) == -1) {
	    if (errno == EACCES) {
		eacces = 1;
	    }
	} else if (idlun == idlun2) {
	    if (pathcpy(path, tmp, len) == NULL) {
		return -1;
	    }
 	    return 0;
	}
    }
    errno = eacces ? EACCES : ENOENT;
    return -1;
}

int convert_to_sg_path(const char *sd, char *sg, size_t len)
{
    int idlun;
    int flag;

    if ((flag = is_sg_device(sd)) < 0) {
	return -1;
    }
    if (flag) {
	if (pathcpy(sg, sd, len) == NULL) {
	    return -1;
	}
	return 0;
    }
    if ((flag = is_sd_device(sd)) < 0) {
	return -1;
    }
    if (!flag) {
	errno = ENODEV;
	return -1;
    }
    if ((idlun = get_id_lun(sd)) == -1) {
	return -1;
    }
    if (get_sg_path(idlun, sg, len, 0) < 0) {
	errno = -errno;
	return -1;
    }
    return 0;
}

int convert_to_sd_path(const char *sg, char *sd, size_t len)
{
    int idlun;
    int flag;

    if ((flag = is_sd_device(sg)) < 0) {
	return -1;
    }
    if (flag) {
	if (pathcpy(sd, sg, len) == NULL) {
	    return -1;
	}
	return 0;
    }
    if ((flag = is_sg_device(sg)) < 0) {
	return -1;
    }
    if (!flag) {
	errno = ENODEV;
	return -1;
    }
    if ((idlun = get_id_lun(sg)) == -1) {
	return -1;
    }
    if (get_sd_path(idlun, sd, len, 0) < 0) {
	errno = -errno;
	return -1;
    }
    return 0;
}

static int is_usb_fdd_for_host(int host_id)
{
    int i;
    char path[PATH_MAX];
    char buf[1024];
    FILE *fp;
    struct stat st;

    if (stat("/sys/class", &st) == 0) {
	int n;

	sprintf(path, "/sys/class/scsi_host/host%d/device/../driver", host_id);
	if ((n = readlink(path, buf, sizeof(buf))) < 12 || strncmp(buf + n - 12, "/usb-storage", 12) != 0) {
	    return 0;
	}

	sprintf(path, "/sys/class/scsi_host/host%d/device/../bInterfaceClass", host_id);
	if ((fp = fopen(path, "r")) == NULL) {
	    return 0;
	}
	if (fgets(buf, sizeof(buf), fp) == NULL || strcmp(buf, "08\n") != 0) {
	    fclose(fp);
	    return 0;
	}
	fclose(fp);

	sprintf(path, "/sys/class/scsi_host/host%d/device/../bInterfaceSubClass", host_id);
	if ((fp = fopen(path, "r")) == NULL) {
	    return 0;
	}
	if (fgets(buf, sizeof(buf), fp) == NULL || strcmp(buf, "04\n") != 0) {
	    fclose(fp);
	    return 0;
	}
	fclose(fp);

	return 1;
    }

    sprintf(path, "/proc/scsi/usb-storage/%d", host_id);
    if ((fp = fopen(path, "r")) == NULL) {
	for (i = 0; i < 256; i++) {
	    sprintf(path, "/proc/scsi/usb-storage-%d/%d", i, host_id);
	    if ((fp = fopen(path, "r")) != NULL) {
		break;
	    }
	}
	if (fp == NULL) {
	    return 0;
	}
    }
    while (fgets(buf, sizeof(buf), fp) != NULL) {
	if (strcmp(buf, "     Protocol: Uniform Floppy Interface (UFI)\n") == 0) {
	    fclose(fp);
	    return 1;
	}
    }
    fclose(fp);
    return 0;
}

int is_usb_fdd(const char *dev)
{
    int idlun;
    int is_sd;
    int is_sg;

    if ((is_sd = is_sd_device(dev)) < 0 || (is_sg = is_sg_device(dev)) < 0) {
	return -1;
    }
    if (!is_sd && !is_sg) {
	return 0;
    }
    if ((idlun = get_id_lun(dev)) == -1) {
	return -1;
    }
    return is_usb_fdd_for_host(HOST_ID(idlun));
}

int get_usb_fdds(struct ufi_path_info **info)
{
    int i;
    struct ufi_path_info *top = NULL;
    struct ufi_path_info *last = NULL;
    struct ufi_path_info *current;
    char sg_path[PATH_MAX];
    char sd_path[PATH_MAX];

    for (i = 0; i < 256; i++) {
	if (is_usb_fdd_for_host(i)) {
	    if (get_sg_path(i, sg_path, sizeof(sg_path), 1) < 0) {
		if (errno != ENOENT) {
		    free_ufi_path_info(top);
		    return -1;
		}
		sg_path[0] = '\0';
	    }
	    if (get_sd_path(i, sd_path, sizeof(sd_path), 1) < 0) {
		if (errno != ENOENT) {
		    free_ufi_path_info(top);
		    return -1;
		}
		sd_path[0] = '\0';
	    }
	    current = malloc(sizeof(struct ufi_path_info));
	    current->host_id = i;
	    current->sg_path = sg_path[0] != '\0' ? strdup(sg_path) : NULL;
	    current->sd_path = sd_path[0] != '\0' ? strdup(sd_path) : NULL;
	    current->next = NULL;
	    if (last != NULL) {
		last->next = current;
	    }
	    if (top == NULL) {
		top = current;
	    }
	    last = current;
	}
    }
    *info = top;
    return 0;
}

void free_ufi_path_info(struct ufi_path_info *info)
{
    while (info != NULL) {
	if (info->sg_path != NULL) {
	    free(info->sg_path);
	}
	if (info->sd_path != NULL) {
	    free(info->sd_path);
	}
	info = info->next;
    }
}
