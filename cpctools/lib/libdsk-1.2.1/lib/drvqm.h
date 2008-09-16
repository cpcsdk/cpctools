/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001-2,2005  John Elliott <jce@seasip.demon.co.uk>     *
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

/* Declarations for the CopyQM driver */

typedef struct
{
    DSK_DRIVER    qm_super;
    char*         qm_filename;
    int           qm_h_sector_size;
    /* Number of total sectors. Not valid if blind */
    int           qm_h_nbr_sectors;
    int           qm_h_nbr_sec_per_track;
    int           qm_h_nbr_heads;
    int           qm_h_comment_len;
    /* Density - 1 means HD, 2 means QD */
    int           qm_h_density;
    /* Blind transfer or not. */
    int           qm_h_blind;
    int           qm_h_used_tracks;
    int           qm_h_total_tracks;
    /* Interleave */
    int           qm_h_interleave;
    /* Skew. Negative number for skew between sides */
    int           qm_h_skew;
    /* Sector number base. */
    signed char   qm_h_secbase;
    /* The crc read from the header */
    unsigned long qm_h_crc;
    /* The crc calculated while the image is read */
    unsigned long qm_calc_crc;
    unsigned int  qm_image_offset;
    unsigned char* qm_image;
    /* Fake sector for READ ID command */
    unsigned int  qm_sector;
} QM_DSK_DRIVER;
