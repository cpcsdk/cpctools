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
#include <getopt.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "ufi_detect.h"
#include "ufi_command.h"

#ifndef PACKAGE
#define PACKAGE "ufiformat"
#endif

static void list_devices(int verbose)
{
    struct ufi_path_info *path_info_p;

    if (verbose > 1) {
	printf("list usb fdd devices\n");
    }
    if (verbose) {
	printf("%-12s %-12s\n", "disk", "generic");
    }
    if (get_usb_fdds(&path_info_p) < 0) {
	goto error;
    }
    while (path_info_p != NULL) {
	if (path_info_p->sd_path != NULL || path_info_p->sg_path != NULL) {
	    printf("%-12s %-12s\n",
		   path_info_p->sd_path != NULL ? path_info_p->sd_path : "",
		   path_info_p->sg_path != NULL ? path_info_p->sg_path : "");
	} else {
	    if (verbose > 1) {
		printf("ignoreing unattached fdd: host_id = %d\n", path_info_p->host_id);
	    }
	}
	path_info_p = path_info_p->next;
    }
    return;

 error:
    perror("");
    exit(1);
}

static void show_capacity(const char *status, int blocks, int block_size)
{
    printf("%-12s %4d %4d %4d\n", status, blocks, block_size, blocks * block_size / 1024);
}

#define IS_2HD(size) ((size) > 1024 ? 1 : 0)

static void inquire(const char *path, int verbose, int force)
{
    char *sg = NULL;
    int fd = -1;
    struct ufi_product *product;
    struct ufi_capacities *capacities;
    int type;
    int wp;
    int is_fdd;

    if (verbose > 1) {
	printf("inquire on device=%s\n", path);
    }
    if (!force) {
	if ((is_fdd = is_usb_fdd(path)) < 0) {
	    goto error;
	}
	if (!is_fdd) {
	    fprintf(stderr, "%s: device is not usb fdd\n", path);
	    goto error2;
	}
	sg = malloc(PATH_MAX);
	if (convert_to_sg_path(path, sg, PATH_MAX)) {
	    if (verbose > 1 && errno == -ENOENT) {
		fprintf(stderr, "\"modprobe sg\" might be needed to prepare SCSI pass-thru device.\n");
	    }
	    sg = errno < 0 ? "/dev/sg* family" : NULL;
	    errno = errno < 0 ? -errno : errno;
	    goto error;
	}
    } else {
	sg = strdup(path);
    }
    if ((fd = open(sg, O_RDWR | O_NONBLOCK)) < 0) {
	goto error;
    }
    if (ufi_test_unit_ready(fd) == UFI_ERROR && !force) {
	goto error;
    }
    if ((capacities = ufi_read_format_capacities(fd, &type)) == NULL) {
	goto error;
    }
    if (type == UFI_UNFORMATTED_MEDIA || type == UFI_FORMATTED_MEDIA) {
	if (ufi_mode_sense(fd, &wp) != UFI_GOOD) {
	    goto error;
	}
    }
    if ((product = ufi_inquiry(fd)) == NULL) {
	goto error;
    }

    if (verbose) {
	printf("vendor:  %s\n", product->vendor);
	printf("product: %s\n", product->product);
	if (type == UFI_UNFORMATTED_MEDIA || type == UFI_FORMATTED_MEDIA) {
	    printf("write protect: %s\n", wp == UFI_PROTECTED ? "on" : "off");
	    printf("media type: %s\n", IS_2HD(capacities->blocks * capacities->block_size / 1024) ? "2HD" : "2DD");
	}
	printf("status      block size   kb\n");
    }
    switch (type) {
    case UFI_UNFORMATTED_MEDIA:
	show_capacity("unformatted", capacities->blocks, capacities->block_size);
	break;
    case UFI_FORMATTED_MEDIA:
	show_capacity("formatted", capacities->blocks, capacities->block_size);
	while (capacities->next != NULL) {
	    capacities = capacities->next;
	    show_capacity("formattable", capacities->blocks, capacities->block_size);
	}
	break;
    case UFI_NO_MEDIA:
	show_capacity("no", capacities->blocks, capacities->block_size);
	break;
    default:
	errno = EPROTO;
	goto error;
    }
    close(fd);
    return;

 error:
    perror(sg == NULL ? path : sg);
 error2:
    if (fd >= 0) {
	close(fd);
    }
    exit(1);
}

static const int geometry[][5] = {
    { 1440, 80, 2, 18,  512 },
    { 1232, 77, 2,  8, 1024 },
    { 1200, 80, 2, 15,  512 },
    {  720, 80, 2,  9,  512 },
    {  640, 80, 2,  8,  512 },
    { 0, 0, 0, 0 }
};

static int size_to_geometry(int size, int *track, int *head, int *sector, int *block)
{
    int i;

    for (i = 0; geometry[i][0] != 0; i++) {
	if (geometry[i][0] == size) {
	    *track = geometry[i][1];
	    *head = geometry[i][2];
	    *sector = geometry[i][3];
	    *block = geometry[i][4];
	    return 0;
	}
    }
    return 1;
}

static void format(const char *path, int size, int verbose, int force)
{
    char *sg = NULL;
    int fd = -1;
    int wp;
    struct ufi_capacities *capacities;
    int type;
    int track, head, sector, block, t, h;
    int mount_flags;
    int is_fdd;

    if (verbose > 1) {
	printf("format on device=%s, size=%d\n", path, size);
    }
    if (!force) {
	if ((is_fdd = is_usb_fdd(path)) < 0) {
	    goto error;
	}
	if (!is_fdd) {
	    fprintf(stderr, "%s: device is not usb fdd\n", path);
	    goto error2;
	}
	sg = malloc(PATH_MAX);
	if (convert_to_sd_path(path, sg, PATH_MAX) < 0) {
	    sg = errno < 0 ? "/dev/sd* family" : NULL;
	    errno = errno < 0 ? -errno : errno;
	    goto error;
	}
	if (convert_to_sg_path(path, sg, PATH_MAX) < 0) {
	    if (verbose > 1 && errno == -ENOENT) {
		fprintf(stderr, "\"modprobe sg\" might be needed to prepare SCSI pass-thru device.\n");
	    }
	    sg = errno < 0 ? "/dev/sg* family" : NULL;
	    errno = errno < 0 ? -errno : errno;
	    goto error;
	}
    } else {
	sg = strdup(path);
    }
    if ((fd = open(sg, O_RDWR | O_NONBLOCK)) < 0) {
	goto error;
    }
    if ((type = ufi_test_unit_ready(fd)) == UFI_ERROR && !force) {
	goto error;
    }
    if (!force) {
	if (type == UFI_NO_MEDIA) {
	    fprintf(stderr, "%s: no media\n", path);
	    goto error2;
	}
	if (ufi_mode_sense(fd, &wp) != UFI_GOOD) {
	    goto error;
	}
	if (wp == UFI_PROTECTED) {
	    fprintf(stderr, "%s: write protected\n", path);
	    goto error2;
	}
    }
    if (size == 0) {
	if ((capacities = ufi_read_format_capacities(fd, &type)) == NULL) {
	    goto error;
	}
	size = (capacities->blocks * capacities->block_size) / 1024;
	if (size_to_geometry(size, &track, &head, &sector, &block) != 0 || block != capacities->block_size) {
	    fprintf(stderr, "unable to figure out geometry for current media\n");
	    goto error2;
	}
    } else {
	if (size_to_geometry(size, &track, &head, &sector, &block) != 0) {
	    fprintf(stderr, "unable to figure out geometry for size=%d\n", size);
	    goto error2;
	}
	if (!force) {
	    if ((capacities = ufi_read_format_capacities(fd, &type)) == NULL) {
		goto error;
	    }
	    if (type == UFI_NO_MEDIA) {
		fprintf(stderr, "%s: no media\n", path);
		goto error2;
	    }
	    if (IS_2HD(capacities->blocks * capacities->block_size / 1024) != IS_2HD(size)) {
		fprintf(stderr, "%s: media type mismatch\n", path);
		goto error2;
	    }
	}
    }
    if (verbose) {
	printf("geometry: track=%d, head=%d, sector=%d, block=%d\n", track, head, sector, block);
    }
    for (t = 0; t < track; t++) {
	for (h = 0; h < head; h++) {
	    if (verbose) {
	        printf("formatting track=%d, head=%d\r", t, h);
		fflush(stdout);
	    }
	    if (ufi_format_unit(fd, track * head * sector, block, t, h) != UFI_GOOD) {
		goto error;
	    }
	}
    }
    printf("done                                   \n");
    close(fd);
    return;

 error:
    perror(sg == NULL ? path : sg);
 error2:
    if (fd >= 0) {
	close(fd);
    }
    exit(1);
}

static void usage(int exit_code)
{
    fputs(
	 "Usage: " PACKAGE " [OPTION]... [DEVICE]\n"
	 "Format a floppy disk in a USB floppy disk DEVICE.\n"
	 "\n"
	 "  -f, --format [SIZE]  specify format capacity SIZE in KB\n"
	 "                       without -f option, the format of the current media will be used\n"
	 "  -F, --force          do not perform any safety checks\n"
	 "  -i, --inquire        show device information, instead of performing format\n"
	 "                       without DEVICE argument, list USB floppy disk devices\n"
	 "  -v, --verbose        show detailed output\n"
	 "  -q, --quiet          suppress minor output\n"
	 "  -h, --help           show this message\n"
	 "\n",
	 exit_code == 0 ? stdout : stderr);
    exit(exit_code);
}

static const struct option long_options[] = {
    { "inquire", no_argument,       0, 'i' },
    { "format",  required_argument, 0, 'f' },
    { "force",   no_argument,       0, 'F' },
    { "verbose", no_argument,       0, 'v' },
    { "quiet",   no_argument,       0, 'q' },
    { "help",    no_argument,       0, 'h' },
    { 0, 0, 0, 0 }
};

static const char short_options[] = "if:Fvqh";

int main(int argc, char **argv)
{
    int c;
    int opt_inquire = 0;
    int opt_format = 0;
    int opt_force = 0;
    int opt_verbose = 1;
    int format_size = 0;
    char tmp[256];

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (c) {
        case 'i':
	    opt_inquire = 1;
            break;
        case 'f':
	    opt_format = 1;
	    format_size = atoi(optarg);
	    sprintf(tmp, "%d", format_size);
	    if (strcmp(optarg, tmp) != 0 || format_size < 0) {
		printf("format parameter should be numeric\n");
		usage(1);
	    }
            break;
        case 'F':
	    opt_force = 1;
            break;
        case 'v':
	    opt_verbose = 2;
            break;
        case 'q':
	    opt_verbose = 0;
            break;
        case 'h':
	    usage(0);
	    break;
        case '?':
        default:
	    usage(1);
        }
    }

    if (!opt_inquire && !opt_format) {
	opt_format = 1;
	format_size = 0;
    }
    if (opt_format && opt_inquire) {
	fprintf(stderr, "%s: both inquire and format options specified\n", argv[0]);
	usage(1);
    } else if ((opt_format || opt_inquire) && argc - optind > 1) {
	fprintf(stderr, "%s: two or more devices specified\n", argv[0]);
	usage(1);
    } else if (opt_format && argc == optind) {
	fprintf(stderr, "%s: no device specified\n", argv[0]);
	usage(1);
    }

    ufi_set_verbose(opt_verbose);
    if (opt_inquire) {
	if (argc == optind) {
	    list_devices(opt_verbose);
	} else {
	    inquire(argv[optind], opt_verbose, opt_force);
	}
    } else if (opt_format) {
	format(argv[optind], format_size, opt_verbose, opt_force);
    }
    exit(0);
}
