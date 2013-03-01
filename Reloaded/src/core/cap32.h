/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2005 Ulrich Doewich

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

#ifndef _CAP32_H_
#define _CAP32_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _MAX_PATH
 #ifdef _POSIX_PATH_MAX
 #define _MAX_PATH _POSIX_PATH_MAX
 #else
 #define _MAX_PATH 256
 #endif
#endif

#define CYCLE_COUNT_INIT 80000 // 4MHz divided by 50Hz = number of CPU cycles per frame

// Multiface 2 flags
#define MF2_ACTIVE      1
#define MF2_RUNNING     2
#define MF2_INVISIBLE   4

#endif
