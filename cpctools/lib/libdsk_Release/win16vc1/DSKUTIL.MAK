# Microsoft Visual C++ generated build script - Do not modify

PROJ = DSKUTIL
DEBUG = 1
PROGTYPE = 3
CALLER = 
ARGS = j:\disks\dosgames.dsk
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = J:\C\LIBDSK\WIN16VC1\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DSKUTIL.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_WTTY = /nologo /G2 /Mq /W3 /Zi /ALw /Od /D "_DEBUG" /D "WIN16" /I "." /I "../include" /I "../tools" /FR /Fd"DSKUTIL.PDB"
CFLAGS_R_WTTY = /nologo /Gs /G2 /Mq /W3 /ALw /Ox /D "NDEBUG" /D "WIN16" /I "." /I "../include" /I "../tools" /FR 
LFLAGS_D_WTTY = /NOLOGO /NOD /PACKC:57344 /ALIGN:16 /ONERROR:NOEXE /CO 
LFLAGS_R_WTTY = /NOLOGO /NOD /PACKC:57344 /ALIGN:16 /ONERROR:NOEXE 
LIBS_D_WTTY = oldnames libw llibcewq libdsk16
LIBS_R_WTTY = oldnames libw llibcewq libdsk16 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = LIBDSK16.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WTTY)
LFLAGS = $(LFLAGS_D_WTTY)
LIBS = $(LIBS_D_WTTY)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
DEFFILE=D:\USR\MSVC\BIN\CL.DEF
!else
CFLAGS = $(CFLAGS_R_WTTY)
LFLAGS = $(LFLAGS_R_WTTY)
LIBS = $(LIBS_R_WTTY)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
DEFFILE=D:\USR\MSVC\BIN\CL.DEF
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DSKUTIL.SBR \
		UTILOPTS.SBR \
		FORMNAME.SBR


LIBDSK16_DEP = 

DSKUTIL_DEP = j:\c\libdsk\tools\utilopts.h \
	j:\c\libdsk\tools\formname.h


UTILOPTS_DEP = j:\c\libdsk\tools\utilopts.h


FORMNAME_DEP = j:\c\libdsk\tools\utilopts.h


all:	$(PROJ).EXE $(PROJ).BSC

DSKUTIL.OBJ:	..\TOOLS\DSKUTIL.C $(DSKUTIL_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ..\TOOLS\DSKUTIL.C

UTILOPTS.OBJ:	..\TOOLS\UTILOPTS.C $(UTILOPTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\TOOLS\UTILOPTS.C

FORMNAME.OBJ:	..\TOOLS\FORMNAME.C $(FORMNAME_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\TOOLS\FORMNAME.C


$(PROJ).EXE::	DSKUTIL.OBJ UTILOPTS.OBJ FORMNAME.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DSKUTIL.OBJ +
UTILOPTS.OBJ +
FORMNAME.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
D:\USR\MSVC\LIB\+
D:\USR\MSVC\MFC\LIB\+
LIBDSK16.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@


run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
