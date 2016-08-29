#!/bin/bash

LIBDSK_HEADERS_DIR=/usr/local/include/


# Build the files
sed -e '1i#include <cstdlib>' -i $LIBDSK_HEADERS_DIR/libdsk.h && \
	cmake -DLIBDSK_HEADERS_DIR=$LIBDSK_HEADERS_DIR /cpctools/cpctools &&
	make createSnapshot  aft2 damsConverter


# Launch unit tests
echo TODO - Write the tests
