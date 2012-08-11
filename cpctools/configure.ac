# Author Romain Giot <giot.romain@gmail.com>

# Init
AC_INIT([cpctools], [1.1], [ramlaid@cpcscene.com])
AM_INIT_AUTOMAKE([ -Wall -Werror ])

# Check programs
AC_PROG_MAKE_SET
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_CC
AC_PROG_LIBTOOL




#Args
AC_ARG_WITH(	cpcdskmanager,
		[  --with-cpcdskmanager    Create cpcdskmanager application [default=yes]],
                 ,
		 with_cpcdskmanager=yes)


AC_ARG_WITH(    damsConverter,
                [  --with-damsconverter    Create damsconverter application [default=yes]],
                ,
                with_damsconverter=yes)


AC_ARG_WITH(    datadbgenerator,
                [  --with-datadbgenerator  Create datadbgenerator application [default=yes]],
                ,
                with_datadbgenerator=yes)

AC_ARG_WITH(    datalinker,
                [  --with-datalinker       Create datalinker application [default=yes]],
                ,
                with_datalinker=yes)

AC_LANG([C++])
AC_CHECK_HEADERS(sstream strstream cassert)

if test "$with_cpcdskmanager" = "yes" ; then
	AC_CHECK_HEADERS([ctype.h])
fi



#Output file
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	tools/CPCTools/Makefile
        tools/CPCTools/cpcDSK/Makefile
        tools/CPCTools/cpcFSExt/Makefile
        tools/CPCTools/damsConverter/Makefile
        tools/CPCTools/dataDBGenerator/Makefile
        tools/CPCTools/dataLinker/Makefile
])
AC_OUTPUT
