cmake_minimum_required(VERSION 2.8)
project(Reloaded)

#TODO check for StSound
add_definitions(-DST_SOUND)

add_subdirectory(src/core)
add_subdirectory(src/gui)

