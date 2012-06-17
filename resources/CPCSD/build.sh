#!/bin/sh
vasmz80_oldstyle cpcsd.z80 -Fbin -o A.BIN && \
cpcfs /dev/disk/ufi/0/0/raw p -b -e A.BIN,0x4000,0x4000
