/* aylet 0.4, a .AY music file player.
 * Copyright (C) 2001-2005 Russell Marks and Ian Collier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "main.h"
#include "sound.h"
#include "ui.h"
#include "z80.h"

#define AYLET_VER	"0.5"


#define FRAME_STATES_48		(3500000/50)
#define FRAME_STATES_128	(3546900/50)
#define FRAME_STATES_CPC	(4000000/50)


/* see main.h */
struct aydata_tag aydata;
struct time_tag tunetime;

char *progname;

/* start stopping (errr) after this many sec, 0=never.
 * This is too short to allow some things to finish (Agent X 2
 * springs to mind), but *so* many tunes repeat after less than 2 mins
 * that I think this is a sensible default.
 */
int stopafter=3*60;
int fadetime=10;	/* fadeout time *after* that in sec, 0=none */
static int done_fade=0;

/* the memory is a flat all-RAM 64k */
unsigned char mem[64*1024];
unsigned long tstates=0,tsmax=FRAME_STATES_128;
int ay_current_reg=0;
int silent_max=4*50;	/* max frames of silence before skipping */

int highspeed=0;
int playing=1;
int paused=0;
int want_quit=0;

int use_ui=1;
int play_to_stdout=0;
int list_only=0;

char **ay_filenames=NULL;	/* for ptrs to filenames */
int ay_num_files=0;
int ay_file=0;
int ay_track=0;

/* for prev-track - when we skip back a file, this flag
 * indicates that we want to start at the last track.
 */
int go_to_last=0;

/* -1 to run as speccy, allowing only speccy ports;
 *  0 to run as speccy, and allow speccy and CPC ports (initial value);
 *  1 to run as CPC, allowing only CPC ports.
 */
int do_cpc=0;

/* 0 if playing "normal" (or with curses/GTK UI)
 * # of track otherwise
 * track number passed through additional parameter (-t)
 */
int play_one_track_only=0;



unsigned int in(int h,int l)
{
/* presumably nothing? XXX */

return(255);
}


unsigned int out(int h,int l,int a)
{
static int cpc_f4=0;

/* unlike a real speccy, it seems we should only emulate exact port
 * number matches, rather than using bitmasks.
 */
if(do_cpc<1)
  switch(l)
    {
    case 0xfd:
      switch(h)
        {
        case 0xff:
          do_cpc=-1;
        write_reg:
          ay_current_reg=(a&15);
          break;
        case 0xbf:
          do_cpc=-1;
        write_dat:
          sound_ay_write(ay_current_reg,a,tstates);
          break;
        default:
          /* ok, since we do at least have low byte=FDh,
           * do bitmask for top byte to allow for
           * crappy .ay conversions. But don't disable
           * CPC autodetect, just in case.
           */
          if((h&0xc0)==0xc0) goto write_reg;
          if((h&0xc0)==0x80) goto write_dat;
        }
      break;

    case 0xfe:
      do_cpc=-1;
      sound_beeper(a&0x10);
      break;
    }

if(do_cpc>-1)
  switch(h)
    {
    case 0xf6:
      switch(a&0xc0)
        {
        case 0x80:	/* write */
          sound_ay_write(ay_current_reg,cpc_f4,tstates);
          break;
        
        case 0xc0:	/* select */
          ay_current_reg=(cpc_f4&15);
          break;
        }
      break;

    case 0xf4:
      cpc_f4=a;
      if(!do_cpc)
        {
        /* restart as a more CPC-ish emulation */
        do_cpc=1;
        sound_ay_reset_cpc();
        tsmax=FRAME_STATES_CPC;
        if(tstates>tsmax) tstates-=tsmax;
        }
      break;
    }

return(0);	/* additional t-states */
}



void mem_init(int track)
{
static unsigned char intz[]=
  {
  0xf3,		/* di */
  0xcd,0,0,	/* call init */
  0xed,0x5e,	/* loop: im 2 */
  0xfb,		/* ei */
  0x76,		/* halt */
  0x18,0xfa	/* jr loop */
  };
static unsigned char intnz[]=
  {
  0xf3,		/* di */
  0xcd,0,0,	/* call init */
  0xed,0x56,	/* loop: im 1 */
  0xfb,		/* ei */
  0x76,		/* halt */
  0xcd,0,0,	/* call interrupt */
  0x18,0xf7	/* jr loop */
  };
int init,ay_1st_block,ourinit,interrupt;
unsigned char *ptr;
int addr,len,ofs;

#define GETWORD(x) (((*(x))<<8)|(*(x+1)))

init=GETWORD(aydata.tracks[track].data_stacketc+2);
interrupt=GETWORD(aydata.tracks[track].data_stacketc+4);
ay_1st_block=GETWORD(aydata.tracks[track].data_memblocks);

memset(mem+0x0000,0xc9,0x0100);
memset(mem+0x0100,0xff,0x3f00);
memset(mem+0x4000,0x00,0xc000);
mem[0x38]=0xfb;		/* ei */

/* call first AY block if no init */
ourinit=(init?init:ay_1st_block);

if(!interrupt)
  memcpy(mem,intz,sizeof(intz));
else
  {
  memcpy(mem,intnz,sizeof(intnz));
  mem[ 9]=interrupt%256;
  mem[10]=interrupt/256;
  }

mem[2]=ourinit%256;
mem[3]=ourinit/256;

/* now put the memory blocks in place */
ptr=aydata.tracks[track].data_memblocks;
while((addr=GETWORD(ptr))!=0)
  {
  len=GETWORD(ptr+2);
  ofs=GETWORD(ptr+4);
  if(ofs>=0x8000) ofs=-0x10000+ofs;

  /* range check */
  if(ptr-4-aydata.filedata+ofs>=aydata.filelen ||
     ptr-4-aydata.filedata+ofs<0)
    {
    ptr+=6;
    continue;
    }

  /* fix any broken length */
  if(ptr+4+ofs+len>=aydata.filedata+aydata.filelen)
    len=aydata.filedata+aydata.filelen-(ptr+4+ofs);
  if(addr+len>0x10000)
    len=0x10000-addr;
  
  memcpy(mem+addr,ptr+4+ofs,len);
  ptr+=6;
  }
}


/* do action requested by UI code. returns zero if we need to
 * exit the current track.
 */
int action_callback(enum cb_action_tag action)
{
switch(action)
  {
  case cb_quit:
    /* we have to pretend to be playing to get out... :-) */
    playing=1; paused=0;
    want_quit=1;
    return(0);	/* exit track */
    
  case cb_highspeed:
    highspeed=!highspeed;
    ui_change_notify();
    break;

  case cb_prev_file:
  do_prev_file:
    if(ay_file<=0)
      {
      go_to_last=0;	/* wouldn't need that now */
      goto do_stop;
      }
    ay_file--;
    goto do_move;

  case cb_next_file:
  do_next_file:
    if(ay_file>=ay_num_files-1)
      goto do_stop;
    ay_file++;
    goto do_move;

  case cb_prev_track:
    if(ay_track<=0)
      {
      go_to_last=1;
      goto do_prev_file;
      }
    ay_track--;
    goto do_move;

  case cb_next_track:
    if(ay_track>=aydata.num_tracks-1)
      goto do_next_file;
    ay_track++;
  do_move:
    paused=0;
    ui_change_notify();
    return(0);

  case cb_play:
    playing=1;
    if(paused) paused=0;
    ui_change_notify();
    break;

  case cb_pause:
    if(playing)
      paused=!paused,ui_change_notify();
    break;

  case cb_stop:
  do_stop:
    if(playing)
      {
      playing=paused=0;
      ui_change_notify();
      return(0);
      }
    break;

  case cb_restart:
    ui_change_notify();
    return(0);
    
  case cb_dec_stopafter:
    if(!(stopafter%30))
      stopafter-=30;
    else
      stopafter-=stopafter%30;
    if(stopafter<0) stopafter=10*60;
    ui_change_notify();
    break;

  case cb_inc_stopafter:
    if(!(stopafter%30))
      stopafter+=30;
    else
      stopafter+=(30-stopafter%30);
    if(stopafter>10*60) stopafter=0;
    ui_change_notify();
    break;

  case cb_dec_fadetime:
    fadetime--;
    if(fadetime<1) fadetime=20;
    ui_change_notify();
    break;
    
  case cb_inc_fadetime:
    fadetime++;
    if(fadetime>20) fadetime=1;
    ui_change_notify();
    break;

  case cb_dec_vol:
  case cb_inc_vol:
    /* XXX NYI */
    break;

  default:	/* for cb_none */
    break;
  }

return(1);
}


/* rets zero if we want to exit the emulation (i.e. exit track) */
int do_interrupt(void)
{
static int count=0;
static int silent_for=0;

count++;
if(count>=4) count=0;

if(!playing || paused)
  usleep(20000);
else
  {
  /* check for fade needed */
  if(!done_fade && stopafter && tunetime.min*60+tunetime.sec>=stopafter)
    {
    done_fade=1;
    sound_start_fade(fadetime);
    }

  /* incr time */
  tunetime.subsecframes++;
  if(tunetime.subsecframes>=50)
    {
    tunetime.subsecframes=0;
    tunetime.sec++;
    if(tunetime.sec>=60)
      {
      tunetime.sec=0;
      tunetime.min++;
      }
    }

  /* play frame, and stop if it's been silent for a while */
  if(!sound_frame(count==0 || !highspeed))
    silent_for++;
  else
    silent_for=0;
  if(silent_for>=silent_max)
    {
    silent_for=0;
    ui_change_notify();
    /* do next track, or file, or just stop */
    /* if play_one_track_only is set, then finish now */
    if (play_one_track_only)
      {
      want_quit=1;
      return(0);
      }
    ay_track++;
    if(ay_track>=aydata.num_tracks)
      {
      ay_track=0;
      ay_file++;
      if(ay_file>=ay_num_files)
        {
        /* return to first file/track (except for non-UI,
         * to save any pointless reload), and stop.
         */
        if(use_ui)
          ay_file=ay_track=0;
        else
          ay_file--,ay_track=aydata.num_tracks-1;
        playing=0;
        }
      }
    return(0);
    }
  }

return(ui_frame());
}


int read_ay_file(char *filename)
{
FILE *in;
unsigned char *data,*buf,*ptr,*ptr2;
int data_alloc=16384,buf_alloc=16384,data_ofs=0;
int data_len;
int ret,tmp,f;

/* given the loopy format, it's much easier to deal with in memory.
 * But I'm avoiding mmap() in case I want to tweak this to run from
 * a pipe at some point.
 */
if((buf=malloc(buf_alloc))==NULL)
  return(0);

if((data=malloc(data_alloc))==NULL)
  {
  free(buf);
  return(0);
  }

if((in=fopen(filename,"rb"))==NULL)
  {
  free(buf);
  free(data);
  return(0);
  }

while((ret=fread(buf,1,buf_alloc,in))>0)
  {
  if(data_ofs+ret>=data_alloc)
    {
    unsigned char *oldptr=data;

    data_alloc+=buf_alloc;
    if((data=realloc(data,data_alloc))==NULL)
      {
      fclose(in);
      free(oldptr);
      free(buf);
      return(0);
      }
    }

  memcpy(data+data_ofs,buf,ret);
  data_ofs+=ret;
  }

free(buf);

if(ferror(in))
  {
  fclose(in);
  free(data);
  return(0);
  }

fclose(in);

data_len=data_ofs;

if(memcmp(data,"ZXAYEMUL",8)!=0)
  {
  free(data);
  return(0);
  }

/* for the rest, we don't parse that much; just make copies of the
 * offset `pointers' as real pointers, and save all the `top-level'
 * stuff.
 */

aydata.tracks=NULL;

#define READWORD(x)	(x)=256*(*ptr++); (x)|=*ptr++
#define READWORDPTR(x)	READWORD(tmp); \
		if(tmp>=0x8000) tmp=-0x10000+tmp; \
		if(ptr-data-2+tmp>=data_len || ptr-data-2+tmp<0) \
		  { \
                  free(data); \
                  if(aydata.tracks) free(aydata.tracks); \
		  return(0); \
                  } \
		(x)=ptr-2+tmp
#define CHECK_ASCIIZ(x) \
		if(!memchr((x),0,data+data_len-(x))) \
		  { \
                  free(data); \
                  if(aydata.tracks) free(aydata.tracks); \
		  return(0); \
                  }

ptr=data+8;
aydata.filever=*ptr++;
aydata.playerver=*ptr++;
ptr+=2;		/* skip `custom player' stuff */
READWORDPTR(aydata.authorstr);
CHECK_ASCIIZ(aydata.authorstr);
READWORDPTR(aydata.miscstr);
CHECK_ASCIIZ(aydata.miscstr);
aydata.num_tracks=1+*ptr++;
aydata.first_track=*ptr++;

/* skip to track info */
READWORDPTR(ptr2);
ptr=ptr2;

if((aydata.tracks=malloc(aydata.num_tracks*sizeof(struct ay_track_tag)))==NULL)
  {
  free(data);
  return(0);
  }

for(f=0;f<aydata.num_tracks;f++)
  {
  READWORDPTR(aydata.tracks[f].namestr);
  CHECK_ASCIIZ(aydata.tracks[f].namestr);
  READWORDPTR(aydata.tracks[f].data);
  }

for(f=0;f<aydata.num_tracks;f++)
  {
  if(aydata.tracks[f].data-data+10>data_len-4)
    {
    free(aydata.tracks);
    free(data);
    return(0);
    }

  ptr=aydata.tracks[f].data+10;
  READWORDPTR(aydata.tracks[f].data_stacketc);
  READWORDPTR(aydata.tracks[f].data_memblocks);
  
  ptr=aydata.tracks[f].data+4;
  READWORD(aydata.tracks[f].fadestart);
  READWORD(aydata.tracks[f].fadelen);
  }

/* ok then, that's as much parsing as we do here. */

aydata.filedata=data;
aydata.filelen=data_len;
return(1);
}


static void tunetime_reset(void)
{
tunetime.min=tunetime.sec=tunetime.subsecframes=0;
done_fade=0;
}


/* read a file and play it. sound_init() should already have been called,
 * and sound device should still be open when we exit.
 */
void mainloop(void)
{
int oldfile=-1;

aydata.filedata=NULL;
aydata.tracks=NULL;

/* this is kind of a weird multi-level event loop (if
 * you consider do_interrupt()); it's difficult to do it very
 * differently without turning the Z80 emulation inside-out.
 */
while(!want_quit)
  {
  /* load a new file if we need to */
  if(ay_file!=oldfile)
    {
    if(aydata.tracks) free(aydata.tracks);
    if(aydata.filedata) free(aydata.filedata);
    
    if(!read_ay_file(ay_filenames[ay_file]))
      {
      ui_end();
      if(sound_enabled) sound_end();
      fprintf(stderr,"%s: reading `%s' failed.\n",
              progname,ay_filenames[ay_file]);
      exit(1);
      }

    if(!play_one_track_only)
      ay_track=0;
    else
      {
      ay_track=play_one_track_only-1;
      
      if(ay_track>=aydata.num_tracks)
        {
        ui_end();
        if(sound_enabled) sound_end();
        fprintf(stderr,"%s: `%s' has only %d track%s.\n",
                progname,ay_filenames[ay_file],
                aydata.num_tracks,(aydata.num_tracks==1)?"":"s");
        exit(1);
        }
      }

    if(go_to_last)
      {
      go_to_last=0;
      ay_track=aydata.num_tracks-1;
      }
    }
  
  oldfile=ay_file;

  /* only do the whole emulation thing if we're actually playing... */
  if(playing)
    {
    /* re-enable sound after stopping, if needed */
    if(!sound_enabled && !sound_init())
      {
      ui_end();
      fprintf(stderr,"%s: couldn't reopen sound device.\n",progname);
      exit(1);
      }
  
    ay_current_reg=0;
    sound_ay_reset();
    mem_init(ay_track);
    tunetime_reset();
    tsmax=FRAME_STATES_128;
    do_cpc=0;
    z80loop(aydata.tracks[ay_track].data,
            aydata.tracks[ay_track].data_stacketc);
    }

  /* if stopped, close sound device */
  if(sound_enabled && !playing)
    sound_end();
  
  /* do reset now, so any paused/stopped status time makes sense */
  tunetime_reset();
  while((!playing || paused) && ay_file==oldfile)
    do_interrupt();
  }

free(aydata.tracks);
free(aydata.filedata);
}


void usage_help(void)
{
printf("%s " AYLET_VER
	" - copyright (C) 2001-2005 Russell Marks and Ian Collier.\n\n",progname);
printf("usage: %s [-BhlmnNsS] [-A stopafter] [-F fadetime] [-t tracknum]\n"
	"\t\tfile [file2 ...]\n",
       progname);
puts("\n"
"	-A	set stop-after time in seconds (the time at which tracks\n"
"		start fading out), default 180 seconds (3 minutes).\n"
"\n"
"	-B	use ABC stereo (default ACB).\n"
"\n"
"	-e	force 8-bit output even if 16-bit playback is possible.\n"
"\n"
"	-F	set fade-out time in seconds (the time tracks take to\n"
"		fade out), default 10 seconds.\n"
"\n"
"	-h	give this help.\n"
"\n"
"	-l	list contents of the files, rather than playing them.\n"
"\n"
"	-m	use mono rather than the default stereo.\n"
"\n"
"	-n	use a simple batch-playing tty style, rather than the\n"
"		usual interface. (Only works in the curses version.)\n"
"\n"
"	-N	use narrower AY stereo separation.\n"
"\n"
"	-s	output sample data to stdout rather than playing it;\n"
"		implies `-n'. The sample is 44.1kHz 16-bit stereo using\n"
"		the machine's native byte ordering (or mono if using `-m',\n"
"		and 8-bit if using `-e').\n"
"\n"
"	-S	use fake pseudo-stereo for the beeper.\n"
"\n"
"	-t	play specified track, then quit.\n");
}


void parseoptions(int argc,char *argv[])
{
int done=0;

opterr=0;

do
  switch(getopt(argc,argv,"A:BeF:hlmnNsSt:"))
    {
    case 'A':	/* stopafter */
      stopafter=atoi(optarg);
      if(stopafter<0) stopafter=0;
      if(stopafter>10*60) stopafter=10*60;
      break;
    case 'B':	/* ABC stereo (not ACB) */
      sound_stereo_ay_abc=1;
      break;
    case 'e':	/* eight-bit-only */
      sixteenbit=0;
      break;
    case 'F':	/* fadetime */
      fadetime=atoi(optarg);
      if(fadetime<1) fadetime=1;
      if(fadetime>20) fadetime=20;
      break;
    case 'h':
      usage_help();
      exit(0);
    case 'l':	/* list tracks etc. rather than playing */
      list_only=1;
      break;
    case 'm':	/* mono */
      sound_stereo=0;
      break;
    case 'N':	/* narrow stereo separation */
      sound_stereo_ay_narrow=1;
      break;
    case 'n':	/* no UI (effective in non-X ver only) */
      use_ui=0;
      break;
    case 'S':	/* pseudostereo (for beeper) */
      sound_stereo_beeper=1;
      break;
    case 's':	/* play to stdout */
      use_ui=0;		/* implied */
      play_to_stdout=1;
      break;
    case 't':
      if (!(play_one_track_only=atoi(optarg))) {
         fprintf(stderr, "%s: error parsing an argument.\n This should be "
                         "a numerical value betwen 1 and num_tracks.\n",
                         progname);
         exit(1);
      }
      break;
    case '?':
      switch(optopt)
        {
        case 'A':
          fprintf(stderr,"%s: "
                  "the `-A' option requires a stop-after time (in seconds).\n",
                  progname);
          break;
        case 'F':
          fprintf(stderr,"%s: "
                  "the `-F' option requires a fade-out time (in seconds).\n",
                  progname);
          break;
        case 't':
          fprintf(stderr,"%s: the -t option requires a track number to "
                          "play.\n",progname);
          break;
        default:
          fprintf(stderr,"%s: "
                  "option `%c' not recognised.\n",
                  progname,optopt);
        }
      exit(1);
    case -1:
      done=1;
    }
while(!done);

if(optind>=argc)	/* if no filenames given... */
  {
  fprintf(stderr,"%s: you must specify the file(s) to %s.\n",
          progname,list_only?"list":"play");
  exit(1);
  }

ay_filenames=argv+optind;
ay_num_files=argc-optind;
}


void do_list(void)
{
char *ptr;
int f,g;

aydata.filedata=NULL;
aydata.tracks=NULL;

for(f=0;f<ay_num_files;f++)
  {
  if(!read_ay_file(ay_filenames[f]))
    {
    fprintf(stderr,"%s: reading `%s' failed.\n",
            progname,ay_filenames[f]);
    continue;
    }
  
  ptr=strrchr(ay_filenames[f],'/');
  printf("\n  File:\t%s\n  Misc:\t%s\nAuthor:\t%s\nTracks:\t%3d\n",
         ptr?ptr+1:ay_filenames[f],aydata.miscstr,aydata.authorstr,
         aydata.num_tracks);
  for(g=0;g<aydata.num_tracks;g++)
    printf("\t%3d - %s\n",g+1,aydata.tracks[g].namestr);
  
  if(aydata.tracks) free(aydata.tracks);
  if(aydata.filedata) free(aydata.filedata);
  }
}


int main(int argc,char *argv[])
{
char *ptr;

progname=argv[0]?argv[0]:"aylet";
if((ptr=strrchr(progname,'/'))!=NULL)
  progname=ptr+1;

parseoptions(argc,argv);

if(list_only)
  {
  do_list();
  exit(0);
  }

/* even when not using curses/GTK+ we go via the UI code
 * (apart from the above :-)).
 */
ui_init(argc,argv);

if(!sound_init())
  {
  ui_end();
  fprintf(stderr,"%s: couldn't open sound device.\n",progname);
  exit(1);
  }

mainloop();

if(sound_enabled)
  sound_end();

ui_end();

exit(0);
}
