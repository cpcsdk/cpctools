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


AC_LANG([C++])
AC_CHECK_HEADERS(sstream strstream cassert)


#Args
AC_ARG_WITH(	cpcfs,
		[  --with-cpcfs            Create cpcfs application [default=yes]],
                 ,
		 with_cpcfs=yes)

AC_ARG_WITH(	cpcdskmanager,
		[  --with-cpcdskmanager    Create cpcdskmanager application [default=yes]],
                 ,
		 with_cpcdskmanager=yes)


AC_ARG_WITH(	cpcvideo,
		[  --with-cpcvideo         Create cpcvideo application [default=yes]],
                 ,
		 with_cpcvideo=yes)

AC_ARG_WITH( 	cpcsnapshot,
		[  --with-cpcsnapshot      Create cpcsnapshot application [default=yes]],
		,
		with_cpcsnapshot=yes)

AC_ARG_WITH(    damsConverter,
                [  --with-damsconverter    Create damsconverter application [default=yes]],
                ,
                with_damsconverter=yes)


AC_ARG_WITH(    datadbgenerator,
                [  --with-datadbgenerator  Create datadbgenerator application [default=yes]],
                ,
                with_datadbgenerator=yes)

AC_ARG_WITH(    aft,
                [  --with-aft              Create aft application [default=yes]],
                ,
                with_aft=yes)


AC_ARG_WITH(    datalinker,
                [  --with-datalinker       Create datalinker application [default=yes]],
                ,
                with_datalinker=yes)



if (test "$with_cpcfs" = "yes") || (test "$with_cpcdskmanager" = "yes"); then
	AC_CHECK_LIB([dsk], [dsk_map_delete])
	AC_CHECK_HEADERS([libdsk.h])
fi


if test "$with_cpcdskmanager" = "yes" ; then
	AC_CHECK_HEADERS([ctype.h])
fi


if test "$with_cpcvideo" = "yes" ; then
	AC_CHECK_HEADER([AntTweakBar.h])
	AC_CHECK_HEADER([SDL_video.h])

	SDL_VERSION=1.2.5
	AM_PATH_SDL($SDL_VERSION, :,
                        AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
fi

#Output file
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	lib/Makefile
	lib/Common/Makefile
	tools/Makefile
	tools/CPCTools/Makefile
	tools/CPCTools/aft/Makefile 
        tools/CPCTools/cpcDSK/Makefile
        tools/CPCTools/cpcFS/Makefile
        tools/CPCTools/cpcFSExt/Makefile
        tools/CPCTools/cpcSnapshot/Makefile
        tools/CPCTools/cpcVideo/Makefile
        tools/CPCTools/damsConverter/Makefile
        tools/CPCTools/dataDBGenerator/Makefile
        tools/CPCTools/dataLinker/Makefile
 	lib/Common/libaft/Makefile
	lib/Common/libcpcfs/Makefile
	lib/Common/libsnapshot/Makefile
	lib/Common/libtools/Makefile
	lib/Common/libVideoCPC/Makefile
	lib/Common/libYM/Makefile
])
AC_OUTPUT
