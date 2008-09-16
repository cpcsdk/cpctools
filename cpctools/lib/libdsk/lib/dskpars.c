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

LDPUBLIC32 dsk_err_t LDPUBLIC16 dsk_set_forcehead(DSK_DRIVER *self, int force)
{
	if (!self) return DSK_ERR_BADPTR;
	return dsk_set_option(self, "HEAD", force);
}

LDPUBLIC32 dsk_err_t LDPUBLIC16 dsk_get_forcehead(DSK_DRIVER *self, int *force)
{
	if (!self) return DSK_ERR_BADPTR;
	return dsk_get_option(self, "HEAD", force);
}


LDPUBLIC32 dsk_err_t  LDPUBLIC16 dsk_set_option(DSK_PDRIVER self, const char *name, int value)
{
        DRV_CLASS *dc;
        if (!self || !name || !self->dr_class) return DSK_ERR_BADPTR;

        dc = self->dr_class;
	if (!dc->dc_option_set) return DSK_ERR_BADOPT;
	return (*dc->dc_option_set)(self, name, value);	
}


LDPUBLIC32 dsk_err_t  LDPUBLIC16 dsk_get_option(DSK_PDRIVER self, const char *name, int *value)
{
        DRV_CLASS *dc;
        if (!self || !name || !self->dr_class || !value) return DSK_ERR_BADPTR;

        dc = self->dr_class;
	if (!dc->dc_option_get) return DSK_ERR_BADOPT;
	return (*dc->dc_option_get)(self, name, value);	
}


/* If "index" is in range, returns the n'th option name in (*optname).
 *  * Else sets (*optname) to null. */
LDPUBLIC32 dsk_err_t  LDPUBLIC16 dsk_option_enum(DSK_PDRIVER self, int idx, char **name)
{
        DRV_CLASS *dc;
        if (!self || !name || !self->dr_class) return DSK_ERR_BADPTR;

        dc = self->dr_class;
	*name = NULL;
	if (!dc->dc_option_enum) return DSK_ERR_OK;
	return (*dc->dc_option_enum)(self, idx, name);	
}

