cmake_minimum_required(VERSION 2.8)
project(Reloaded)

#TODO check for StSound
add_definitions(-DST_SOUND)

add_subdirectory(src/core)

IF (BEOS)
	add_definitions(-DUSE_PTHREAD)
	add_subdirectory(src/gui_be)
ELSE (BEOS)
	add_subdirectory(src/gui)
ENDIF (BEOS)

