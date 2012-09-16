/*
 *   $Id: filetools.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 Reloded an Amstrad CPC emulator
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
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

// Returns a vector containing the names of the files in the specified directory
std::vector<std::string> listDirectory(std::string sDirectory) {
   std::vector<std::string> s;

   static const char pathSeparator =
#ifdef _WIN32
	'\\'
#else
	'/'
#endif
	;

   if (sDirectory[sDirectory.size() - 1] != pathSeparator) {
      sDirectory += pathSeparator;
   }

#ifdef _WIN32
   WIN32_FIND_DATA cookie;

   sDirectory += '*';
   HANDLE nextFile = FindFirstFile(sDirectory.c_str(), &cookie);

   do
   {
	   // Add to the list
	   s.push_back(cookie.cFileName);
   }
   while (FindNextFile(nextFile, &cookie) != 0);
   FindClose(nextFile);
#else
   DIR* pDir;
   struct dirent *pent;
   pDir = opendir(sDirectory.c_str());
   if (!pDir){
       printf ("opendir(%s) failed; terminating\n", sDirectory.c_str());
       return s;
   }
   while ((pent = readdir(pDir))){
       if (strcmp(pent->d_name,"..")!=0 && strcmp(pent->d_name,".")!=0) {
           s.push_back(pent->d_name);
       }
   }
   closedir(pDir);
#endif

   std::sort(s.begin(), s.end()); // sort elements
   return s;
}

int file_size (int file_num)
{
	struct stat s;
	
	if (!fstat(file_num, &s)) {
		return s.st_size;
	} else {
		return 0;
	}
}
