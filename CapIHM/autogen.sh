#!/bin/sh

aclocal
autoconf
automake -a -c
./configure
make clean
make
