/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
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

//{{{ Code copied from sjasm.cpp

namespace Options {
	char SymbolListFName[LINEMAX];
	char ListingFName[LINEMAX];
	char ExportFName[LINEMAX];
	char DestionationFName[LINEMAX];
	char RAWFName[LINEMAX];
	char UnrealLabelListFName[LINEMAX];

	char ZX_SnapshotFName[LINEMAX];
	char ZX_TapeFName[LINEMAX];

	bool IsPseudoOpBOF = 0;
	bool IsAutoReloc = 0;
	bool IsLabelTableInListing = 0;
	bool IsReversePOP = 0;
	bool IsShowFullPath = 0;
	bool AddLabelListing = 0;
	bool HideLogo = 0;
	bool NoDestinationFile = 0;

	CStringsList* IncludeDirsList = 0;


} // eof namespace Options
//EMemoryType MemoryType = MT_NONE;
CDevice *Devices = 0;
CDevice *Device = 0;
CDeviceSlot *Slot = 0;
CDevicePage *Page = 0;
char* DeviceID = 0;

// extend
char filename[LINEMAX], * lp, line[LINEMAX], temp[LINEMAX], * tp, pline[LINEMAX2], ErrorLine[LINEMAX2], * bp;
char mline[LINEMAX2], sline[LINEMAX2], sline2[LINEMAX2];

char SourceFNames[128][MAX_PATH];
int CurrentSourceFName = 0;
int SourceFNamesCount = 0;

bool displayerror,displayinprocces = 0;
int ConvertEncoding = ENCWIN; /* added */

int pass = 0, IsLabelNotFound = 0, ErrorCount = 0, WarningCount = 0, IncludeLevel = -1;
int IsRunning = 0, IsListingFileOpened = 1, donotlist = 0,listdata  = 0,listmacro  = 0;
int adrdisp = 0,PseudoORG = 0; /* added for spectrum ram */
char* MemoryRAM=NULL, * MemoryPointer=NULL; /* added for spectrum ram */
int MemoryCPage = 0, MemoryPagesCount = 0, StartAddress = 0;
aint MemorySize = 0;
int macronummer = 0, lijst = 0, reglenwidth = 0, synerr = 1;
aint CurAddress = 0, AddressOfMAP = 0, CurrentGlobalLine = 0, CurrentLocalLine = 0, CompiledCurrentLine = 0;
int destlen = 0;
int size = -1;
aint PreviousErrorLine = (aint)-1, maxlin = 0, comlin = 0;
char* CurrentDirectory=NULL;

void (*GetCPUInstruction)(void);

char* ModuleName=NULL, * macrolabp=NULL, * LastParsedLabel=NULL;
char* vorlabp= NULL;
stack<SRepeatStack> RepeatStack; /* added */
CStringsList* lijstp = 0;
CLabelTable LabelTable;
CLocalLabelTable LocalLabelTable;
CDefineTable DefineTable;
CMacroDefineTable MacroDefineTable;
CMacroTable MacroTable;
CStructureTable StructureTable;
CAddressList* AddressList = 0; /* from SjASM 0.39g */
CStringsList* ModuleList = 0;

lua_State *LUA;
int LuaLine=-1;

/* modified */
void InitPass(int p) {
	reglenwidth = 1;
	if (maxlin > 9) {
		reglenwidth = 2;
	}
	if (maxlin > 99) {
		reglenwidth = 3;
	}
	if (maxlin > 999) {
		reglenwidth = 4;
	}
	if (maxlin > 9999) {
		reglenwidth = 5;
	}
	if (maxlin > 99999) {
		reglenwidth = 6;
	}
	if (maxlin > 999999) {
		reglenwidth = 7;
	}
	if (ModuleName != NULL) {
		delete[] ModuleName;
	}
	ModuleName = NULL;
	if (LastParsedLabel != NULL) {
		delete[] LastParsedLabel;
	}
	LastParsedLabel = NULL;
  if (vorlabp == NULL)
  {
    //TODO verify if correct allocation and remove allocation after
    vorlabp = (char *) malloc( sizeof(char) * 2);
  }
	strcpy(vorlabp, "_");
	macrolabp = NULL;
	listmacro = 0;
	pass = p;
	CurAddress = AddressOfMAP = 0;
	IsRunning = 1;
	CurrentGlobalLine = CurrentLocalLine = CompiledCurrentLine = 0;
	PseudoORG = 0; adrdisp = 0; /* added */
	PreviousAddress = 0; epadres = 0; macronummer = 0; lijst = 0; comlin = 0;
	ModuleList = 0;
	StructureTable.Init();
	MacroTable.Init();
	DefineTable.Init();
	MacroDefineTable.Init();

	// predefined
	DefineTable.Replace("_SJASMPLUS", "1");
	DefineTable.Replace("_VERSION", "\"1.07\"");
	DefineTable.Replace("_RELEASE", "7");
	DefineTable.Replace("_ERRORS", "0");
	DefineTable.Replace("_WARNINGS", "0");

  DefineTable.Replace("_CAPRICE", "1");
}

void FreeRAM() {
	delete Devices;
	delete AddressList;
	delete ModuleList;
	delete lijstp;
}

/* added */
void ExitASM(int p) {
	FreeRAM();
	if (pass == LASTPASS) {
		Close();
	}
	exit(p);
}

void LuaFatalError(lua_State *L) {
	Error((char *)lua_tostring(L, -1), 0, FATAL);
}

//}}}

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
	delete Devices;

	// close Lua
	lua_close(LUA);

	//return (ErrorCount != 0);


}
