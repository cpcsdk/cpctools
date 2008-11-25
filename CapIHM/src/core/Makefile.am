noinst_LIBRARIES = libcapricecore.a

libcapricecore_a_CPPFLAGS=$(SDL_CFLAGS) 
libcapricecore_a_SOURCES = \
audio.cpp\
audio.h\
c_bool.h\
c_inifile.cpp\
c_inifile.h\
cap32.h\
cap32type.h\
config.cpp\
config.h\
crtc.cpp\
crtc.h\
debug.h\
dsk.cpp\
dsk.h\
emulator.h\
error.h\
fdc.cpp\
fdc.h\
fdc_765.cpp\
fdc_765.h\
fdc_old.h\
filetools.cpp\
filetools.h\
fileutils.cpp\
fileutils.h\
gatearray.cpp\
gatearray.h\
glfunclist.h\
glfuncs.cpp\
glfuncs.h\
input.h\
ioport.cpp\
ioport.h\
memory.cpp\
memory.h\
ppi.cpp\
ppi.h\
psg.cpp\
psg.h\
render.cpp\
render.h\
renderData.cpp\
rom_mods.cpp\
rom_mods.h\
snapshot.cpp\
snapshot.h\
tape.cpp\
tape.h\
vdu.cpp\
vdu.h\
video.h\
z80.cpp\
z80.h\
z80daa.h\
zip.cpp\
zip.h


if RELEASE
	libcapricecore_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
