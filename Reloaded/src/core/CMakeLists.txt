# In this directory we build the static library of emulator core
cmake_minimum_required(VERSION 2.8)
project(ReloadedCore)

set(SOURCE_FILES
	# core
	config_manager.cpp
	crtc.cpp
	crtc1.cpp
	Desass.cpp
	dsk.cpp
	emulator.cpp
	fdc_old.cpp
	gatearray.cpp
	input.cpp
	ioport.cpp
	memory.cpp
	ppi.cpp
	psg.cpp
	render.cpp
	renderData.cpp
	snapshot.cpp
	tape.cpp
	vdu.cpp
	video.cpp
	z80.cpp
	# We may further separate these into a toolkit lib...
	c_inifile.cpp
	filetools.cpp
	log.cpp
	synchro.cpp
)

add_library(ReloadedCore STATIC ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(ReloadedCore stsoundlibrary pthreadVC2)
else (WIN32)
    target_link_libraries(ReloadedCore stsound pthread)
endif (WIN32)
