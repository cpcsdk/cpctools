# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
#SET(COMPILER_PREFIX i586-mingw32msvc)
SET(COMPILER_PREFIX i686-w64-mingw32)
SET(CMAKE_C_COMPILER ${COMPILER_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}-g++)
SET(CMAKE_RC_COMPILER ${COMPILER_PREFIX}-windres)
