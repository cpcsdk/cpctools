# In this directory we build the executable without gui (SNAMP)
cmake_minimum_required(VERSION 2.8)
project(SNAMP)

include_directories(..)

add_executable(snamp WIN32 
    main.cpp
	../aop/portAudioAudioPlugin.cpp
)

add_dependencies(snamp ReloadedCore)
target_link_libraries(snamp ReloadedCore)

if (WIN32)
    target_link_libraries(snamp portaudio_x86 pthreadVC2)
else (WIN32)
    target_link_libraries(snamp portaudio pthread)
endif (WIN32)
