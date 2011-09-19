noinst_LIBRARIES = libreloadedbe.a

libreloadedbe_a_CPPFLAGS =  \
    -I@top_srcdir@/src/core \
    -I@top_srcdir@/src/asm \
    -Ikeymap -I. -I-

libreloadedbe_a_SOURCES = \
RApp.cpp RApp.h \
REmulator.h REmulator.cpp \
RVideo.h \
RWin.h RWin.cpp RBitmapView.h keymap/KeymapWindow.cpp keymap/KeymapWindow.h \
keymap/Keymap2.cpp keymap/Keymap.cpp keymap/KeyboardLayout.cpp \
keymap/KeyboardLayoutView.cpp

if RELEASE
  libreloadedbe_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s
endif

