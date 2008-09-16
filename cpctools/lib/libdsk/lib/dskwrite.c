/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001  John Elliott <jce@seasip.demon.co.uk>            *
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

#include "drvi.h"
#include "compi.h"

LDPUBLIC32 dsk_err_t LDPUBLIC16 dsk_pwrite(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                              const void *buf, dsk_pcyl_t cylinder,
                              dsk_phead_t head, dsk_psect_t sector)
{
    DRV_CLASS *dc;
    dsk_err_t e = DSK_ERR_UNKNOWN;
    unsigned n;

    if (!self || !geom || !buf || !self->dr_class) return DSK_ERR_BADPTR;

    dc = self->dr_class;

    if (self && self->dr_compress && self->dr_compress->cd_readonly)
        return DSK_ERR_RDONLY;

    if (!dc->dc_write) return DSK_ERR_NOTIMPL;
    for (n = 0; n < self->dr_retry_count; n++)
    {
        e = (dc->dc_write)(self,geom,buf,cylinder,head,sector); 
        if (e == DSK_ERR_OK) self->dr_dirty = 1;
        if (!DSK_TRANSIENT_ERROR(e)) return e;
    }
    return e;
}


LDPUBLIC32 dsk_err_t LDPUBLIC16 dsk_lwrite(DSK_DRIVER *self, const DSK_GEOMETRY *geom,
                              const void *buf, dsk_lsect_t sector)
{
        dsk_pcyl_t  c;
        dsk_phead_t h;
        dsk_psect_t s;
        dsk_err_t e;

        if (self && self->dr_compress && self->dr_compress->cd_readonly)
                return DSK_ERR_RDONLY;
 
        e = dg_ls2ps(geom, sector, &c, &h, &s);
        if (e != DSK_ERR_OK) return e;
        e = dsk_pwrite(self, geom, buf, c, h, s);
    if (e == DSK_ERR_OK) self->dr_dirty = 1;
    return e;
}


LDPUBLIC32 dsk_err_t LDPUBLIC16 dsk_xwrite(DSK_DRIVER *self, const DSK_GEOMETRY *geom, 
            const void *buf, 
                        dsk_pcyl_t cylinder,   dsk_phead_t head,
                        dsk_pcyl_t cyl_expect, dsk_phead_t head_expect,
                        dsk_psect_t sector, size_t sector_len, int deleted)
{
        DRV_CLASS *dc;
    dsk_err_t err = DSK_ERR_UNKNOWN;
    unsigned n;

        if (!self || !geom || !buf || !self->dr_class) return DSK_ERR_BADPTR;

        dc = self->dr_class;

        if (self && self->dr_compress && self->dr_compress->cd_readonly)
                return DSK_ERR_RDONLY;

        if (!dc->dc_xwrite) return DSK_ERR_NOTIMPL;
    for (n = 0; n < self->dr_retry_count; n++)
    {
            err = (dc->dc_xwrite)(self,geom,buf,cylinder,head, cyl_expect, 
                head_expect, sector, sector_len, deleted);
        if (err == DSK_ERR_OK) self->dr_dirty = 1;
        if (!DSK_TRANSIENT_ERROR(err)) return err;
    }
    return err;
}
                                                                                        
