bin_PROGRAMS = caprice

#SUBDIRS = core gui

#LDADD = core/libcapricecore.a  gui/libcapricewx.a
caprice_SOURCES = aylet/driver.h      core/cap32.h       core/error.h        core/gatearray.cpp  core/ppi.cpp         core/tape.cpp             gui/CapriceAboutImpl.h \
aylet/main.h        core/cap32type.h   core/fdc.cpp        core/gatearray.h    core/ppi.h           core/tape.h               gui/CapriceApp.cpp \
aylet/sound.h       core/config.cpp    core/fdc.h          core/glfunclist.h   core/psg.cpp         core/vdu.cpp              gui/CapriceApp.h \
aylet/ui.h          core/config.h      core/fdc_765.cpp    core/glfuncs.cpp    core/psg.h           core/vdu.h                gui/CapriceRegistersDialogImpl.cpp \
aylet/z80.h         core/crtc.cpp      core/fdc_765.h      core/glfuncs.h      core/render.cpp      core/video.h              gui/CapriceRegistersDialogImpl.h \
core/audio.cpp      core/crtc.h          core/input.cpp      core/render.h        core/z80.cpp              gui/CapriceUI.cpp \
core/audio.h        core/debug.h       core/fdc_old.h      core/input.h        core/renderData.cpp  core/z80.h                gui/CapriceUI.h \
core/c_bool.h       core/dsk.cpp       core/filetools.cpp  core/ioport.cpp     core/rom_mods.cpp    core/z80daa.h             gui/CapriceWindowImpl.cpp \
core/c_inifile.cpp  core/dsk.h         core/filetools.h    core/ioport.h       core/rom_mods.h      core/zip.cpp              gui/CapriceWindowImpl.h \
core/c_inifile.h    core/emulator.cpp  core/fileutils.cpp  core/memory.cpp     core/snapshot.cpp    core/zip.h                gui/video.cpp \
      core/emulator.h    core/fileutils.h    core/memory.h       core/snapshot.h      gui/CapriceAboutImpl.cpp
caprice_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -Icore -Igui
caprice_LDADD =  $(SDL_LIBS) $(SOUNDLIB) $(LIB765) $(WX_LIBS) -ldsk 

#Add aylet files if used as backend
if AYLET
  caprice_CPPFLAGS += -Iaylet
  caprice_SOURCES += aylet/sound.cc \
		     aylet/sound.h \
		     aylet/drv-oss.cc \
		     aylet/driver.h 
endif



if RELEASE
  caprice_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
