# Author Romain Giot <giot.romain@gmail.com>

# Init
AC_INIT([StSound], [1.1], [krusty@cpcscene.com])
AM_INIT_AUTOMAKE([ -Wall -Werror])

# Check programs
AC_PROG_CXX
#Enable building DLLs on windows
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL


#Output file
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
        Makefile
])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT

AC_ARG_ENABLE(integer,
        [  --enable-integer        Integer emulation mode [default=no]],
        integer_only=yes,
        integer_only=no)

AM_CONDITIONAL([YM_INTEGER_ONLY], [test "$integer_only" = "yes"])

AC_MSG_NOTICE([
libstsound compilation options :

    Use integer only mode.............. ${integer_only}
])
