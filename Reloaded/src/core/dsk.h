//
// Caprice32 DSK manager
//

#ifndef HAVE_LIB765_H

#include "fdc_old.h"

#ifndef _DSK_H_
#define _DSK_H_

// FDC constants
#define DSK_BPTMAX      8192
#define DSK_TRACKMAX    102   // max amount that fits in a DSK header
#define DSK_SIDEMAX     2
#define DSK_SECTORMAX   64    // max amount that fits in a track header

#define MAX_DISK_FORMAT 8
#define DEFAULT_DISK_FORMAT 0
#define FIRST_CUSTOM_DISK_FORMAT 2

// Disk information block
typedef struct {
   char id[34]; // "EXTENDED CPC DSK File\r\nDisk-Info\r\n
   char unused1[14]; // name of creator
   unsigned char tracks; // number of tracks
   unsigned char sides; // number of sides
   unsigned char unused2[2]; // unused
   unsigned char track_size[DSK_TRACKMAX*DSK_SIDEMAX]; // track size table
} t_DSK_header;

typedef struct {
   char id[12]; // "Track-Info\r\n"
   char unused1[4]; // unused
   unsigned char track; // track number
   unsigned char side; // side number
   unsigned char unused2[2]; // EXT: data rate + rec. mode
   unsigned char bps; // sector size
   unsigned char sectors; // number of sectors
   unsigned char gap3; // gap#3 length
   unsigned char filler; // filler byte
   unsigned char sector[DSK_SECTORMAX][8]; // sector info list
} t_track_header;

// ... sector data ??!
// EXT : store full sector size for N=6 and N=7
// EXT : store multiple copies for weak sectors
// (if size(N) != specified size > multiple copies)

typedef struct {
   unsigned char label[40]; // label to display in options dialog
   unsigned int tracks; // number of tracks
   unsigned int sides; // number of sides
   unsigned int sectors; // sectors per track
   unsigned int sector_size; // sector size as N value
   unsigned int gap3_length; // GAP#3 size
   unsigned char filler_byte; // default byte to use
   unsigned char sector_ids[2][16]; // sector IDs
} t_disk_format;

void dsk_eject (t_drive *drive);
int dsk_load (const char *pchFileName, t_drive *drive, char chID);
int dsk_save (const char *pchFileName, t_drive *drive);
int dsk_format (t_drive *drive, int iFormat);

#endif

#endif
