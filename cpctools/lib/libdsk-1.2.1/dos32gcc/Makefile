CC=gcc
CFLAGS=-g -Wall -I. -I../include -I../lib -DDOS32FLOPPY

OBS =   compbz2.o     dskerror.o    dskread.o    drvdos32.o   rpccli.o \
	compgz.o      drvlinux.o    dskfmt.o     dsksecid.o   rpcserv.o \
	compress.o    drvmyz80.o    dskgeom.o    dskseek.o    rpcpack.o \
	compsq.o      drvposix.o    dskjni.o     dsksgeom.o   rpcmap.o \
	drvdos16.o    drvwin16.o    dsklphys.o   dskstat.o    drvnwasp.o \
	drvcfi.o      drvwin32.o    dskopen.o    dsktread.o   dskcmt.o \
	drvcpcem.o    dskcheck.o    dskpars.o    dskwrite.o   dskretry.o  \
	drvadisk.o    drvrcpm.o     drvqm.o      dskretry.o   dskcmt.o \
	dskreprt.o    crctable.o    dskdirty.o   dskrtrd.o    dsktrkid.o \
	remote.o      rpcfossl.o    crc16.o      drvint25.o   drvtele.o \
	drvlogi.o

OBS1 = dskid.o       utilopts.o    libdsk.a
OBS2 = dskform.o     utilopts.o    formname.o   libdsk.a
OBS3 = dsktrans.o    utilopts.o    formname.o   bootsec.o libdsk.a
OBS4 = dskdump.o     utilopts.o    formname.o   libdsk.a
OBS5 = dskscan.o     utilopts.o    formname.o   libdsk.a
OBS6 = dskutil.o     utilopts.o    formname.o   libdsk.a
OBS7 = serslave.o    crc16.o       libdsk.a
OBS8 = apriboot.o    utilopts.o    formname.o   bootsec.o libdsk.a

all:	libdsk.a dskid.exe dsktrans.exe dskform.exe dskscan.exe dskdump.exe \
	dskutil.exe serslave.exe apriboot.exe

dskid.exe:	$(OBS1)
	$(CC) -o $@ $(CFLAGS) $(OBS1) 

dskform.exe:	$(OBS2)
	$(CC) -o $@ $(CFLAGS) $(OBS2)

dsktrans.exe:	$(OBS3)
	$(CC) -o $@ $(CFLAGS) $(OBS3) 

dskdump.exe:	$(OBS4)
	$(CC) -o $@ $(CFLAGS) $(OBS4) 

dskscan.exe:	$(OBS5)
	$(CC) -o $@ $(CFLAGS) $(OBS5) 

dskutil.exe:	$(OBS6)
	$(CC) -o $@ $(CFLAGS) $(OBS6) 

serslave.exe:	$(OBS7)
	$(CC) -o $@ $(CFLAGS) $(OBS7) 

apriboot.exe:	$(OBS8)
	$(CC) -o $@ $(CFLAGS) $(OBS8) 

libdsk.a:	$(OBS) 
	ar rc $@ $(OBS)
	ranlib $@

%.o:	../lib/%.c
	$(CC) -c $(CFLAGS) $<

%.o:	../tools/%.c
	$(CC) -c $(CFLAGS) $<

clean:
	del dskid.exe
	del dskform.exe
	del dsktrans.exe
	del dskscan.exe
	del dskdump.exe
	del serslave.exe
	del libdsk.a
	del *.o
