# In this directory we build the executable with wxWidgets gui

# wxWidgets
set(wxWidgets_CONFIGURATION 'wxmsw')
find_package(wxWidgets COMPONENTS aui adv core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )

include_directories(../core)

add_executable(reloaded WIN32 
	CapriceAboutImpl.cpp
	CapriceApp.cpp
	CapriceInputSettingsImpl.cpp
	CapriceRegistersDialogImpl.cpp
	CapriceUIWrapper.cpp
	CapriceWindowImpl.cpp
	MemoryImpl.cpp
	../aop/portAudioAudioPlugin.cpp
	WXEmulator.cpp
)

add_dependencies(reloaded ReloadedCore)
target_link_libraries(reloaded ReloadedCore)

target_link_libraries(reloaded ${wxWidgets_LIBRARIES})
if (WIN32)
    target_link_libraries(reloaded portaudio_x86 pthreadVC2)
else (WIN32)
    target_link_libraries(reloaded portaudio pthread)
endif (WIN32)
