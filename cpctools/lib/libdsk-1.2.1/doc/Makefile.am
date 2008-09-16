
noinst_DATA = libdsk.lyx libdsk.pdf libdsk.txt protocol.txt \
	      COPYING  TODO INSTALL README cfi.html apridisk.html

EXTRA_DIST = $(noinst_DATA)

%.txt:	%.lyx
	lyx -e text $<

%.tex:	%.lyx
	lyx -e latex $<

%.pdf:	%.lyx
	lyx -e pdf $<
