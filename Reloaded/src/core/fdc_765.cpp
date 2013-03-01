/**
 * Caprice32
 * fdc_765cpp
 * 
 * Copyright (C) 2008 Krusty/Benediction <krusty@cpcscene.com>
 *	    
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_LIB765_H

#include "fdc_765.h"
#include "cap32.h"
#include "config.h"
#include "debug.h"



static void gl_report_on(const char *progress)
{
	printf("+\n");
}

static void gl_report_off()
{
	printf("-\n");
}

/* Default error action is just to print a message to stderr */
static void
error_function(int debuglevel, char *fmt, va_list ap)
{
	/* Let's say default action is level 1; showing all messages
 * 	 * would be just too horribly disturbing. */
	
//	if (debuglevel > 1) return;	
//	fprintf( stderr, "lib765 level:%d error: ", debuglevel );
//	vfprintf( stderr, fmt, ap );
//	fprintf( stderr, "\n" );
}


t_FDC::t_FDC(shared_ptr<t_CPC> cpc) :
CPC(cpc)
{
	lib765_register_error_function(error_function);
	dsk_reportfunc_set(gl_report_on, gl_report_off);

	m_fdc = fdc_new() ;

	driveA.fdl = fd_newldsk() ;
	driveB.fdl = fd_newldsk() ;

	fd_settype (driveA.fdl, FD_30);
	fd_setheads(driveA.fdl, 1);
	fd_setcyls (driveA.fdl, 40);
	fd_setreadonly (driveA.fdl, 0);

	fd_settype (driveB.fdl, FD_30);
	fd_setheads(driveB.fdl, 1);
	fd_setcyls (driveB.fdl, 40);
	fd_setreadonly (driveA.fdl, 0);


	fdc_reset(m_fdc);
	fdc_setisr(m_fdc, NULL);	/* Not interrupt-driven */


	fdc_setdrive(m_fdc, 0, driveA.fdl);
	fdc_setdrive(m_fdc, 1, driveB.fdl);
	fdc_setdrive(m_fdc, 2, driveB.fdl);
	fdc_setdrive(m_fdc, 3, driveB.fdl);
}


t_FDC::~t_FDC()
{
	fdc_destroy(&m_fdc);
}


void t_FDC::Reset()
{
}

//TODO verify this function
unsigned char t_FDC::fdc_read_status(void)
{
	return ::fdc_read_ctrl(m_fdc);
}

unsigned char t_FDC::fdc_read_data(void)
{
	return  ::fdc_read_data(m_fdc) ;
}


void t_FDC::fdc_write_data(unsigned char val)
{
	::fdc_write_data(m_fdc, val);
}


void t_FDC::Emulate(int iCycleCount)
{
	for( int i=0 ; i< iCycleCount ; i++ )
	{
		::fdc_tick(m_fdc);
	}
}


void t_FDC::insertA(const string filename, const char *type )
{
	
	fdl_settype    (driveA.fdl, type);
 	fdl_setfilename(driveA.fdl, filename.c_str());

	int status = fd_drive_status(driveA.fdl);
	printf("<%s> inserted\n", filename.c_str());
	printf("Type : <%s>\n", fdl_gettype(driveA.fdl));
	printf("Status : <&%X>\n", fd_drive_status(driveA.fdl));
	printf("Sylinders : <%d>\n", fd_getcyls(driveA.fdl));

	if ( (status >> 3) & 1 )
	    printf("Double sided\n");
	else
	    printf("Single sided\n");
}


void t_FDC::insertB(const string filename, const char *type )
{
	fdl_setfilename(driveB.fdl, filename.c_str());
	fdl_settype    (driveB.fdl, type);

}

#endif
