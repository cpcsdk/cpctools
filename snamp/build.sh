g++ -g main.cpp \
../Reloaded/src/core/emulator.cpp  \
../Reloaded/src/core/video.cpp  \
../Reloaded/src/core/libcapricecore.a  \
-lportaudio -lstsound -lpthread -o snamp -I../Reloaded/src/core/ -I../Reloaded/
