vasmz80 loader.s -o m5view.bin -L fx_mode5.lst -Fbin -z80asm && \
iDSK fx5.dsk -n && \
iDSK fx5.dsk -i m5view.bin -e 2000 -c 2000 -t 1 && \
iDSK fx5.dsk -i recursos/goku.cm5 -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/goku.gfx -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/opwolf.cm5 -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/opwolf.gfx -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/pilingis.cm5 -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/pilingis.gfx -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/spidey.cm5 -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/spidey.gfx -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/term.cm5 -e 0000 -c 0000 -t 1 && \
iDSK fx5.dsk -i recursos/term.gfx -e 0000 -c 0000 -t 1 && \
./cap32 fx5.dsk
