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

#include <zlib.h>
#include <SDL.h>

#include "cap32.h"
#include "config.h"
#include "debug.h"

#include "emulator.h"

#include "fdc.h"

#include "dsk.h"
#include "snapshot.h"
#include "tape.h"

#include "filetools.h"
#include "configBis.h"
#include <iostream>



Emulator emulator;


void ParseCommandLine( Emulator &emulator, int argc, char ** argv )
{
#ifndef HAVE_LIB765_H
	t_drive& driveA = emulator.GetDriveA();
	t_drive& driveB = emulator.GetDriveB();
#endif
	bool have_DSK = false;
	bool have_SNA = false;
	bool have_TAP = false;
	
#ifndef HAVE_LIB765_H
	// clear disk drive A data structure
	memset(&driveA, 0, sizeof(t_drive));
#endif
	t_CPC &CPC = emulator.GetConfig();

	// loop for all command line arguments
	for (int i = 1; i < argc; i++) 
	{

		int length = strlen(argv[i]);
		// minumum for a valid filename
		if (length > 5) 
		{
			char path[_MAX_PATH + 1];
			char extension[5];
			// argument passed with quotes?
			if (argv[i][0] == '"') 
			{
				// strip the quotes
				length -= 2;
				strncpy(path, &argv[i][1], length);
			}
			else 
			{
				// take it as is
				strncpy(path, &argv[i][0], sizeof(path)-1); 
			}
			int pos = length - 4;
			if (pos > 0) 
			{
				char file_name[_MAX_PATH + 1];
				//bool zip = false;
				
				// grab the extension
				strncpy(extension, &path[pos], 4); 
				// zero terminate string
				extension[4] = '\0'; 
				
				// split into components
				splitPathFileName(path, path, file_name); 
				
				// a disk image?
				if ((!have_DSK) && (strcmp(extension, ".dsk") == 0)) 
				{
					strcat(path, file_name);

#ifndef HAVE_LIB765_H
					if(!dsk_load(path, &driveA, 'A')) 
					{
						strcpy(CPC.drvA_path, path);
						strcpy(CPC.drvA_file, file_name);
						CPC.drvA_zip = 0;
						have_DSK = true;
					}
#else
					emulator.GetFDC().insertA(path);
#endif
				}
				
				if ((!have_SNA) && (strcmp(extension, ".sna") == 0)) 
				{
					strcat(path, file_name);
					if(!snapshot_load(emulator, path)) 
					{
						strcpy(CPC.snap_path, path);
						strcpy(CPC.snap_file, file_name);
						CPC.snap_zip = 0;
						have_SNA = true;
					}
				}
				
				if ((!have_TAP) && (strcmp(extension, ".cdt") == 0)) 
				{
					strcat(path, file_name);
					if(!emulator.GetTape().tape_insert(path)) 
					{
						strcpy(CPC.tape_path, path);
						strcpy(CPC.tape_file, file_name);
						CPC.tape_zip = 0;
						have_TAP = true;
					}
				}
				
				if ((!have_TAP) && (strcmp(extension, ".voc") == 0)) 
				{
					strcat(path, file_name);
					if(!emulator.GetTape().tape_insert_voc(path)) 
					{
						strcpy(CPC.tape_path, path);
						strcpy(CPC.tape_file, file_name);
						CPC.tape_zip = 0;
						have_TAP = true;
					}
				}
         }
      }
   }
   
   // insert disk in drive A?
   if ((!have_DSK) && (CPC.drvA_file[0])) 
   {
	   char chFileName[_MAX_PATH + 1];
	   
	   strncpy(chFileName, CPC.drvA_path, sizeof(chFileName)-1);
	   strncat(chFileName, CPC.drvA_file, sizeof(chFileName)-1 - strlen(chFileName));

#ifndef HAVE_LIB765_H
	   dsk_load(chFileName, &driveA, 'A');
#endif
   }



#ifndef HAVE_LIB765_H
   memset(&driveB, 0, sizeof(t_drive)); // clear disk drive B data structure
#endif
   
   // insert disk in drive B?
   if (CPC.drvB_file[0]) 
   {
	   char chFileName[_MAX_PATH + 1];
	   
	   strncpy(chFileName, CPC.drvB_path, sizeof(chFileName)-1);
	   strncat(chFileName, CPC.drvB_file, sizeof(chFileName)-1 - strlen(chFileName));

#ifndef HAVE_LIB765_H
	   dsk_load(chFileName, &driveB, 'B');
#endif
   }
   
   // insert a tape?
   if ((!have_TAP) && (CPC.tape_file[0])) 
   {
	   int iErrorCode = 0;
	   char chFileName[_MAX_PATH + 1];
	   
	   strncpy(chFileName, CPC.tape_path, sizeof(chFileName)-1);
	   strncat(chFileName, CPC.tape_file, sizeof(chFileName)-1 - strlen(chFileName));
	   
	   if (!iErrorCode) 
	   {
		   int iOffset = strlen(CPC.tape_file) - 3;
		   
		   // pointer to the extension
		   char *pchExt = &CPC.tape_file[iOffset > 0 ? iOffset : 0];
		   // is it a cdt file?
		   if (strncmp(pchExt, "cdt", 3) == 0) 
		   {
			   iErrorCode = emulator.GetTape().tape_insert(chFileName);
		   }
		   // is it a voc file?
		   else if (strncmp(pchExt, "voc", 3) == 0) 
		   { 
			   iErrorCode = emulator.GetTape().tape_insert_voc(chFileName);
		   }
		   if (CPC.tape_zip) 
		   {
			   // dispose of the temp file
			   remove(chFileName);
		   }
	   }
   }
}

int main (int argc, char **argv)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0) 
    {
	std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
	return -1;
    }


    if (emulator.Init())
    {
#ifdef DEBUG
	InitDebug();
#endif
	ParseCommandLine(emulator, argc, argv);

	Renderer render = emulator.GetRenderer();

	emulator.Loop();

	SDL_Quit();

	return 0;
    }
    else
    {
	SDL_Quit();

	std::cerr << "Emulator initialisation failed !" << std::endl;
	return -1;
    }
}
