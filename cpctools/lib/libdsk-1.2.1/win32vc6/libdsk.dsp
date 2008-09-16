# Microsoft Developer Studio Project File - Name="libdsk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libdsk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libdsk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libdsk.mak" CFG="libdsk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libdsk - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libdsk - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libdsk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDSK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "../include" /I "../lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDSK_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDSK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "." /I "../include" /I "../lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDSK_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "libdsk - Win32 Release"
# Name "libdsk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lib\compbz2.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\compgz.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\compress.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\compsq.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\crc16.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\crctable.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvadisk.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvcfi.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvcpcem.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvdos16.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvdos32.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvlinux.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvlogi.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvmyz80.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvntwdm.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvnwasp.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvposix.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvqm.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvrcpm.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvtele.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvwin16.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\drvwin32.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskcheck.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskcmt.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskerror.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskfmt.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskgeom.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskjni.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dsklphys.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskopen.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskpars.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskread.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskreprt.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskretry.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskrtrd.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dsksecid.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskseek.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dsksgeom.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskstat.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dsktread.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dsktrkid.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\dskwrite.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\remote.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\rpccli.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\rpcmap.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\rpcpack.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\rpcserv.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lib\rpcwin32.c

!IF  "$(CFG)" == "libdsk - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libdsk - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\lib\comp.h
# End Source File
# Begin Source File

SOURCE=..\lib\compbz2.h
# End Source File
# Begin Source File

SOURCE=..\lib\compgz.h
# End Source File
# Begin Source File

SOURCE=..\lib\compi.h
# End Source File
# Begin Source File

SOURCE=..\lib\compress.h
# End Source File
# Begin Source File

SOURCE=..\lib\compsq.h
# End Source File
# Begin Source File

SOURCE=..\lib\crc16.h
# End Source File
# Begin Source File

SOURCE=..\lib\crctable.h
# End Source File
# Begin Source File

SOURCE=..\lib\drivers.h
# End Source File
# Begin Source File

SOURCE=..\lib\drv.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvadisk.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvcfi.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvcpcem.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvdos16.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvdos32.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvi.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvlinux.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvmyz80.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvposix.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvqm.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvrcpm.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvwin16.h
# End Source File
# Begin Source File

SOURCE=..\lib\drvwin32.h
# End Source File
# Begin Source File

SOURCE=..\lib\remote.h
# End Source File
# Begin Source File

SOURCE=..\lib\rpcwin32.h
# End Source File
# Begin Source File

SOURCE=..\lib\w16defs.h
# End Source File
# Begin Source File

SOURCE=..\lib\w95defs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
