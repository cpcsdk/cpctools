# Microsoft Developer Studio Project File - Name="libvideoCPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libvideoCPC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libvideoCPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libvideoCPC.mak" CFG="libvideoCPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libvideoCPC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libvideoCPC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libvideoCPC - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\libtools" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libvideoCPC - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\libtools" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libvideoCPC - Win32 Release"
# Name "libvideoCPC - Win32 Debug"
# Begin Group "FileManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCPCFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CCPCFileManager.h
# End Source File
# End Group
# Begin Group "CPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCPCVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\CCPCVideo.h
# End Source File
# End Group
# Begin Group "Cap32"

# PROP Default_Filter ""
# Begin Group "Video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Cap32\glfunclist.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\glfuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\glfuncs.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\render.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\render.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\renderData.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\video.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\video.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Cap32\cap32type.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\crtc.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\crtc.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\debug.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\gatearray.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\gatearray.h
# End Source File
# Begin Source File

SOURCE=.\Cap32\vdu.cpp
# End Source File
# Begin Source File

SOURCE=.\Cap32\vdu.h
# End Source File
# End Group
# End Target
# End Project
