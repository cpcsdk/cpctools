CC=pacc
CFLAGS=-Bl -I. -I../include -I../lib -DDOS16FLOPPY
OBS1=compbz2.obj     dskerror.obj    dskread.obj    drvnwasp.obj  rpccli.obj
OBS2=compgz.obj      drvlinux.obj    dskfmt.obj     dsksecid.obj  rpcserv.obj
OBS3=compress.obj    drvmyz80.obj    dskgeom.obj    dskseek.obj   rpcmap.obj
OBS4=compsq.obj      drvposix.obj    dskjni.obj     dsksgeom.obj  rpcpack.obj
OBS5=drvdos16.obj    drvwin16.obj    dsklphys.obj   dskstat.obj   dskcmt.obj
OBS6=drvcfi.obj      drvwin32.obj    dskopen.obj    dsktread.obj  dskretry.obj
OBS7=drvcpcem.obj    dskcheck.obj    dskpars.obj    dskwrite.obj  dskcmt.obj 
OBS8=drvadisk.obj    drvrcpm.obj     drvqm.obj      crctable.obj  dskretry.obj
OBS9=dskreprt.obj    dskdirty.obj    dskrtrd.obj    dsktrkid.obj  remote.obj
OBSA=rpcfossl.obj    crc16.obj       drvint25.obj   int25l.obj    drvtele.obj
OBSB=drvlogi.obj
OBS10=dskid.obj       utilopts.obj    libdsk.lib
OBS11=dskform.obj     utilopts.obj    formname.obj   libdsk.lib
OBS12=dsktrans.obj   utilopts.obj    formname.obj   bootsec.obj libdsk.lib
OBS13=serslave.obj   libdsk.lib
OBS14=dskscan.obj     utilopts.obj    formname.obj   libdsk.lib
OBS15=dskdump.obj     utilopts.obj    formname.obj   libdsk.lib
OBS16=dskutil.obj     utilopts.obj    formname.obj   libdsk.lib
OBS17=apriboot.obj    utilopts.obj    formname.obj   bootsec.obj libdsk.lib

all:	libdsk.lib dskid.exe dsktrans.exe dskform.exe serslave.exe \
	dskdump.exe dskscan.exe dskutil.exe apriboot.exe

apriboot.exe:	$(OBS17)
	$(CC) -o$@ $(CFLAGS) $(OBS17)

dskid.exe:	$(OBS10)
	$(CC) -o$@ $(CFLAGS) $(OBS10)

dsktrans.exe:	$(OBS12)
	$(CC) -o$@ $(CFLAGS) $(OBS12)

dskscan.exe:	$(OBS14)
	$(CC) -o$@ $(CFLAGS) $(OBS14)

dskdump.exe:	$(OBS15)
	$(CC) -o$@ $(CFLAGS) $(OBS15)

dskutil.exe:	$(OBS16)
	$(CC) -o$@ $(CFLAGS) $(OBS16)

dskform.exe:	$(OBS11)
	$(CC) -o$@ $(CFLAGS) $(OBS11)

serslave.exe:	$(OBS13)
	$(CC) $(CFLAGS) $(OBS13)

libdsk.lib:	$(OBS1) $(OBS2) $(OBS3) $(OBS4) $(OBS5) $(OBS6) $(OBS7) \
		$(OBS8) $(OBS9) $(OBSA) $(OBSB)
	libr r $@ $(OBS1)
	libr r $@ $(OBS2)
	libr r $@ $(OBS3)
	libr r $@ $(OBS4)
	libr r $@ $(OBS5)
	libr r $@ $(OBS6)
	libr r $@ $(OBS7)
	libr r $@ $(OBS8)
	libr r $@ $(OBS9)
	libr r $@ $(OBSA)
	libr r $@ $(OBSB)

%.obj:	%.as
	$(CC) -c $(CFLAGS) $<

%.obj:	../lib/%.c
	$(CC) -c $(CFLAGS) $<

%.obj:	../tools/%.c
	$(CC) -c $(CFLAGS) $<

clean:
	del dskid.exe
	del dskform.exe
	del dsktrans.exe
	del dskdump.exe
	del dskscan.exe
	del dskutil.exe
	del libdsk.lib
	del *.obj
