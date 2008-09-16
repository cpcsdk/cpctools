# Microsoft Visual C++ generated build script - Do not modify

PROJ = LIBDSK16
DEBUG = 1
PROGTYPE = 1
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
FIRSTC = COMPBZ2.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_WDLL = /nologo /G2 /W3 /Zi /ALw /Od /D "_DEBUG" /D "WIN16" /D "WIN16FLOPPY" /I "." /I "." /I "..\include" /I ".." /FR /GD /Fd"LIBDSK.PDB"
CFLAGS_R_WDLL = /nologo /W3 /ALw /O1 /D "NDEBUG" /D "WIN16" /D "WIN16FLOPPY" /I "." /I "..\include" /I ".." /FR /GD 
LFLAGS_D_WDLL = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /CO /NOE /ALIGN:16 /MAP:FULL
LFLAGS_R_WDLL = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /NOE /ALIGN:16 /MAP:FULL
LIBS_D_WDLL = oldnames libw commdlg shell olecli olesvr ldllcew
LIBS_R_WDLL = oldnames libw commdlg shell olecli olesvr ldllcew
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = LIBDSK.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WDLL)
LFLAGS = $(LFLAGS_D_WDLL)
LIBS = $(LIBS_D_WDLL)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WDLL)
LFLAGS = $(LFLAGS_R_WDLL)
LIBS = $(LIBS_R_WDLL)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = COMPBZ2.SBR \
		COMPGZ.SBR \
		COMPRESS.SBR \
		COMPSQ.SBR \
		DRVCFI.SBR \
		DRVCPCEM.SBR \
		DRVLINUX.SBR \
		DRVMYZ80.SBR \
		DRVPOSIX.SBR \
		DRVWIN32.SBR \
		DSKCHECK.SBR \
		DSKERROR.SBR \
		DSKFMT.SBR \
		DSKGEOM.SBR \
		DSKJNI.SBR \
		DSKLPHYS.SBR \
		DSKOPEN.SBR \
		DSKPARS.SBR \
		DSKREAD.SBR \
		DSKSECID.SBR \
		DSKSEEK.SBR \
		DSKSGEOM.SBR \
		DSKSTAT.SBR \
		DSKTREAD.SBR \
		DSKWRITE.SBR \
		DRVWIN16.SBR \
		DRVNWASP.SBR \
		RPCCLI.SBR \
		RPCMAP.SBR \
		RPCPACK.SBR \
		RPCSERV.SBR \
		DRVRCPM.SBR \
		DRVQM.SBR \
		DSKREPRT.SBR \
		CRCTABLE.SBR \
		DRVADISK.SBR \
		DSKCMT.SBR \
		DSKRETRY.SBR \
		DSKDIRTY.SBR \
		DSKRTRD.SBR \
		DSKTRKID.SBR \
		DRVNTWDM.SBR \
		REMOTE.SBR \
		DRVTELE.SBR \
		DRVLOGI.SBR


COMPBZ2_DEP = j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h \
	j:\c\libdsk\lib\compbz2.h


COMPGZ_DEP = j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h \
	j:\c\libdsk\lib\compgz.h


COMPRESS_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h \
	j:\c\libdsk\lib\comp.h \
	j:\c\libdsk\lib\compsq.h \
	j:\c\libdsk\lib\compgz.h \
	j:\c\libdsk\lib\compbz2.h \
	j:\c\libdsk\lib\compress.inc


COMPSQ_DEP = j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h \
	j:\c\libdsk\lib\compsq.h


DRVCFI_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvcfi.h


DRVCPCEM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvcpcem.h


DRVLINUX_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvlinux.h \
	sys/ioctl.h


DRVMYZ80_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvmyz80.h


DRVPOSIX_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvposix.h


DRVWIN32_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvwin32.h \
	j:\c\libdsk\lib\rpcfuncs.h \
	j:\c\libdsk\lib\w95defs.h


DSKCHECK_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKERROR_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKFMT_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h


DSKGEOM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKLPHYS_DEP = j:\c\libdsk\lib\drvi.h


DSKLPHYS_DEP = j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKOPEN_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drivers.h \
	j:\c\libdsk\lib\compress.h \
	j:\c\libdsk\lib\drivers.inc


DSKPARS_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKREAD_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKSECID_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKSEEK_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKSGEOM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKSTAT_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h


DSKTREAD_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKWRITE_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\compi.h \
	j:\c\libdsk\lib\compress.h


DRVWIN16_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvwin16.h \
	j:\c\libdsk\lib\w16defs.h


DRVNWASP_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvnwasp.h


RPCCLI_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\rpcfuncs.h \
	j:\c\libdsk\lib\remote.h


RPCMAP_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


RPCPACK_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\rpcfuncs.h


RPCSERV_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\rpcfuncs.h


DRVRCPM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvrcpm.h


DRVQM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvqm.h \
	j:\c\libdsk\lib\crctable.h


DSKREPRT_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


CRCTABLE_DEP = 

DRVADISK_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvadisk.h


DSKCMT_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKRETRY_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKDIRTY_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKRTRD_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DSKTRKID_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h


DRVNTWDM_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvntwdm.h


REMOTE_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\rpcfuncs.h \
	j:\c\libdsk\lib\remote.h \
	j:\c\libdsk\lib\remall.h \
	j:\c\libdsk\lib\rpctios.h \
	j:\c\libdsk\lib\rpcwin32.h \
	j:\c\libdsk\lib\rpcfossl.h \
	j:\c\libdsk\lib\rpcfork.h \
	j:\c\libdsk\lib\remote.inc


DRVTELE_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvtele.h


DRVLOGI_DEP = j:\c\libdsk\lib\drvi.h \
	j:\c\libdsk\lib\drv.h \
	linux/fd.h \
	linux/fdreg.h \
	j:\c\libdsk\lib\drvlogi.h


all:	$(PROJ).DLL $(PROJ).BSC

COMPBZ2.OBJ:	..\LIB\COMPBZ2.C $(COMPBZ2_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ..\LIB\COMPBZ2.C

COMPGZ.OBJ:	..\LIB\COMPGZ.C $(COMPGZ_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\COMPGZ.C

COMPRESS.OBJ:	..\LIB\COMPRESS.C $(COMPRESS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\COMPRESS.C

COMPSQ.OBJ:	..\LIB\COMPSQ.C $(COMPSQ_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\COMPSQ.C

DRVCFI.OBJ:	..\LIB\DRVCFI.C $(DRVCFI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVCFI.C

DRVCPCEM.OBJ:	..\LIB\DRVCPCEM.C $(DRVCPCEM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVCPCEM.C

DRVLINUX.OBJ:	..\LIB\DRVLINUX.C $(DRVLINUX_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVLINUX.C

DRVMYZ80.OBJ:	..\LIB\DRVMYZ80.C $(DRVMYZ80_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVMYZ80.C

DRVPOSIX.OBJ:	..\LIB\DRVPOSIX.C $(DRVPOSIX_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVPOSIX.C

DRVWIN32.OBJ:	..\LIB\DRVWIN32.C $(DRVWIN32_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVWIN32.C

DSKCHECK.OBJ:	..\LIB\DSKCHECK.C $(DSKCHECK_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKCHECK.C

DSKERROR.OBJ:	..\LIB\DSKERROR.C $(DSKERROR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKERROR.C

DSKFMT.OBJ:	..\LIB\DSKFMT.C $(DSKFMT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKFMT.C

DSKGEOM.OBJ:	..\LIB\DSKGEOM.C $(DSKGEOM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKGEOM.C

DSKJNI.OBJ:	..\LIB\DSKJNI.C $(DSKJNI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKJNI.C

DSKLPHYS.OBJ:	..\LIB\DSKLPHYS.C $(DSKLPHYS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKLPHYS.C

DSKOPEN.OBJ:	..\LIB\DSKOPEN.C $(DSKOPEN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKOPEN.C

DSKPARS.OBJ:	..\LIB\DSKPARS.C $(DSKPARS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKPARS.C

DSKREAD.OBJ:	..\LIB\DSKREAD.C $(DSKREAD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKREAD.C

DSKSECID.OBJ:	..\LIB\DSKSECID.C $(DSKSECID_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKSECID.C

DSKSEEK.OBJ:	..\LIB\DSKSEEK.C $(DSKSEEK_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKSEEK.C

DSKSGEOM.OBJ:	..\LIB\DSKSGEOM.C $(DSKSGEOM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKSGEOM.C

DSKSTAT.OBJ:	..\LIB\DSKSTAT.C $(DSKSTAT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKSTAT.C

DSKTREAD.OBJ:	..\LIB\DSKTREAD.C $(DSKTREAD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKTREAD.C

DSKWRITE.OBJ:	..\LIB\DSKWRITE.C $(DSKWRITE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKWRITE.C

DRVWIN16.OBJ:	..\LIB\DRVWIN16.C $(DRVWIN16_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVWIN16.C

DRVNWASP.OBJ:	..\LIB\DRVNWASP.C $(DRVNWASP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVNWASP.C

RPCCLI.OBJ:	..\LIB\RPCCLI.C $(RPCCLI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\RPCCLI.C

RPCMAP.OBJ:	..\LIB\RPCMAP.C $(RPCMAP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\RPCMAP.C

RPCPACK.OBJ:	..\LIB\RPCPACK.C $(RPCPACK_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\RPCPACK.C

RPCSERV.OBJ:	..\LIB\RPCSERV.C $(RPCSERV_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\RPCSERV.C

DRVRCPM.OBJ:	..\LIB\DRVRCPM.C $(DRVRCPM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVRCPM.C

DRVQM.OBJ:	..\LIB\DRVQM.C $(DRVQM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVQM.C

DSKREPRT.OBJ:	..\LIB\DSKREPRT.C $(DSKREPRT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKREPRT.C

CRCTABLE.OBJ:	..\LIB\CRCTABLE.C $(CRCTABLE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\CRCTABLE.C

DRVADISK.OBJ:	..\LIB\DRVADISK.C $(DRVADISK_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVADISK.C

DSKCMT.OBJ:	..\LIB\DSKCMT.C $(DSKCMT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKCMT.C

DSKRETRY.OBJ:	..\LIB\DSKRETRY.C $(DSKRETRY_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKRETRY.C

DSKDIRTY.OBJ:	..\LIB\DSKDIRTY.C $(DSKDIRTY_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKDIRTY.C

DSKRTRD.OBJ:	..\LIB\DSKRTRD.C $(DSKRTRD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKRTRD.C

DSKTRKID.OBJ:	..\LIB\DSKTRKID.C $(DSKTRKID_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DSKTRKID.C

DRVNTWDM.OBJ:	..\LIB\DRVNTWDM.C $(DRVNTWDM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVNTWDM.C

REMOTE.OBJ:	..\LIB\REMOTE.C $(REMOTE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\REMOTE.C

DRVTELE.OBJ:	..\LIB\DRVTELE.C $(DRVTELE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVTELE.C

DRVLOGI.OBJ:	..\LIB\DRVLOGI.C $(DRVLOGI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIB\DRVLOGI.C


$(PROJ).DLL::	COMPBZ2.OBJ COMPGZ.OBJ COMPRESS.OBJ COMPSQ.OBJ DRVCFI.OBJ DRVCPCEM.OBJ \
	DRVLINUX.OBJ DRVMYZ80.OBJ DRVPOSIX.OBJ DRVWIN32.OBJ DSKCHECK.OBJ DSKERROR.OBJ DSKFMT.OBJ \
	DSKGEOM.OBJ DSKJNI.OBJ DSKLPHYS.OBJ DSKOPEN.OBJ DSKPARS.OBJ DSKREAD.OBJ DSKSECID.OBJ \
	DSKSEEK.OBJ DSKSGEOM.OBJ DSKSTAT.OBJ DSKTREAD.OBJ DSKWRITE.OBJ DRVWIN16.OBJ DRVNWASP.OBJ \
	RPCCLI.OBJ RPCMAP.OBJ RPCPACK.OBJ RPCSERV.OBJ DRVRCPM.OBJ DRVQM.OBJ DSKREPRT.OBJ \
	CRCTABLE.OBJ DRVADISK.OBJ DSKCMT.OBJ DSKRETRY.OBJ DSKDIRTY.OBJ DSKRTRD.OBJ DSKTRKID.OBJ \
	DRVNTWDM.OBJ REMOTE.OBJ DRVTELE.OBJ DRVLOGI.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
COMPBZ2.OBJ +
COMPGZ.OBJ +
COMPRESS.OBJ +
COMPSQ.OBJ +
DRVCFI.OBJ +
DRVCPCEM.OBJ +
DRVLINUX.OBJ +
DRVMYZ80.OBJ +
DRVPOSIX.OBJ +
DRVWIN32.OBJ +
DSKCHECK.OBJ +
DSKERROR.OBJ +
DSKFMT.OBJ +
DSKGEOM.OBJ +
DSKJNI.OBJ +
DSKLPHYS.OBJ +
DSKOPEN.OBJ +
DSKPARS.OBJ +
DSKREAD.OBJ +
DSKSECID.OBJ +
DSKSEEK.OBJ +
DSKSGEOM.OBJ +
DSKSTAT.OBJ +
DSKTREAD.OBJ +
DSKWRITE.OBJ +
DRVWIN16.OBJ +
DRVNWASP.OBJ +
RPCCLI.OBJ +
RPCMAP.OBJ +
RPCPACK.OBJ +
RPCSERV.OBJ +
DRVRCPM.OBJ +
DRVQM.OBJ +
DSKREPRT.OBJ +
CRCTABLE.OBJ +
DRVADISK.OBJ +
DSKCMT.OBJ +
DSKRETRY.OBJ +
DSKDIRTY.OBJ +
DSKRTRD.OBJ +
DSKTRKID.OBJ +
DRVNTWDM.OBJ +
REMOTE.OBJ +
DRVTELE.OBJ +
DRVLOGI.OBJ +
$(OBJS_EXT)
$(PROJ).DLL
$(MAPFILE)
E:\USR\MSVC\LIB\+
E:\USR\MSVC\MFC\LIB\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@
	implib /nowep $(PROJ).LIB $(PROJ).DLL


run: $(PROJ).DLL
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
