# In this directory we build the executable with wxWidgets gui

# wxWidgets
set(wxWidgets_CONFIGURATION 'wxmsw')
find_package(wxWidgets COMPONENTS aui adv core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )

include_directories(../core)

# TODO build a lib for caprice core
add_executable(reloaded WIN32 
	CapriceAboutImpl.cpp
	CapriceApp.cpp
	CapriceInputSettingsImpl.cpp
	CapriceRegistersDialogImpl.cpp
	CapriceUIWrapper.cpp
	CapriceWindowImpl.cpp
	MemoryImpl.cpp
	../core/portAudioAudioPlugin.cpp
	WXEmulator.cpp

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

if (WIN32)
    target_link_libraries(reloaded ${wxWidgets_LIBRARIES} stsoundlibrary portaudio_x86 pthreadVC2)
else (WIN32)
    target_link_libraries(reloaded ${wxWidgets_LIBRARIES} stsound portaudio pthread)
endif (WIN32)
