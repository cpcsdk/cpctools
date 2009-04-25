noinst_LIBRARIES = libcapricecli.a

libcapricecli_a_SOURCES = \
                          cparser_tree.c  cparser_tree.h \
                          cparser.h  cparser_io.h  cparser_line.h  cparser_options.h \
                          cparser.c cparser_token.c cparser_token_tbl.c cparser_io_unix.c \
                          cparser_fsm.c cparser_line.c \
                          cparser_toker.h cparser_token.c \
                          cparser_priv.h \
                          cparser_fsm.h cparser_fsm.c \
                          cparser_caprice.c \
                          caprice_cli_routs.cpp caprice_cli_routs.h


if RELEASE
	libcapricecli_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
