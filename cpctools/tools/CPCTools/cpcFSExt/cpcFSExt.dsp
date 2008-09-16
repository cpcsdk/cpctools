# Microsoft Developer Studio Project File - Name="cpcFSExt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cpcFSExt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cpcFSExt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cpcFSExt.mak" CFG="cpcFSExt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cpcFSExt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cpcFSExt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "cpcFSExt"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cpcFSExt - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\lib\Common\libcpcfs" /I "..\..\..\lib\libdsk\include" /I "..\..\..\lib\Common\libaft" /I "..\..\..\lib\Common\libcpcdsk" /I "..\..\..\lib\Common\libtools" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\cpcFSExt.dll
InputPath=.\Debug\cpcFSExt.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy         ..\..\..\lib\libdsk\win32vc6\Debug\libdsk.dll         Debug\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cpcFSExt - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cpcFSExt___Win32_Release"
# PROP BASE Intermediate_Dir "cpcFSExt___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\lib\Common\libcpcfs" /I "..\..\..\lib\libdsk\include" /I "..\..\..\lib\Common\libaft" /I "..\..\..\lib\Common\libcpcdsk" /I "..\..\..\lib\Common\libtools" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\cpcFSExt.dll
InputPath=.\Release\cpcFSExt.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "cpcFSExt - Win32 Debug"
# Name "cpcFSExt - Win32 Release"
# Begin Group "XDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XDialog\XFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFileDialog.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFileDialogRes.h
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFolderDialog.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\XDialog\XFolderDialogRes.h
# End Source File
# Begin Source File

SOURCE=.\XDialog\XHistoryCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\XDialog\XHistoryCombo.h
# End Source File
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CatalogueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CatalogueDlg.h
# End Source File
# Begin Source File

SOURCE=.\CPCBoosterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CPCBoosterDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditBinaryHeaderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditBinaryHeaderDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\TransfertADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfertADlg.h
# End Source File
# End Group
# Begin Group "cpcFSExt"

# PROP Default_Filter ""
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cpcFSExt.def
# End Source File
# Begin Source File

SOURCE=.\cpcFSExt.idl
# ADD MTL /tlb ".\cpcFSExt.tlb" /h "cpcFSExt.h" /iid "cpcFSExt_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\cpcFSExt.rc
# End Source File
# Begin Source File

SOURCE=.\cpcFSShellExt.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cpcFSExt.cpp
# End Source File
# Begin Source File

SOURCE=.\cpcFSShellExt.cpp
# End Source File
# Begin Source File

SOURCE=.\cpcFSShellExt.h
# End Source File
# Begin Source File

SOURCE=.\fileTools.cpp
# End Source File
# Begin Source File

SOURCE=.\fileTools.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\transfertThread.cpp
# End Source File
# Begin Source File

SOURCE=.\transfertThread.h
# End Source File
# End Group
# End Target
# End Project
