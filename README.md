cpcsdk - tools for Amstrad CPC development.

What is it?
===========

This is a collection of various tools for cross development for Amstrad CPC machines.
The goal is to provide a fast and automated "modify,build,run" development cycle in line
with the UNIX way of doing things.

Some graphical tools are also provided, with a focus on debugging, analyzing,
reverse engineering and experimentations.

What's in there
===============

* AMSDOScope: hexdumps the first 128 bytes of a file and highlight the relevant fields of the AMSDOS header.
* DiskSwissKnife: discology-like DSK sector explorer in Qt (unfinished)
* font_catcher: extract a font from a PNG file and convert it to a format suitable for use in a writer/scroller
* hideur_maikeur: add and edit AMSDOS header on files.
* iDSK: Add and extract files from DSK images.
* libstsound: modified version of the STSound library for accurate AY3-8912 emulation
* pydsk: tool to put data in sectors of a DSK file (no filesystem support)
* reloaded: Amstrad CPC emulator, fork of Caprice 32
* snamp: SNApshot Music Player
* starkos: a relocator for starkos songs
* ufitoolbox: experiments to access CPC floppies using an USB floppy drive (unfinished)
* cpctools: Ramlaid cpctools
	* AFT2: cpcbooster file transfer system
	* cpcfs: manage dsk files, and real disks. Supports USB floppy drives with some Parados formats
	* cpcvideo: experiment with the CRTC, view scr files, prototype effects with Lua scripts
	* damsconverter: converts DAMS source files to ASCII.
	* datalinker: smart memory organizer to put your data in memory using as few space as possible, but still matching alignment constraints
	* zmac: a z80 assembler
	* cpcFSExt: an explorer.exe extension to manage DSK files like zip archives (right click > extract, etc)
	* cpcDSKManager: low-level DSK management tool (read and write sectors, generate custom formats)
	* createsnapshot: edit .sna files from the command line
	* datadbgenerator: convert a binary file to .db statements, if your assembler is lacking INCBIN support
	* cpcDSK: read or write DSK files to floppies
	* Various reusable libraries to manage all of this in other tools
* resources: z80 sourcecode for various uses
	* CPC_BOOSTER: firmware for the CPCBooster (AVR assembly code)
	* CPCSD: SD card interface for CPC, using the printer port.
	* mode5_viewer: viewer for "mode 5" pictures (mode 1 with rasters)
	* sample_project: sample of a makefile to use with all these tools.

Remember to check the wiki for more information about the tools. Most of them also have a separate README inside their source folders.
