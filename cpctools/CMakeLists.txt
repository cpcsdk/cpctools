cmake_minimum_required(VERSION 3.10)
project (CPCTools)

include(CheckIncludeFileCXX)
include(CheckCCompilerFlag)
include(cmake/CheckSharedLibraryExists.cmake)


if (MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC")
endif()

check_c_compiler_flag(-Wno-multichar COMPILER_HAS_WNO_MULTICHAR)
if(COMPILER_HAS_WNO_MULTICHAR)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
endif()

#LibDSK checks
set(LIBDSK_HEADERS_DIR "" CACHE PATH "Path to libdsk.h")
set(LIBDSK_LIB_DIR "" CACHE PATH "Path to libdsk.dll")

set(CMAKE_REQUIRED_INCLUDES "${LIBDSK_HEADERS_DIR}")

if(HAIKU)
  # libdsk.h uses size_t without proper includes...
  set(CMAKE_REQUIRED_FLAGS "-Dsize_t=int")
elseif(UNIX)
  # libdsk.h uses size_t without proper includes...
  # (this doesn't work on haiku because gcc2, see haiku fix above)
  set(CMAKE_REQUIRED_FLAGS "-include stddef.h")
endif()

CHECK_INCLUDE_FILE_CXX("libdsk.h" HAVE_LIBDSK_H)
if(NOT HAVE_LIBDSK_H)
	unset(HAVE_LIBDSK_H CACHE)
	message(FATAL_ERROR "libdsk.h not found. Set LIBDSK_HEADERS_DIR if it is installed to a nonstandard location.")
endif()

include_directories(${LIBDSK_HEADERS_DIR})

# On linux you link with -ldsk, but on windows the lib is "libdsk.dll" instead of "dsk.dll"...
FIND_LIBRARY(LIBDSK_LIBRARY NAMES dsk libdsk HINTS ${LIBDSK_LIB_DIR})
if (${LIBDSK_LIBRARY} EQUAL LIBDSK_LIBRARY_NOTFOUND)
	unset(LIBDSK_LIBRARY CACHE)
	message(FATAL_ERROR "libdsk library not found. Set LIBDSK_LIB_DIR if it is installed to a nonstandard location.")
endif()

INSTALL(FILES README DESTINATION docs)

set(libtools_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtools)
set(libcpcfs_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcpcfs)
set(libsnapshot_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories (${libcpcfs_dir})
include_directories (${libtools_dir})
include_directories (${libsnapshot_dir})
add_subdirectory(tools)
add_subdirectory(lib)

# Now generate a nice release package !
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ramlaid CPC Tools")
SET(CPACK_PACKAGE_VENDOR "Ramlaid and cpcsdk team")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
#SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

SET(CPACK_PACKAGE_EXECUTABLES "cpcfs" "CPCFS")
SET(CPACK_PACKAGE_EXECUTABLES "createSnapshot" "create snapshot")
SET(CPACK_PACKAGE_EXECUTABLES "AFT" "Arkos File Transfer server")
SET(CPACK_PACKAGE_EXECUTABLES "cpcVideo" "CPC Video mode graphics viewer")
SET(CPACK_PACKAGE_EXECUTABLES "damsConverter" "Convert DAMS source files to ASCII")
SET(CPACK_PACKAGE_EXECUTABLES "ZMac" "z80 macro assembler")

SET(CPACK_GENERATOR "ZIP")
INCLUDE(CPack)
