/**
 *   $Id$
 *   Reloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
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

#include <iostream>
#include <string>
#include "CapASM.h"

#include "sjdefs.h"
#include "lua_sjasm.h"
#include "sjasm.h"


void CapASM::Run()
{
  extern int runAddress ;
  emulator->GoTo(runAddress);
}

void CapASM::Compile(const char * filename)
{
  std::cout << "&Compilation of " << string(filename).c_str() << endl ;

  char buf[MAX_PATH];
  int base_encoding; /* added */
  char* p;
  int i = 1;


  CleanUP();

    // init LUA
  LUA = lua_open();
  lua_atpanic(LUA, (lua_CFunction)LuaFatalError);
  luaL_openlibs(LUA);
  luaopen_pack(LUA);

  tolua_sjasm_open(LUA);
  
  // init vars
  Options::DestionationFName[0] = 0;
  STRCPY( Options::ListingFName, LINEMAX, "/tmp/caprice.lst"); //TODO generate file name
  Options::UnrealLabelListFName[0] = 0;
  STRCPY( Options::SymbolListFName, LINEMAX, "/tmp/caprice.sym"); 
  Options::ExportFName[0] = 0;
  Options::RAWFName[0] = 0;
  Options::NoDestinationFile = true; // not *.out files by default

  // start counter
  long dwStart;
  dwStart = GetTickCount();

  // get current directory
  GetCurrentDirectory(MAX_PATH, buf);
  CurrentDirectory = buf;


  STRCPY(SourceFNames[SourceFNamesCount++], LINEMAX, filename);


  if (!SourceFNames[0][0]) {
    _COUT "No inputfile(s)" _ENDL;
    return;
  }

  if (!Options::DestionationFName[0]) {
    STRCPY(Options::DestionationFName, LINEMAX, SourceFNames[0]);
    if (!(p = strchr(Options::DestionationFName, '.'))) {
      p = Options::DestionationFName;
    } else {
      *p = 0;
    }
    STRCAT(p, LINEMAX-(p-Options::DestionationFName), ".out");
  }

  // init some vars
  InitCPU();

  // if memory type != none
  base_encoding = ConvertEncoding;

  // init first pass
  InitPass(1);

  // open lists
  OpenList();

  // open source filenames
  for (i = 0; i < SourceFNamesCount; i++) {
    OpenFile(SourceFNames[i]);
  }

  _COUT "Pass 1 complete (" _CMDL ErrorCount _CMDL " errors)" _ENDL;

  ConvertEncoding = base_encoding;

  do {
    pass++;

    InitPass(pass);

    if (pass == LASTPASS) {
      OpenDest();
    }
    for (i = 0; i < SourceFNamesCount; i++) {
      OpenFile(SourceFNames[i]);
    }

    if (PseudoORG) {
      CurAddress = adrdisp; PseudoORG = 0;
    }

    if (pass != LASTPASS) {
      _COUT "Pass " _CMDL pass _CMDL " complete (" _CMDL ErrorCount _CMDL " errors)" _ENDL;
    } else {
      _COUT "Pass 3 complete" _ENDL;
    }
  } while (pass < 3);//MAXPASSES);

  pass = 9999; /* added for detect end of compiling */
  if (Options::AddLabelListing) {
    LabelTable.Dump();
  }

  Close();

  if (Options::UnrealLabelListFName[0]) {
    LabelTable.DumpForUnreal();
  }

  if (Options::SymbolListFName[0]) {
    LabelTable.DumpSymbols();
  }

  _COUT "Errors: " _CMDL ErrorCount _CMDL ", warnings: " _CMDL WarningCount _CMDL ", compiled: " _CMDL CompiledCurrentLine _CMDL " lines" _END;

  double dwCount;
  dwCount = GetTickCount() - dwStart;
  if (dwCount < 0) {
    dwCount = 0;
  }
  printf(", work time: %.3f seconds", dwCount / 1000);

  _COUT "" _ENDL;

#ifndef UNDER_CE
  cout << flush;
#endif

  // free RAM
  delete Devices; Devices = 0;

  // close Lua
  lua_close(LUA);

  //return (ErrorCount != 0);


}
