# In this directory we build the Haiku GUI

# TODO we replicate a lot of the src/gui dir in here. Cleanup !
include_directories(../core)
include_directories(keymap)

# TODO build a lib for caprice core
add_executable(reloaded
	RWin.cpp
	REmulator.cpp
	RApp.cpp
	keymap/KeyboardLayout.cpp
	keymap/KeyboardLayoutView.cpp
	keymap/KeymapWindow.cpp
	keymap/Keymap.cpp
	keymap/Keymap2.cpp

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

target_link_libraries(reloaded stsound be game tracker)
