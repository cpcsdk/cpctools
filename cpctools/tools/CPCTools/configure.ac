# Author Romain Giot <giot.romain@gmail.com>

# Init
AC_INIT([CapriceReloaded], [1.1], [krusty@cpcscene.com])
AM_INIT_AUTOMAKE([ -Wall -Werror])

AC_PROG_CXX




AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strtoul])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([memory.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_C_CONST
AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_PROG_CC
AC_TYPE_INT64_T




AM_PATH_SDL($SDL_VERSION, :,
AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
#Output file
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
        Makefile
])
