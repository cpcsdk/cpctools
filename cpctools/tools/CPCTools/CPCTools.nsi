; CPCTools.nsi

;--------------------------------

!include "Path functions.nsi"

; The name of the installer
Name "CPCTools"

; The file to write
OutFile "CPCTools.exe"

XPStyle on

VIProductVersion "0.3.3"

; The default installation directory
InstallDir $PROGRAMFILES\Arkos\CPCTools

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Arkos\CPCTools" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles
PageEx license
	Caption " Read me"
	LicenseText "General information"
	LicenseData "ReadMe!.txt"
PageExEnd

UninstPage uninstConfirm
UninstPage instfiles

!define LIBRARY_SHELL_EXTENSION
!include Library.nsh
;--------------------------------

Section ""
	SetOutPath $INSTDIR

	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CPCTools" "DisplayName" "CPCTools - Arkos prod"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CPCTools" "UninstallString" "$INSTDIR\uninstall.exe"

	Push $INSTDIR
	Call AddToPath

	File "ReadMe!.txt"	

	WriteUninstaller "uninstall.exe"
SectionEnd

SectionGroup "CPCfs - .dsk manager" SCT_CPCFS

	Section "CPCfs - command line file manager" SCT_CPCFSCMD
	
		SectionSetText SCT_CPCFSCMD "Tools to manage your .dsk for putting / getting"
		
		SetOutPath $INSTDIR
		
		File "cpcFS\Release\cpcfs.exe"
		File "..\..\lib\libdsk\win32vc6\Release\libdsk.dll"

		SetOutPath $INSTDIR\Share
		File "share\libdskrc"
	  
	  	WriteRegStr HKLM "Software\jce@seasip\LibDsk" "ShareDir" $INSTDIR\Share\
	  
	SectionEnd
	
	Section "CPCfsExt - explorer extension file manager" SCT_CPCFSEXTs
	
		SetOutPath $INSTDIR
		
		!insertmacro InstallLib REGDLL SHARED REBOOT_NOTPROTECTED cpcFSExt\Release\cpcFSExt.dll $INSTDIR\cpcFSExt.dll $INSTDIR
		!insertmacro InstallLib DLL SHARED REBOOT_NOTPROTECTED ..\..\lib\libdsk\win32vc6\Release\libdsk.dll $INSTDIR\libdsk.dll $INSTDIR

		SetOutPath $INSTDIR\Share
		File "share\libdskrc"
	  
	  	WriteRegStr HKLM "Software\jce@seasip\LibDsk" "ShareDir" $INSTDIR\Share\
	  	
		MessageBox MB_YESNO|MB_ICONQUESTION "Do you wish to reboot the system (needed for explorer extension install) ?" IDNO +2
			Reboot  	
	SectionEnd
SectionGroupEnd


Section "CPCDSK - .dsk reader and writer" SCT_CPCDSK

	SetOutPath $INSTDIR
	File "cpcDSK\Release\cpcDSK.exe"
SectionEnd

Section "cpcDSKManager - .dsk manager" SCT_CPCDSKMANAGER

	SetOutPath $INSTDIR
	File "cpcDSKManager\Release\cpcDSKManager.exe"
SectionEnd

Section "CPCSnapshot - .sna manager" SCT_CPCSNA

	SetOutPath $INSTDIR
	File "cpcSnapshot\Release\createSnapshot.exe"
SectionEnd

Section "CPCvideo - .scr,.win viewer" SCT_CPCVIDEO

	SetOutPath $INSTDIR	
	File "cpcVideo\Release\cpcFileViewer.exe"
	File "D:\Travail\PC\Lib\SDL\lib\SDL.dll"
SectionEnd

Section "damsConverter - dams to text converter" SCT_DAMSCONV

	SetOutPath $INSTDIR
	File "damsConverter\Release\damsConverter.exe"
SectionEnd

Section "dataDBGenerator - generate data from binary file" SCT_DATADB

	SetOutPath $INSTDIR
	File "dataDBGenerator\Release\dataDBGenerator.exe"
SectionEnd

Section "dataLinker - generate linked data for binary files" SCT_DATALINK

	SetOutPath $INSTDIR
	File "dataLinker\Release\dataLinker.exe"
SectionEnd

Section "AFT - Command line for CPCBooster Arkos File Transfert" SCT_AFT

	SetOutPath $INSTDIR
	File "aft\Release\aft.exe"
SectionEnd

Section /o "source code - all source code" SCT_SOURCE

	SetOutPath $INSTDIR\source\tools\CPCTools
	File CPCTools.dsw
	File _AllCPCTools.dsp

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcfs
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg cpcfs\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcfsExt
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg /x *.dsk /x *.aps /x *.c /x *.tlb /x *.bak  /x *.mk cpcfsExt\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcdsk
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg cpcdsk\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcDSKManager
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg cpcDSKManager\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcSnapshot
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg cpcSnapshot\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\cpcVideo
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg cpcVideo\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\damsConverter
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg damsConverter\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\dataDBGenerator
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg dataDBGenerator\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\dataLinker
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg dataLinker\*.*

	SetOutPath $INSTDIR\source\tools\CPCTools\aft
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg aft\*.*

	SetOutPath $INSTDIR\source\lib\Common\libcpcfs
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libcpcfs\*.*

	SetOutPath $INSTDIR\source\lib\Common\libsnapshot
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libsnapshot\*.*

	SetOutPath $INSTDIR\source\lib\Common\libaft
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libaft\*.*

	SetOutPath $INSTDIR\source\lib\Common\libtools
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libtools\*.*

	SetOutPath $INSTDIR\source\lib\Common\libvideoCPC
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libvideoCPC\*.*

	SetOutPath $INSTDIR\source\lib\Common\libYM
	File /x Debug /x Release /x *.ncb /x *.opt /x *.plg ..\..\lib\Common\libYM\*.*

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
	; Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CPCTools"
	DeleteRegKey HKLM SOFTWARE\Arkos\CPCTools"
	
	DeleteRegKey HKLM "Software\jce@seasip\LibDsk"
	
	!insertmacro UnInstallLib REGDLL SHARED REBOOT_NOTPROTECTED $INSTDIR\cpcFSExt.dll
	!insertmacro UnInstallLib DLL SHARED REBOOT_NOTPROTECTED $INSTDIR\libdsk.dll
	
	Push $INSTDIR
	Call un.RemoveFromPath
	
	; Remove files and uninstaller
	Delete $INSTDIR\cpcfs.exe
	Delete $INSTDIR\cpcdsk.exe
	Delete $INSTDIR\cpcDSKManager.exe
	RMDir /r $INSTDIR\share
	Delete $INSTDIR\createSnapshot.exe
	Delete $INSTDIR\cpcFileViewer.exe
	Delete $INSTDIR\SDL.dll
	Delete $INSTDIR\damsConverter.exe
	Delete $INSTDIR\dataDBGenerator.exe
	Delete $INSTDIR\dataLinker.exe
	Delete $INSTDIR\aft.exe
	RMDir /r $INSTDIR\source
	Delete $INSTDIR\ReadMe!.txt
	Delete $INSTDIR\uninstall.exe
	
	; Remove directories used
	RMDir "$INSTDIR"

	MessageBox MB_YESNO|MB_ICONQUESTION "Do you wish to reboot the system (needed for explorer extension uninstall) ?" IDNO +2
		Reboot
SectionEnd
