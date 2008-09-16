# Microsoft Visual C++ generated build script - Do not modify

PROJ = LDSERVER
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
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
FIRSTC = ABOUTBOX.C  
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /W3 /Zi /AM /Od /D "_DEBUG" /D "WIN16" /I "../include" /I "../rpcserv" /FR /GA /Fd"LDSERVER.PDB"
CFLAGS_R_WEXE = /nologo /W3 /AM /O1 /D "NDEBUG" /D "WIN16" /I "../include" /I "../rpcserv" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = oldnames libw mlibcew libdsk16 netapi commdlg.lib olecli.lib olesvr.lib shell.lib 
LIBS_R_WEXE = oldnames libw mlibcew libdsk16 netapi commdlg.lib olecli.lib olesvr.lib shell.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = ..\RPCSERV\LDSERVER.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = ABOUTBOX.SBR \
		LDSERVER.SBR


ABOUTBOX_DEP = j:\c\libdsk\rpcserv\ldserver.h \
	j:\c\libdsk\rpcserv\mailslot.h


LDSERVER_DEP = j:\c\libdsk\rpcserv\ldserver.h \
	j:\c\libdsk\rpcserv\mailslot.h


LDSERVER_RCDEP = j:\c\libdsk\rpcserv/ldserver.ico \
	j:\c\libdsk\rpcserv/shade.bmp


all:	$(PROJ).EXE $(PROJ).BSC

ABOUTBOX.OBJ:	..\RPCSERV\ABOUTBOX.C $(ABOUTBOX_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ..\RPCSERV\ABOUTBOX.C

LDSERVER.OBJ:	..\RPCSERV\LDSERVER.C $(LDSERVER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\RPCSERV\LDSERVER.C

LDSERVER.RES:	..\RPCSERV\LDSERVER.RC $(LDSERVER_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r -FoJ:\C\LIBDSK\WIN16VC1\LDSERVER.RES ..\RPCSERV\LDSERVER.RC


$(PROJ).EXE::	LDSERVER.RES

$(PROJ).EXE::	ABOUTBOX.OBJ LDSERVER.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
ABOUTBOX.OBJ +
LDSERVER.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
H:\DOSC\USR\MSVC\LIB\+
H:\DOSC\USR\MSVC\MFC\LIB\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) LDSERVER.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	LDSERVER.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) LDSERVER.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
