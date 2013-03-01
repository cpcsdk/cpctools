cmake_minimum_required(VERSION 2.8)
project(Reloaded)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ActivateCXX11)

#TODO check for StSound
add_definitions(-DST_SOUND)

add_subdirectory(src/core)

IF (BEOS)
	add_definitions(-DUSE_PTHREAD)
	add_subdirectory(src/gui_be)
ELSE (BEOS)
    #	add_definitions(-DUSE_PTHREAD)
	add_subdirectory(src/gui)
	add_subdirectory(src/snamp)
ENDIF (BEOS)

