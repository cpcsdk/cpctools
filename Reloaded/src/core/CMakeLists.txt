cmake_minimum_required(VERSION 2.8)
project(ReloadedCore)

set(SOURCE_FILES
	# core (to be moved)
	../core/config_manager.cpp
	../core/crtc.cpp
	../core/crtc1.cpp
	../core/Desass.cpp
	../core/dsk.cpp
	../core/emulator.cpp
	../core/fdc_old.cpp
	../core/gatearray.cpp
	../core/input.cpp
	../core/ioport.cpp
	../core/memory.cpp
	../core/ppi.cpp
	../core/psg.cpp
	../core/render.cpp
	../core/renderData.cpp
	../core/snapshot.cpp
	../core/tape.cpp
	../core/vdu.cpp
	../core/video.cpp
	../core/z80.cpp
	# We may further separate these into a toolkit lib...
	../core/c_inifile.cpp
	../core/filetools.cpp
	../core/log.cpp
	../core/synchro.cpp
)

add_library(ReloadedCore STATIC ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(ReloadedCore stsoundlibrary)
else (WIN32)
    target_link_libraries(ReloadedCore stsound)
endif (WIN32)
