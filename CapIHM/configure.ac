# Init
AC_INIT([CapriceReloaded], [5], [ramlaid@cpcscene.com])
AM_INIT_AUTOMAKE([ -Wall ])
#AC_CONFIG_SRCDIR(core/cap32.cpp)

# Check for programs
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_RANLIB
AC_PROG_MAKE_SET

m4_pattern_allow([^AC])

# Check for functions
AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strtoul])

# Check for headers
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([memory.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([zlib.h])
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h memory.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_C_CONST
AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_PROG_CC
AC_TYPE_INT64_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memchr memset pow strchr strcspn strrchr strstr strtol strtoul])


dnl Check for sed
AC_CHECK_PROGS(regex_cmd, sed)
if test x$regex_cmd = "x" ; then
	    AC_MSG_ERROR([sed is required to build the data files.])
fi

AM_PROG_CC_C_O


#ARGS

AC_ARG_WITH(	stsound,
		[  --with-stsound	  use St Sound backend [default=yes]],
                 with_stsound=no,
		 with_stsound=yes)
	   

AC_ARG_WITH(	aylet,
		[  --with-aylet		  use Aylet backend [default=no]],
		with_aylet=yes,
		with_aylet=no)

AC_ARG_WITH(	ayemu,
		[  --with-ayemu		  use LibAYEmul [default=no]],
		with_ayemu=yes,
		with_ayemu=no)


AC_ARG_WITH(	lib765,
		[  --with-lib765		  use lib765 backend associate with libdsk for fdc emulation [default=yes]],
		with_lib765=no,
		with_lib765=yes)

AC_ARG_WITH(	wxwidget,
		[  --with-wxwidget	  use wxWidget as a gui [default=yes]],
		with_wxwidget=no,
		with_wxwidget=yes)

AC_MSG_CHECKING(for debugging environment)
AC_ARG_ENABLE(	debug,
		[  --enable-debug	  turn on debug compiler [default=yes]],
		enable_debug=no,
		enable_debug=yes)

if test "$enable_debug" = yes ; then
	CXXFLAGS="-O0 -Wall -g -DDEBUG"
	CFLAGS="-O0 -Wall  -g -DDEBUG"
fi

AM_CONDITIONAL([RELEASE], [test ! "enable_debug" = "yes"])

AC_MSG_RESULT([ $enable_debug ])


AC_ARG_ENABLE(	cli,
		[  --enable-cli		  turn on cli interface [default=no]],
		enable_cli=yes,
		enable_cli=no)

if test "$enable_cli" = "yes" ; then
    AC_DEFINE(CLI,1,[Use a command line interface])
    MAYBE_CLI=cli
    CLI_LIB=cli/libcapricecli.a
else
    MAYBE_CLI=
    CLI_LIB=
fi
AC_SUBST([MAYBE_CLI])
AC_SUBST([CLI_LIB])

# GUI
if test "$with_wxwidget" = "yes" ; then
	m4_include(wxwin.m4)
	
	AM_OPTIONS_WXCONFIG
	reqwx=2.8.0
	AM_PATH_WXCONFIG($reqwx, wxWin=1)
	if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.

		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
	fi
fi


# Sound

AC_MSG_CHECKING([ sound options ----------------------------------------------
])
if test "$with_aylet" = "yes" ; then
    with_stsound="no";
    with_ayemu="no" ;
    sound_backend="Aylet";
    
    
    AC_DEFINE(AYLET,1,[Use Aylet as a sound backend])


else if test "$with_ayemu" = "yes" ; then
	with_aylet="no";
	with_stsound="no";
	sound_backend="LibAYEmu";

	AC_CHECK_HEADERS( [ayemu.h],
			  AC_DEFINE(AYEMU, 1, [Use ayemu library as a sound backend]), AC_MSG_ERROR([Ayemu headers not found !])
			  )

	AC_SUBST([SOUNDLIB], [-layemu])

else if test "$with_stsound" = "yes" ; then
	with_aylet="no";
	with_ayemu="no";
	sound_backend="StSound";
	
	AC_CHECK_HEADER( [Ym2149Ex.h],AC_DEFINE(ST_SOUND,1,[Use St Sound as a sound backend]),AC_MSG_ERROR([St Sound headers not found !]))

	#TODO debug test library
	AC_SUBST([SOUNDLIB], [-lstsound])

	
fi
fi
fi
AM_CONDITIONAL([AYLET], [test "$with_aylet" = "yes"])


# Dsk
AC_MSG_CHECKING([ disk management options ------------------------------------
])
if test "$with_lib765" = "yes" ; then
	AC_CHECK_LIB([dsk], [dsk_open])
	AC_CHECK_HEADER( 765.h, AC_DEFINE(HAVE_LIB765_H,1,[Use lib765 for FDC emulation]), AC_MSG_ERROR([lib765 not found ! look at http://www.seasip.demon.co.uk/Unix/LibDsk ]))
	AC_SUBST([LIB765], [-l765])
fi



AC_SUBST(CXXFLAGS)
AC_SUBST(CFLAGS)


SDL_VERSION=1.2.5
AM_PATH_SDL($SDL_VERSION, :,
		AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))

AC_CHECK_HEADER([SDL/SDL_opengl.h], , AC_MSG_ERROR([SDL_opengl.h not found !]))


#Output file
AC_CONFIG_HEADERS([configBis.h])
AC_CONFIG_FILES([
        Makefile
		src/Makefile
		src/gui/Makefile
		src/core/Makefile
    src/cli/Makefile
])

AC_OUTPUT


AC_MSG_CHECKING([



CapriceReloaded compilation option :

    Use lib765 ................... ${with_lib765}
    Sound Backend ................ ${sound_backend}

    Cli interface ................ ${enable_cli}

    Debug mode ................... ${enable_debug}

])

