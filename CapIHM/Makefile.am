bin_PROGRAMS = caprice

caprice_SOURCES = audio.cpp \
		  emulator.cpp \
		  input.cpp \
		  ppi.cpp \
		  renderData.cpp \
		  tape.cpp \
		  z80.cpp \
		  crtc.cpp \
		  fdc.cpp \
                  gatearray.cpp \
		  ioport.cpp \
		  psg.cpp \
		  rom_mods.cpp \
		  vdu.cpp \
		  zip.cpp \
		  config.cpp \
		  dsk.cpp \
		  filetools.cpp \
		  memory.cpp \
		  render.cpp \
                  snapshot.cpp \
		  video.cpp \
		  audio.h \
		  config.h \
		  debug.h \
		  error.h \
		  glfuncs.h \
		  memory.h \
		  render.h \
		  tape.h \
		  z80daa.h \
		  cap32.h \
		  dsk.h \
		  fdc.h \
		  gatearray.h \
		  input.h \
		  ppi.h \
		  rom_mods.h \
		  vdu.h \
		  z80.h \
		  cap32type.h \
		  crtc.h \
		  emulator.h \
                  filetools.h  \
		  ioport.h \
		  psg.h \
		  snapshot.h \
		  video.h \
		  zip.h \
		  YmTypes.h \
		  glfunclist.h  \
		  glfuncs.cpp  \
		  glfuncs.h \
		fileutils.h \
		fileutils.cpp \
		CapriceApp.cpp  \
		CapriceApp.h  \
		CapriceRegistersDialogImpl.cpp \
		CapriceRegistersDialogImpl.h \
		CapriceUI.cpp \
		CapriceUI.h \
		CapriceWindowImpl.cpp \
		CapriceWindowImpl.h \
		SDLPanel.cpp \
		SDLPanel.h


caprice_CPPFLAGS = $(SDL_CFLAGS) $(WX_CPPFLAGS)
caprice_LDADD = $(SDL_LIBS) $(SOUNDLIB) $(LIB765) $(WX_LIBS) -ldsk

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
