FROM ubuntu:16.04
MAINTAINER Romain Giot <giot.romain@gmail.com>

ENV LIBDSK_URL  http://www.seasip.info/Unix/LibDsk/libdsk-1.4.0.tar.gz

RUN apt-get update && \
	apt-get install -qy \
		cmake \
		build-essential \
		wget


WORKDIR /tmp
RUN wget ${LIBDSK_URL} -O- | \
	tar -xzf - && \
	cd libdsk-* && \
	./configure && \
	make -j2 && \
	make install && \
	rm -rf ../libdsk-*

RUN mkdir /build 
WORKDIR /build


ADD docker-build.sh /build
