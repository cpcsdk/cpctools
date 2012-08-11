noinst_LIBRARIES = libreloadedbe.a

libreloadedbe_a_CPPFLAGS = -Wno-multichar \
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

