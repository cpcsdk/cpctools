# Init
AC_INIT([Reloaded], [5], [krusty@cpcscene.com])
AM_INIT_AUTOMAKE([ -Wall ])
#AC_CONFIG_SRCDIR(core/cap32.cpp)
AC_CANONICAL_HOST

# Check for programs
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_RANLIB
AC_PROG_MAKE_SET

m4_pattern_allow([^AC])

# Check for functions
AC_CHECK_FUNCS([memchr memset pow strchr strcspn strrchr strstr strtol strtoul])

# Check for headers
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([memory.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_PROG_CC
AC_TYPE_INT64_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MEMCMP
AC_FUNC_STRTOD

#Use autotools replacement for missing things
AC_REPLACE_FUNCS([usleep])

dnl Check for sed
AC_CHECK_PROGS(regex_cmd, sed)
if test x$regex_cmd = "x" ; then
	    AC_MSG_ERROR([sed is required to build the data files.])
fi

AM_PROG_CC_C_O


#ARGS

# With

AC_ARG_WITH(stsound,
        [  --with-stsound          use St Sound backend [default=yes]],
        with_stsound=no,
        with_stsound=yes)

AC_ARG_WITH(aylet,
        [  --with-aylet            use Aylet backend [default=no]],
        with_aylet=yes,
        with_aylet=no)

AC_ARG_WITH(ayemu,
        [  --with-ayemu            use LibAYEmul [default=no]],
        with_ayemu=yes,
        with_ayemu=no)

AC_ARG_WITH(lib765,
        [  --with-lib765           use lib765 backend associate with libdsk for fdc emulation [default=no]],
        with_lib765=yes,
        with_lib765=no)

AC_ARG_WITH(wxwidget,
        [  --with-wxwidget         use wxWidget as a gui [default=yes]],
        with_wxwidget=no,
        with_wxwidget=yes)

AC_ARG_WITH(cli,
        [  --with-cli              turn on cli interface [default=no]],
        with_cli=yes,
        with_cli=no)

AC_ARG_WITH(socket,
        [  --with-socket           turn on control thru socket [default=no]],
        with_socket=yes,
        with_socket=no)

AC_MSG_NOTICE([devtools ---------------------------------------------------])
with_asm=no

if test "$with_cli" = "yes" ; then
    AC_DEFINE(CLI,1,[Use a command line interface])
    with_asm=yes
fi
AM_CONDITIONAL([CLI], [test "$with_cli" = "yes"])

if test "$with_socket" = "yes" ; then
    AC_DEFINE(SOCKET_CONTROL,1,[Activate control thru socket])
fi
AM_CONDITIONAL([USE_SOCKET], [test "$with_socket" = "yes"])

AC_ARG_WITH(	debugger,
		[  --with-debugger	  use the internal debugger [default=yes]],
		with_debugger=no,
		with_debugger=yes)

if test "$with_debugger" = "yes" ; then
	AC_DEFINE(USE_DEBUGGER,1,[Activate the internal debugger])
fi

AC_ARG_WITH(	ide,
		[  --with-ide		  turn on ide (Integrated Development Environment) [default=yes]],
		with_ide=no,
		with_ide=yes)

if test "$with_ide" = "yes" ; then
    AC_DEFINE(WITH_IDE,1,[Activate Integrated Development Environment])
    with_asm=yes
fi
AM_CONDITIONAL([WITH_IDE], [test "$with_ide" = "yes"])

if test "$with_asm" = "yes" ; then
    AC_DEFINE(WITH_ASM,1,[Allow the compilation of programs in caprice])
	wxLibs=stc
fi
AM_CONDITIONAL([WITH_ASM], [test "$with_asm" = "yes"])

# Enable

AC_ARG_ENABLE(debug,
        [  --enable-debug          turn on debug compiler [default=no]],
        enable_debug=yes,
        enable_debug=no)

AC_MSG_NOTICE([platform and operating system ------------------------------])

# Check if compiler support forcing to produce 32 bit binaries
#CFLAGS="-m32"
#CXXFLAGS="-m32"
#AC_MSG_CHECKING([if compiler support forcing to produce 32 bit binaries])
#AC_TRY_COMPILE(, [;],
#        compiler_has_m32=yes,
#        compiler_has_m32=no)
#if test "$compiler_has_m32" == "yes"; then
#    AC_MSG_RESULT([yes])
#    M32="-m32"
#else
#    AC_MSG_RESULT([no])
    M32=""
#fi

AC_MSG_CHECKING(for debugging mode)
if test "$enable_debug" = "yes" ; then
    CXXFLAGS="-O0 -g -DDEBUG -pg ${M32}"
    CFLAGS="-O0 -g -DDEBUG -pg ${M32}"
else
    CXXFLAGS="-Wall -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions ${M32}"
    CFLAGS="-Wall -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions ${M32}"
fi
AC_SUBST([M32], [${M32}])
AM_CONDITIONAL([RELEASE], [test "enable_debug" = "no"])
AC_MSG_RESULT([$enable_debug])


#####################
# Check system type #
#####################
case "$host_os" in
*darwin* | *rhapsody* | *macosx*)
     AC_DEFINE_UNQUOTED(DARWIN,1,[This is an Apple Darwin system])
     AC_DEFINE_UNQUOTED(USE_PTHREAD,1,[We can use the pthread system to use emulator in background])
     CPPFLAGS="-DMAX_PATH=PATH_MAX -D_APPLE_C_SOURCE $CPPFLAGS"
     CXXFLAGS="$CXXFLAGS -m32"
     CFLAGS="-m32"
     build_target="darwin"
     ;;
linux*)
     AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux system])
#     AC_DEFINE_UNQUOTED(USE_PTHREAD,1,[We can use the pthread system])
     build_target="linux"
     ;;
freebsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(FREEBSD,1,[This is a FreeBSD system])
     build_target="freebsd"
     ;;
openbsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(OPENBSD,1,[This is an OpenBSD system])
     build_target="openbsd"
     ;;
netbsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(NETBSD,1,[This is a NetBSD system])
     ;;
*solaris*)
     AC_DEFINE_UNQUOTED(SOLARIS,1,[This is a Solaris system])
     AC_DEFINE_UNQUOTED(_REENTRANT,1,[Need with solaris or errno doesnt work])
     build_target="solaris"
     ;;
*cygwin*)
     AC_DEFINE_UNQUOTED(CYGWIN,1,[This is a Cygwin system])
     AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
     build_target="cygwin"
     ;;
*mingw*)
     AC_DEFINE_UNQUOTED(MINGW,1,[This is a MinGW system])
     AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
     AC_DEFINE_UNQUOTED(_WIN32,1,[This is a Windows system])
     AC_DEFINE_UNQUOTED(USE_PTHREAD,1,[We can use the pthread system])
     build_target="mingw"
     ;;
*)
     AC_MSG_RESULT(Unrecognised OS $host_os)
     AC_DEFINE_UNQUOTED(OTHEROS,1,[Some strange OS])
;;
esac

AC_MSG_CHECKING([for build target])
AM_CONDITIONAL(DARWIN,  test "$build_target" = "darwin")
AM_CONDITIONAL(CYGWIN,  test "$build_target" = "cygwin")
AM_CONDITIONAL(MINGW,   test "$build_target" = "mingw")
AM_CONDITIONAL(SOLARIS, test "$build_target" = "solaris")
AM_CONDITIONAL(XFREEBSD, test "$build_target" = "freebsd")
AM_CONDITIONAL(OPENBSD, test "$build_target" = "openbsd")
AM_CONDITIONAL(LINUX, test "$build_target" = "linux")
AC_MSG_RESULT([$build_target])

AC_MSG_NOTICE([display and gui options ------------------------------------])
AC_ARG_ENABLE(filedrop,
        [  --enable-filedrop       turn on filedrop capacity [default=yes]],
        enable_filedrop=no,
        enable_filedrop=yes)

if test "$enable_filedrop" = "yes" ; then
    AC_DEFINE(ENABLE_FILEDROP,1,[Activate FileDrop ability])
fi

AC_ARG_ENABLE(opengl,
        [  --enable-opengl         turn on opengl support [default=no]],
        enable_opengl=yes,
        enable_opengl=no)

if test "$enable_opengl" = "yes" ;  then
    AC_DEFINE(USE_OPENGL,1,[Activate OpenGL support])
fi

# GUI
if test "$with_wxwidget" = "yes" ; then
	m4_include(wxwin.m4)
	
	AM_OPTIONS_WXCONFIG
	reqwx=2.8.0
	AM_PATH_WXCONFIG($reqwx, 
                   wxWin=1,
                   wxWin=0,
                   [$wxLibs, adv,core,aui])
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
AM_CONDITIONAL([WITH_WXWIDGET], [test "$with_wxwidget" = "yes"])

#png.h is needed for saving screenshots. Is it really useful ?
AC_CHECK_HEADER( [png.h],AC_DEFINE(HAVE_LIBPNG,1,[Use libpng to save screenshots]),AC_MSG_CHECKING([libpng not found ! Saving screenshots will not work]))

#	AC_SUBST([PNG_LIB], [-lpng])

# Sound

AC_MSG_NOTICE([sound options ----------------------------------------------])
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
	
	AC_CHECK_HEADER( [Ym2149Ex.h],AC_DEFINE(ST_SOUND,1,[Use St Sound as a sound backend]),AC_MSG_ERROR([St Sound headers not found ! Go in ../libstsound]))

	#TODO debug test library
	AC_SUBST([SOUNDLIB], [-lstsound])

	
fi
fi
fi
AM_CONDITIONAL([AYLET], [test "$with_aylet" = "yes"])

# Sound Output
AC_DEFINE([SOUND_OUTPUT_None], 0, [Define enum for sound output])
AC_DEFINE([SOUND_OUTPUT_PortAudio], 1, [Define enum for sound output])
sound_output="None";

AC_MSG_CHECKING([portaudio : >= 19?])
AC_TRY_COMPILE(
        [#include <portaudio.h>
        #include <stdio.h>
        ],
        [
        PaStream* audioStream = NULL;
        Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, 0/*CPC.snd_playback_rate*/, 1, NULL, NULL);
        ],
        AC_MSG_RESULT(yes)
        sound_output="PortAudio",
        AC_MSG_RESULT(no)
#AC_MSG_ERROR(You need portaudio >= 19.1)
        )

AC_DEFINE_UNQUOTED(SOUND_OUTPUT, [SOUND_OUTPUT_${sound_output}], [Sound output library used])
if test "$sound_output" = "PortAudio" ; then
    AC_SUBST([SOUND_OUTPUT_LIB], [-lportaudio])
fi


#TODO: See for a clean fallback mode
#Dsk
AC_MSG_NOTICE([disk management options ------------------------------------])
if test "$with_lib765" = "yes" ; then
    AC_CHECK_LIB([dsk], [dsk_open], LIB765="-l765 -ldsk")
    AC_CHECK_HEADER([765.h], AC_DEFINE(HAVE_LIB765_H,1,[Use lib765 for FDC emulation]), AC_MSG_ERROR([lib765 not found ! look at http://www.seasip.demon.co.uk/Unix/LibDsk ]))
    AC_SUBST(LIB765)
fi
AM_CONDITIONAL([LIB765], [test "$with_lib765" = "yes"])


AC_SUBST(CXXFLAGS)
AC_SUBST(CFLAGS)

# TODO: Detect other timer library
AC_SEARCH_LIBS([clock_gettime], [rt],
        AC_SUBST([TIMER_LIB], [-lrt])
        )

#Output file
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
        Makefile
        src/Makefile
        src/gui/Makefile
        src/gui/IDE/Makefile
        src/core/Makefile
        src/cli/Makefile
        src/asm/Makefile
])

AC_OUTPUT
AC_MSG_NOTICE([
Reloaded compilation options :

    Use lib765 ................... ${with_lib765}
    Sound Backend ................ ${sound_backend}
    Sound Output ................. ${sound_output}

    Cli interface ................ ${with_cli}
    Debugger ..................... ${with_debugger}
    IDE .......................... ${with_ide}
    Assembler .................... ${with_asm}
    OpenGL ....................... ${enable_opengl}

    Debug mode ................... ${enable_debug}

    Host system .................. ${host}
])

if test "$sound_output" = "None" ; then
    AC_MSG_WARN([/!\ No Sound Output capacity, sound desactivated, please install PortAudio >= 19.1 or an other supported sound library])
fi
