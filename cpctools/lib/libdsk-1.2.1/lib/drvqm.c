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

/*
 *  This driver is for CopyQM images
 *  Written by Per Ola Ingvarsson.
 *  Thanks to Roger Plant for the expandqm program which has helped a lot.
 *  Currently it is read only
 */

#include <stdio.h>
#include "libdsk.h"
#include "drvi.h"
#include "drvqm.h"
#include "crctable.h"

#undef DRV_QM_DEBUG

dsk_err_t drv_qm_open(DSK_DRIVER *self, const char *filename);
dsk_err_t drv_qm_close(DSK_DRIVER *self);
dsk_err_t drv_qm_read(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                      void *buf, dsk_pcyl_t cylinder,
                      dsk_phead_t head, dsk_psect_t sector);
dsk_err_t drv_qm_xseek(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                       dsk_pcyl_t cylinder, dsk_phead_t head);
dsk_err_t drv_qm_status(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                        dsk_phead_t head, unsigned char *result);
dsk_err_t drv_qm_getgeom(DSK_DRIVER *self, DSK_GEOMETRY *geom);

dsk_err_t drv_qm_secid(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
                      dsk_pcyl_t cylinder, dsk_phead_t head, 
                      DSK_FORMAT *result);


#define MAKE_CHECK_SELF QM_DSK_DRIVER* qm_self;\
                        if ( self->dr_class != &dc_qm ) return DSK_ERR_BADPTR;\
                        qm_self = (QM_DSK_DRIVER*)self;

#define QM_HEADER_SIZE 133


static int get_i16( char* buf, int pos )
{
    unsigned char low_byte;
    unsigned char high_byte;
    unsigned int outInt;
    
    low_byte = buf[pos++];
    high_byte = buf[pos];
    
    /* Signed, eh. Lets see. */
    outInt = 0;
    if ( (signed char)high_byte < 0 ) {
        /* Set all to ones except for the lower 16 */
        /* Should work if sizeof(int) >= 16 */
        outInt = (-1) << 16;
    }

    outInt |= (high_byte << 8 ) | low_byte;
    
    return outInt;
}

static unsigned int get_u16( char* buf, int pos )
{
    return ((unsigned int)get_i16( buf, pos )) & 0xffff;
}

static unsigned long get_u32( char* buf, int pos )
{
    int i;
    unsigned long ret_val = 0;
    for ( i = 3; i >= 0; --i ) {
        ret_val <<= 8;
        ret_val |= ( (unsigned long)buf[pos+i] & 0xff );
    }
    return ret_val;
}


/* This struct contains function pointers to the driver's functions, and the
 * size of its DSK_DRIVER subclass */
DRV_CLASS dc_qm =
{
    sizeof(QM_DSK_DRIVER),
    "qm",
    "CopyQM file driver",
    drv_qm_open,     /* open */
    NULL,            /* create new */
    drv_qm_close,    /* close */
    drv_qm_read,     /* read sector */
    NULL,            /* write sector */
    NULL,            /* format track, physical */
    drv_qm_getgeom,  /* get geometry */
    drv_qm_secid,    /* Sector ID */
    drv_qm_xseek,    /* seek to track */
    drv_qm_status,   /* drive status */
    NULL,            /* file name */
};

dsk_err_t drv_qm_load_header( QM_DSK_DRIVER* qm_self, char* header ) 
{
    int i;
    
    /* Check the header checksum */
    unsigned char chksum = 0;
    for ( i = 0; i < QM_HEADER_SIZE; ++i ) {
        chksum += (unsigned char)(header[i]);
    }
    if ( chksum != 0 ) {
#ifdef DRV_QM_DEBUG
        fprintf( stderr, "qm: header checksum error\n");
#endif
        return DSK_ERR_NOTME;
    } else {
#ifdef DRV_QM_DEBUG
        fprintf( stderr, "qm: header checksum is ok!\n");
#endif
    }
    if ( header[0] != 'C' || header[1] != 'Q' ) {
#ifdef DRV_QM_DEBUG
        fprintf( stderr, "qm: First two chars are %c%c\n", header[0],
                 header[1] );
#endif
        return DSK_ERR_NOTME;
    }
    /* I'm guessing sector size is at 3. Expandqm thinks 7 */
    qm_self->qm_h_sector_size = get_u16( header, 0x03 );
    /* Number of sectors 0x0B-0x0C, strange number for non-blind, often 116 */
    qm_self->qm_h_nbr_sectors = get_u16( header, 0x0b );
    /* Number of sectors per track */
    qm_self->qm_h_nbr_sec_per_track = get_u16( header, 0x10 );
    /* Number of heads */
    qm_self->qm_h_nbr_heads = get_u16( header, 0x12 );
    /* Blind or not */
    qm_self->qm_h_blind = header[0x58];
    /* Density - 0 is DD, 1 means HD */
    qm_self->qm_h_density = header[0x59];    
    /* Number of used tracks */
    qm_self->qm_h_used_tracks = header[0x5a];
    /* Number of total tracks */
    qm_self->qm_h_total_tracks = header[0x5b];
    /* CRC 0x5c - 0x5f */
    qm_self->qm_h_crc = get_u32( header, 0x5c );
    /* Length of comment */
    qm_self->qm_h_comment_len = get_u16( header, 0x6f );
    /* 0x71 is first sector number - 1 */
    qm_self->qm_h_secbase = (signed char)(header[0x71]);
    /* 0x74 is interleave, I think. Normally 1, but 0 for old copyqm */
    qm_self->qm_h_interleave = header[0x74];
    /* 0x75 is skew. Normally 0. Negative number for alternating sides */
    qm_self->qm_h_skew = header[0x75];

#ifdef DRV_QM_DEBUG
    fprintf( stderr, "qm: sector size is %d\n", qm_self->qm_h_sector_size);
    fprintf( stderr, "qm: nbr sectors %d\n",
             qm_self->qm_h_nbr_sectors );
    fprintf( stderr, "qm: nbr sectors/track %d\n",
             qm_self->qm_h_nbr_sec_per_track);
    fprintf( stderr, "qm: nbr heads %d\n",
             qm_self->qm_h_nbr_heads );
    fprintf( stderr, "qm: secbase %d\n", qm_self->qm_h_secbase );
    fprintf( stderr, "qm: density %d\n", qm_self->qm_h_density );
    fprintf( stderr, "qm: used tracks %d\n", qm_self->qm_h_used_tracks );
    fprintf( stderr, "qm: CRC 0x%08lx\n", qm_self->qm_h_crc );
    fprintf( stderr, "qm: interleave %d\n", qm_self->qm_h_interleave );
    fprintf( stderr, "qm: skew %d\n", qm_self->qm_h_skew );
    fprintf( stderr, "qm: description \"%s\"\n" , header + 0x1c );
#endif

    /* Fix the interleave value for old versions of CopyQM */
    if ( qm_self->qm_h_interleave == 0 ) {
        qm_self->qm_h_interleave = 1;
    }
    
    return DSK_ERR_OK;
}

static void drv_qm_update_crc( unsigned long* crc, unsigned char byte )
{
    /* Note that there is a bug in the CopyQM CRC calculation  */
    /* When indexing in this table, they shift the crc ^ data  */
    /* 2 bits up to address longwords, but they do that in an  */
    /* eight bit register, so that the top 2 bits are lost,    */
    /* thus the anding with 0x3f                               */
    *crc = crc32r_table[(byte ^ (unsigned char)*crc) & 0x3f ] ^ (*crc >> 8);
}

dsk_err_t drv_qm_load_image( QM_DSK_DRIVER* qm_self, FILE* fp )
{
    unsigned char* image = 0;
    dsk_err_t errcond = DSK_ERR_OK;
    /* Write position in the memory image */
    size_t curwritepos = 0;
    /* FIXME: Use the used tracks instead of the total tracks to detect */
    /*        that there is the correct amount of data in the image     */
    size_t image_size = (size_t)qm_self->qm_h_nbr_sec_per_track *
        (size_t)qm_self->qm_h_nbr_heads *
        (size_t)qm_self->qm_h_total_tracks * (size_t)qm_self->qm_h_sector_size;
    /* Set the position after the header and comment */
    int res = fseek( fp, QM_HEADER_SIZE + qm_self->qm_h_comment_len, SEEK_SET );
    if ( res < 0 ) return DSK_ERR_NOTME;

    /* Alloc memory for the image */
    qm_self->qm_image = dsk_malloc( image_size );
    if ( ! qm_self->qm_image ) return DSK_ERR_NOMEM;
    image = qm_self->qm_image;
    /* Start reading */
    /* Note that it seems like each track starts a new block */
    while ( curwritepos < image_size ) {
        /* Read the length */
        char lengthBuf[2];
        res = fread( lengthBuf, 2, 1, fp );
        if ( res != 1 ) {
            if ( feof( fp ) ) {
                /* End of file - fill with f6 - do not update CRC for these */
                memset( image + curwritepos, 0xf6, image_size - curwritepos );
                curwritepos += image_size - curwritepos;
            } else {
                return DSK_ERR_NOTME;
            }
        } else {
            int length = get_i16( lengthBuf, 0 );
            if ( length < 0 ) {            
                /* Negative number - next byte is repeated (-length) times */
                int c = fgetc( fp );
                if ( c == EOF ) return DSK_ERR_NOTME;
                /* Copy the byte into memory and update the offset */
                memset( image + curwritepos, c, -length );                
                curwritepos -= length;
                /* Update CRC */
                for( ; length != 0; ++length ) {
                    drv_qm_update_crc( &qm_self->qm_calc_crc, (unsigned char)c );
                }
            } else {
                if ( length != 0 ) {
                    /* Positive number - length different characters */
                    res = fread( image + curwritepos, length, 1, fp );
                    /* Update CRC (and write pos) */
                    while ( length-- ) {
                        drv_qm_update_crc( &qm_self->qm_calc_crc,
                                           image[curwritepos++] );
                    }
                    if ( res != 1 ) return DSK_ERR_NOTME;
                }
            }
        }
    }
#ifdef DRV_QM_DEBUG    
    fprintf( stderr, "drv_qm_load_image - crc from header = 0x%08lx, "
             "calc = 0x%08lx\n", qm_self->qm_h_crc, qm_self->qm_calc_crc );
#endif
    /* Compare the CRCs */
    /* The CRC is zero on old images so it cannot be checked then */
    if ( qm_self->qm_h_crc ) {
        if ( qm_self->qm_h_crc != qm_self->qm_calc_crc ) {
            return DSK_ERR_NOTME;
        }
    }
    
    /* Write image to file for testing */
    if ( 0 ) {
        FILE* tmpFile = fopen("/tmp/tmpfile.dd", "w" );
        fwrite( image, image_size, 1, tmpFile );
        fclose( tmpFile );
    }
    return errcond;
}

dsk_err_t drv_qm_open(DSK_DRIVER *self, const char *filename)
{
    FILE* fp;
    char header[QM_HEADER_SIZE];
    size_t res;
    dsk_err_t errcond = DSK_ERR_OK;

    /* Create self pointer or return if wrong type */
    MAKE_CHECK_SELF;

    /* Zero some stuff */
    qm_self->qm_image = 0;
    qm_self->qm_filename = 0;
    
    /* Open file. Read only for now */
    fp = fopen(filename, "rb");
    if ( ! fp ) {
        return DSK_ERR_NOTME;
    }

    /* Keep a copy of the filename for future writing use */
    qm_self->qm_filename = (char*)dsk_malloc( 1 + strlen( filename ) );
    if ( qm_self->qm_filename == 0 ) {        
        return DSK_ERR_NOMEM;
    }
    strcpy( qm_self->qm_filename, filename );

    /* Load the header */
    res = fread( header, QM_HEADER_SIZE, 1, fp );
    if ( res != 1 ) {
        errcond = DSK_ERR_NOTME;
        goto end;
    }
    
    /* Load the header */
    errcond = drv_qm_load_header( qm_self, header );

    /* If there's a comment, allocate a temporary buffer for it and load it. */
    if (errcond == DSK_ERR_OK && qm_self->qm_h_comment_len)
    {
        char *comment_buf = dsk_malloc(1 + qm_self->qm_h_comment_len);
        /* If malloc fails, ignore it - comments aren't essential */
        if (comment_buf)
        {
            int res = fseek( fp, QM_HEADER_SIZE, SEEK_SET );
            if ( res < 0 ) errcond = DSK_ERR_NOTME;
            else 
            {
                res = fread(comment_buf, 1, qm_self->qm_h_comment_len, fp);
                if ( res < qm_self->qm_h_comment_len) errcond = DSK_ERR_NOTME;
                else
                {
                    comment_buf[qm_self->qm_h_comment_len] = 0;
                    dsk_set_comment(&qm_self->qm_super, comment_buf);
                    dsk_free(comment_buf);
                }
            }
        }
    }

    /* Load the rest */
    if ( errcond == DSK_ERR_OK ) {
        errcond = drv_qm_load_image( qm_self, fp );
        if ( errcond != DSK_ERR_OK ) {
#ifdef DRV_QM_DEBUG
            fprintf( stderr, "drv_qm_load_image returned %d\n", (int)errcond );
#endif
        }
    }
    
    /* Return errcond */
  end:
    /* Close the file */
    if ( fp ) {
        fclose( fp );
    }
    return errcond;
}

dsk_err_t drv_qm_close(DSK_DRIVER *self)
{
    QM_DSK_DRIVER* qmself;
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_close\n");
#endif
    if ( self->dr_class != &dc_qm ) return DSK_ERR_BADPTR;
    qmself = (QM_DSK_DRIVER*)self;

    /* Delete the file name */
    dsk_free( qmself->qm_filename );
    dsk_free( qmself->qm_image );
    return DSK_ERR_OK;
}

dsk_err_t drv_qm_read(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                      void *buf, dsk_pcyl_t cylinder,
                      dsk_phead_t head, dsk_psect_t sector)
{
    long offset;
    QM_DSK_DRIVER* qm_self = (QM_DSK_DRIVER*)self;
    if (!buf || !self || !geom || self->dr_class != &dc_qm)
        return DSK_ERR_BADPTR;
    
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_read\n");
#endif
    
    offset = (cylinder * geom->dg_heads) + head;        /* Drive track */
    offset *= geom->dg_sectors;
    offset += (sector - ((qm_self->qm_h_secbase + 1) & 0xFF));
    offset *=  geom->dg_secsize;

    memcpy( buf, qm_self->qm_image + offset, geom->dg_secsize );
    return DSK_ERR_OK;
}


dsk_err_t drv_qm_secid(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
                      dsk_pcyl_t cylinder, dsk_phead_t head, 
                      DSK_FORMAT *result)
{
    QM_DSK_DRIVER* qm_self = (QM_DSK_DRIVER*)self;
    if (!self || !geom || self->dr_class != &dc_qm)
        return DSK_ERR_BADPTR;
    
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_secid\n");
#endif
    if (cylinder >= (dsk_pcyl_t)qm_self->qm_h_total_tracks || 
        head     >= (dsk_phead_t)qm_self->qm_h_nbr_heads) 
    {
        return DSK_ERR_NOADDR;
    }
    if (result)
    {
        result->fmt_cylinder = cylinder;
        result->fmt_head     = head;
        result->fmt_sector   = ((qm_self->qm_sector++) 
                                % qm_self->qm_h_nbr_sec_per_track)
                                + ((qm_self->qm_h_secbase + 1) & 0xFF);
        result->fmt_secsize  = qm_self->qm_h_sector_size;
    }
    return DSK_ERR_OK;
}

dsk_err_t drv_qm_xseek(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                       dsk_pcyl_t cylinder, dsk_phead_t head)
{
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_xseek\n");
#endif
    return DSK_ERR_NOTME;
}

dsk_err_t drv_qm_status(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                        dsk_phead_t head, unsigned char *result)
{
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_status\n");
#endif
    return DSK_ERR_NOTME;
}

dsk_err_t drv_qm_getgeom(DSK_DRIVER *self, DSK_GEOMETRY *geom)
{
    dsk_err_t err;
    QM_DSK_DRIVER* qm_self = (QM_DSK_DRIVER*)self;
#ifdef DRV_QM_DEBUG
    fprintf(stderr, "drv_qm_getgeom\n");
#endif    
    if (!geom || !self || self->dr_class != &dc_qm) return DSK_ERR_BADPTR;

    /* Start by calling the standard guesser */
    err = dsk_defgetgeom(self, geom);
/*
    dc_qm.dc_getgeom = NULL;
    err = dsk_getgeom(self, geom);
    dc_qm.dc_getgeom = drv_qm_getgeom; */
    if (err != DSK_ERR_OK && err != DSK_ERR_BADFMT) return err;

    if (err != DSK_ERR_OK) geom->dg_sidedness = SIDES_ALT;
    /* Now override with the geometry that CopyQM knows about */
    geom->dg_cylinders = qm_self->qm_h_total_tracks;
    geom->dg_heads     = qm_self->qm_h_nbr_heads;
    geom->dg_sectors   = qm_self->qm_h_nbr_sec_per_track;
    geom->dg_secbase   = (qm_self->qm_h_secbase + 1) & 0xFF;
    geom->dg_secsize   = qm_self->qm_h_sector_size;
    /* Translate the datarate */
    switch ( qm_self->qm_h_density ) {
       case 0:
           geom->dg_datarate = RATE_DD;
           break;
       case 1:
           geom->dg_datarate = RATE_HD;
         break;
       default:
           geom->dg_datarate = RATE_ED;
           break;
    }
    /* Took these from drvwin32.c */
    switch(geom->dg_sectors) {
       case 8:  geom->dg_rwgap = 0x2A; geom->dg_fmtgap = 0x50; break;
       case 9:  geom->dg_rwgap = 0x2A; geom->dg_fmtgap = 0x52; break;
       case 10: geom->dg_rwgap = 0x0C; geom->dg_fmtgap = 0x17; break;
       case 15: geom->dg_rwgap = 0x1B; geom->dg_fmtgap = 0x50; break;
       case 18: geom->dg_rwgap = 0x1B; geom->dg_fmtgap = 0x50; break;
       default: geom->dg_rwgap = 0x2A; geom->dg_fmtgap = 0x52; break;
    }

    /* FIXME: What to do with interleave and skew? */
    
    geom->dg_fm        = 0;
    geom->dg_nomulti   = 0;
    return DSK_ERR_OK;
}
