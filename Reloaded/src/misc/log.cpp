/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2010  cpcsdk crew

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "log.h"

Log* Log::instance = NULL;

void Log::errLogMsg(const char *format, va_list args)
{
    vfprintf(stderr,format,args);
    fprintf(stderr,"\n");
    fflush(stderr);
}

void Log::infoLogMsg(const char *format, va_list args)
{
    vfprintf(stdout,format,args);
    fprintf(stdout,"\n");
    fflush(stdout);
}
