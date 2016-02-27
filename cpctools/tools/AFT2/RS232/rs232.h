/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/

/* Last revision: January 10, 2015 */

/* For more info and how to use this libray, visit: http://www.teuniz.net/RS-232/ */


#ifndef rs232_INCLUDED
#define rs232_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>



#ifndef _WIN32

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

typedef int HANDLE;

#else

#include <windows.h>

#endif

int RS232_OpenComport(const char *comport, int, const char *, HANDLE *handle);
int RS232_PollComport(HANDLE, unsigned char *, int);
int RS232_SendByte(HANDLE, unsigned char);
int RS232_SendBuf(HANDLE, unsigned char *, int);
void RS232_CloseComport(HANDLE);
void RS232_cputs(HANDLE, const char *);
int RS232_IsDCDEnabled(HANDLE);
int RS232_IsCTSEnabled(HANDLE);
int RS232_IsDSREnabled(HANDLE);
void RS232_enableDTR(HANDLE);
void RS232_disableDTR(HANDLE);
void RS232_enableRTS(HANDLE);
void RS232_disableRTS(HANDLE);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


