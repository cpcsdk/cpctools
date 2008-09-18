//
// Caprice32 DSK manager
//

#ifndef HAVE_LIB765_H


#include "fdc.h"

#ifndef _DSK_H_
#define _DSK_H_

// FDC constants
#define DSK_BPTMAX      8192
#define DSK_TRACKMAX    102   // max amount that fits in a DSK header
#define DSK_SIDEMAX     2
#define DSK_SECTORMAX   29    // max amount that fits in a track header

#define MAX_DISK_FORMAT 8
#define DEFAULT_DISK_FORMAT 0
#define FIRST_CUSTOM_DISK_FORMAT 2

typedef struct {
   char id[34];
   char unused1[14];
   unsigned char tracks;
   unsigned char sides;
   unsigned char unused2[2];
   unsigned char track_size[DSK_TRACKMAX*DSK_SIDEMAX];
} t_DSK_header;

typedef struct {
   char id[12];
   char unused1[4];
   unsigned char track;
   unsigned char side;
   unsigned char unused2[2];
   unsigned char bps;
   unsigned char sectors;
   unsigned char gap3;
   unsigned char filler;
   unsigned char sector[DSK_SECTORMAX][8];
} t_track_header;

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
int dsk_load (char *pchFileName, t_drive *drive, char chID);
int dsk_save (char *pchFileName, t_drive *drive, char chID);
int dsk_format (t_drive *drive, int iFormat);

#endif

#endif
