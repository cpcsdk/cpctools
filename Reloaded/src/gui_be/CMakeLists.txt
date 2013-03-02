# In this directory we build the Haiku GUI

# TODO we replicate a lot of the src/gui dir in here. Cleanup !
include_directories(..)
include_directories(keymap)

add_executable(reloaded
	RWin.cpp
	REmulator.cpp
	RApp.cpp
	keymap/KeyboardLayout.cpp
	keymap/KeyboardLayoutView.cpp
	keymap/KeymapWindow.cpp
	keymap/Keymap.cpp
	keymap/Keymap2.cpp
)

add_dependencies(reloaded ReloadedCore)
target_link_libraries(reloaded ReloadedCore)

target_link_libraries(reloaded stsound be game tracker)
