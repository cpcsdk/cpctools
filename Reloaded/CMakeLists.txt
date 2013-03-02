cmake_minimum_required(VERSION 2.8)
project(Reloaded)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ActivateCXX11)

#TODO check for StSound
add_definitions(-DST_SOUND)

find_package(Threads REQUIRED)

IF (HAIKU)
	add_definitions(-DUSE_PTHREAD)
	add_subdirectory(src/gui_be)
ELSE (HAIKU)
    #	add_definitions(-DUSE_PTHREAD)
	add_subdirectory(src/gui)
ENDIF (HAIKU)

add_subdirectory(src/core)
add_subdirectory(src/snamp)

