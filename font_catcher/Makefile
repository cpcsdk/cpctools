LINK = g++
LINKFLAGS = $(shell pkg-config --libs ImageMagick++)
CXXFLAGS = $(shell pkg-config --cflags ImageMagick++)

fontcatcher:
	$(LINK) fontcatcher.cpp -o fontcatcher $(LINKFLAGS) $(CXXFLAGS)
