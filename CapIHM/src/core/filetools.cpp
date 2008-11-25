/*
 *   $Id: filetools.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  



//
// Caprice general file tools
//

#include "filetools.h"
#include <sys/stat.h>
#include <string.h>

int file_size (int file_num)
{
	struct stat s;
	
	if (!fstat(file_num, &s)) {
		return s.st_size;
	} else {
		return 0;
	}
}

void splitPathFileName(char *pchCombined, char *pchPath, char *pchFile)
{
	char *pchPtr;
	
	pchPtr = strrchr(pchCombined, '/'); // start from the end and find the first path delimiter
	if (!pchPtr) {
		pchPtr = strrchr(pchCombined, '\\'); // try again with the alternate form
	}
	if (pchPtr) {
		pchPtr++; // advance the pointer to the next character
		if (pchFile) {
			strcpy(pchFile, pchPtr); // copy the filename
		}
		char chOld = *pchPtr;
		*pchPtr = 0; // cut off the filename part
		if (pchPath != pchCombined) {
			if (pchPath) {
				strcpy(pchPath, pchCombined); // copy the path
			}
			*pchPtr = chOld; // restore original string
		}
	} else {
		if (pchFile) {
			*pchFile = 0; // no filename found
		}
		if (pchPath != pchCombined) {
			if (pchPath) {
				strcpy(pchPath, pchCombined); // copy the path
			}
		}
	}
}
