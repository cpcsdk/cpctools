# In this directory we build the executable without gui (SNAMP)
cmake_minimum_required(VERSION 2.8)
project(SNAMP)

include_directories(..)

if(HAIKU)
	set (AudioPlugin ../gui_be/RAudio.h)
else(HAIKU)
	set (AudioPlugin ../aop/portAudioPlugin.cpp)
endif(HAIKU)

add_executable(snamp WIN32 
    main.cpp
	${AudioPlugin}
)

add_dependencies(snamp ReloadedCore)
target_link_libraries(snamp ReloadedCore)

if (WIN32)
	target_link_libraries(snamp portaudio_x86 ${CMAKE_THREAD_LIB_INIT})
else (WIN32)
	if (HAIKU)
		target_link_libraries(snamp ${CMAKE_THREAD_LIB_INIT})
	else (HAIKU)
		target_link_libraries(snamp portaudio ${CMAKE_THREAD_LIB_INIT})
	endif (HAIKU)
endif (WIN32)
