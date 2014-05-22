project (CPCTools)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-multichar -O3 -fPIC")

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
SET(CPACK_PACKAGE_VERSION_PATCH "0")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

SET(CPACK_PACKAGE_EXECUTABLES "cpcfs" "CPCFS")
SET(CPACK_PACKAGE_EXECUTABLES "createSnapshot" "create snapshot")
SET(CPACK_PACKAGE_EXECUTABLES "AFT" "Arkos File Transfer server")
SET(CPACK_PACKAGE_EXECUTABLES "cpcVideo" "CPC Video mode graphics viewer")
SET(CPACK_PACKAGE_EXECUTABLES "damsConverter" "Convert DAMS source files to ASCII")
SET(CPACK_PACKAGE_EXECUTABLES "ZMac" "z80 macro assembler")
INCLUDE(CPack)
