# generic CPC-project makefile
# Copyright 2011-2013 PulkoMandy - Shinra! (pulkomandy@cpcscene.com)

# TODO
# * I usually do incbin "something.exo" in my sourcecode, but this doesn't get 
#	in the makefile dependancies. Either add something equivalent to gcc -M in 
#	vasm, or find another way... (grep INCBIN + generate dependencies ?)

# GENERIC RULES ###############################################################

# Nice header for doing something
BECHO = @echo -ne "\x1B[46m\t$(1) \x1B[1m$(2)\n\x1B[0m"

vpath %.z80 src

# Build the DSK-File
%.dsk:
	$(call BECHO,"Putting files in","$@")
	cpcfs $@ f
	for i in $^;do cpcfs $@ p $$i ;done;

ifdef DEV
$(NAME).zip:: $(NAME).sna memory.bin memory.C4 memory.C5 memory.C6 memory.C7
else
$(NAME).zip:: $(NAME).dsk $(NAME).nfo
endif

memory.bin memory.C4 memory.C5 memory.C6 memory.C7: $(NAME).sna
	$(call BECHO,"Extracting memory data from ","$@")
	createSnapshot $^ -i $^ \
		-o memory.bin 0x0000 0x10000 \
		-o memory.C4 0x10000 0x4000 \
		-o memory.C5 0x14000 0x4000 \
		-o memory.C6 0x18000 0x4000 \
		-o memory.C7 0x1C000 0x4000

# Build the release zipfile
%.zip: 
	$(call BECHO,"Creating release archive","$@")
	zip $@ $^

# Link the sources ($^ means "all dependencies", so all of them should be .o 
# files - which is good, since anything else should be incbined somewhere)
%.bin: build/link.ld
	$(call BECHO,"Linking","$@")
	vlink -b amsdos -o $@ -T$^
	
# Assemble the sources
%.o: %.z80
	$(call BECHO,"Assembling","$<")
	vasmz80_oldstyle -Fvobj -o $@ $<

# Crunch a screen
%.exo: %.scr
	$(call BECHO,"Crunching","$<")
	exoraw -o $@ $<.noh

# convert png to cpc screen format
# SCREENMODE can force the screenmode, otherwise it's guessed from the png 
# bitdepth
%.scr: data/%.png
	$(call BECHO,"Converting","$<")
	png2crtc $< $@.noh 7 $(SCREENMODE)
	hideur $@.noh -o $@ -t 2 -l 49152

clean:
	$(call BECHO,"Cleaning","everything")
	rm -f *.exo *.bin *.dsk *.o *.zip
