noinst_LIBRARIES = libreloadedbe.a

libreloadedbe_a_CPPFLAGS =  \
    -I@top_srcdir@/src/core \
    -I@top_srcdir@/src/asm 

libreloadedbe_a_SOURCES = \
RApp.cpp RApp.h \
REmulator.h REmulator.cpp \
RVideo.h \
RWin.h RBitmapView.h

if RELEASE
  libreloadedbe_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s
endif
