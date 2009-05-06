noinst_LIBRARIES = libsjasm.a

libsjasm_a_CPPFLAGS = -O2 -DLUA_USE_LINUX -DMAX_PATH=PATH_MAX -Ilua5.1 -Itolua++ -Isjasm -I../core $(SDL_CFLAGS) 

SJASMSRC=sjasm/devices.cpp sjasm/directives.cpp sjasm/io_snapshots.cpp sjasm/io_trd.cpp sjasm/lua_lpack.c sjasm/lua_sjasm.cpp \
sjasm/parser.cpp sjasm/reader.cpp sjasm/sjasm.cpp sjasm/sjio.cpp sjasm/support.cpp sjasm/tables.cpp sjasm/z80.cpp



LUASRC= lua5.1/lapi.c lua5.1/lauxlib.c lua5.1/lbaselib.c lua5.1/lcode.c lua5.1/ldblib.c \
lua5.1/ldebug.c lua5.1/ldo.c lua5.1/ldump.c lua5.1/lfunc.c lua5.1/lgc.c lua5.1/linit.c \
lua5.1/liolib.c lua5.1/llex.c lua5.1/lmathlib.c lua5.1/lmem.c lua5.1/loadlib.c \
lua5.1/lobject.c lua5.1/lopcodes.c lua5.1/loslib.c lua5.1/lparser.c lua5.1/lstate.c \
lua5.1/lstring.c lua5.1/lstrlib.c lua5.1/ltable.c lua5.1/ltablib.c lua5.1/ltm.c \
lua5.1/lundump.c lua5.1/lvm.c lua5.1/lzio.c

# tolua objects
TOLUASRC=tolua++/tolua_event.c tolua++/tolua_is.c tolua++/tolua_map.c \
tolua++/tolua_push.c tolua++/tolua_to.c



libsjasm_a_SOURCES = \
  $(SJASMSRC) \
  $(LUASRC) \
  $(TOLUASRC) \
  CapASM.h \
  CapASM.cpp
