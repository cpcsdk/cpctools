/* aylet 0.2, a .AY music file player.
 * Copyright (C) 2001 Russell Marks and Ian Collier. See main.c for licence.
 *
 * ui.c - curses UI code (and no-UI UI :-)).
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <curses.h>
#include "main.h"

#include "ui.h"


static int frame_x=1,frame_y=5;	/* frame_y is adjusted */
/* XXX should adjust frame_x when >80 cols, too... */

static int used_curses_ui=0,need_update=1,need_nexttrack=0;


static void clearscreen(void)
{
wclear(stdscr);
move(0,0); refresh();
}


static void draw_frame(void)
{
int y=frame_y;

move(frame_y,frame_x);
addstr(",--------------------------------------");
addstr("--------------------------------------.");
move(frame_y+12,frame_x);
addstr("`--------------------------------------");
addstr("--------------------------------------'");
move(frame_y+6,frame_x+1);
addstr("--------------------------------------");
addstr("--------------------------------------");
move(frame_y+9,frame_x+1);
addstr("--------------------------------------");
addstr("--------------------------------------");

for(y=frame_y+1;y<frame_y+12;y++)
  {
  mvaddch(y,frame_x,'|');
  mvaddch(y,frame_x+77,'|');
  }

mvaddstr(frame_y+1,frame_x+2,"   File:");
mvaddstr(frame_y+2,frame_x+2,"   Misc:");
mvaddstr(frame_y+3,frame_x+2," Author:");
mvaddstr(frame_y+4,frame_x+2," Tracks:");
mvaddstr(frame_y+5,frame_x+2,"Playing:");
mvaddstr(frame_y+7,frame_x+2," Status:");
mvaddstr(frame_y+8,frame_x+2,"   Time:");
for(y=frame_y+7;y<=frame_y+8;y++)
  {
  mvaddch(y,frame_x+24,'|');
  mvaddch(y,frame_x+52,'|');
  }
mvaddstr(frame_y+7,frame_x+30,"[H]igh-speed:");
mvaddstr(frame_y+7,frame_x+55,"[S]top after:");
mvaddstr(frame_y+8,frame_x+55,"[F]ade time:");

mvaddstr(frame_y+10,frame_x+4,
         "[ z - prev | x - play | c - pause | v - stop | b - next |"
         " r - restart ]");
mvaddstr(frame_y+11,frame_x+12,
         "[ space - next file | del - previous file | q - quit ]");

move(0,0);
refresh();
}


static char *fitwidth(char *str,int decr)
{
static char buf[65+1];	/* width allowed, plus NUL */
int len=strlen(str);
int maxlen=sizeof(buf)-1-decr;

memset(buf,' ',maxlen);
buf[maxlen]=0;

if(str && sizeof(buf)-decr>=1)
  {
  if(len>maxlen) len=maxlen;
  memcpy(buf,str,len);
  }

return(buf);
}


static void draw_status(char *filename,char *misc,
                        char *author,int track,char *playingstr)
{
char *ptr=strrchr(filename,'/');

mvaddstr(frame_y+1,frame_x+11,fitwidth(ptr?ptr+1:filename,0));
mvaddstr(frame_y+2,frame_x+11,fitwidth(misc,0));
mvaddstr(frame_y+3,frame_x+11,fitwidth(author,0));
mvprintw(frame_y+4,frame_x+11,"%3d",aydata.num_tracks);
mvprintw(frame_y+5,frame_x+11,"%3d - %s",track,fitwidth(playingstr,6));

mvaddstr(frame_y+7,frame_x+11,paused?"paused ":(playing?"playing":"stopped"));
mvaddstr(frame_y+7,frame_x+44,highspeed?"on ":"off");

move(frame_y+7,frame_x+69);
if(!stopafter)
  addstr("--:--");
else
  printw("%2d:%02d",stopafter/60,stopafter%60);

mvprintw(frame_y+8,frame_x+68,"%2d sec",fadetime);

move(0,0);
refresh();
}


static void draw_status_timeonly(void)
{
mvprintw(frame_y+8,frame_x+11,"%2d:%02d  ",tunetime.min,tunetime.sec);

move(0,0);
refresh();
}


static int non_ui_frame(void)
{
static int oldfile=-1;
static int oldtrack=-1;

if(need_update)
  {
  need_update=0;
  if(ay_file!=oldfile)
    {
    char *filename=ay_filenames[ay_file];
    char *ptr=strrchr(filename,'/');

    fprintf(stderr,
            "\n   File: %s\n   Misc: %s\n Author: %s\n Tracks: %3d\n",
            ptr?ptr+1:filename,aydata.miscstr,aydata.authorstr,
            aydata.num_tracks);
    }
  if(ay_file!=oldfile || ay_track!=oldtrack)
    {
    oldtrack=ay_track;
    fprintf(stderr,
            "Playing: %3d - %s\n",
            ay_track+1,aydata.tracks[ay_track].namestr);
    }
  oldfile=ay_file;
  }

/* quit if we stop (i.e. when we've played all tracks) */
if(!playing && !paused)
  return(action_callback(cb_quit));

if(need_nexttrack)
  {
  need_nexttrack=0;
  return(action_callback(cb_next_track));
  }

return(1);
}


/* called per 1/50th, this deals with the usual events.
 * returns zero if we want to stop the current track.
 */
int ui_frame(void)
{
enum cb_action_tag action;

if(!use_ui)
  return(non_ui_frame());

if(need_update)
  {
  need_update=0;
  draw_frame();
  draw_status(ay_filenames[ay_file],aydata.miscstr,aydata.authorstr,
              ay_track+1,aydata.tracks[ay_track].namestr);
  }

/* update time display */
draw_status_timeonly();

/* read keys */
action=cb_none;
switch(getch())
  {
  case 27: case 'q':	action=cb_quit; break;
  case 'h':		action=cb_highspeed; break;
#if KEY_BACKSPACE!=8
  case 8:
#endif
#if KEY_DC!=127
  case 127:
#endif
  case KEY_BACKSPACE: case KEY_DC:
    action=cb_prev_file; break;
  case ' ':		action=cb_next_file; break;
  case 'z':		action=cb_prev_track; break;
  case 'b':		action=cb_next_track; break;
  case 'x':		action=cb_play; break;
  case 'c':		action=cb_pause; break;
  case 'v':		action=cb_stop; break;
  case 'r':		action=cb_restart; break;
  case 'S':		action=cb_dec_stopafter; break;
  case 's':		action=cb_inc_stopafter; break;
  case 'F':		action=cb_dec_fadetime; break;
  case 'f':		action=cb_inc_fadetime; break;
  }

if(action==cb_none)
  return(1);

return(action_callback(action));
}


/* called if playback status has changed without us knowing. */
void ui_change_notify(void)
{
need_update=1;
}


static void non_ui_ctrlc(int foo)
{
if(tunetime.min==0 && tunetime.sec==0 &&
   tunetime.subsecframes<25)
  playing=0;	/* quit */
else
  need_nexttrack=1;
}


static void non_ui_init(void)
{
struct sigaction sa;

sa.sa_flags=SA_RESTART;
sa.sa_handler=non_ui_ctrlc;
sigaction(SIGINT,&sa,NULL);

need_update=1;
}


void ui_init(int argc,char **argv)
{
if(!use_ui)
  {
  non_ui_init();
  return;
  }

used_curses_ui=1;

initscr();
cbreak(); noecho();
curs_set(0);	/* remove cursor (if possible) */
keypad(stdscr,TRUE);
nodelay(stdscr,1);

/* XXX do I need to enable SIGWINCH handling in ncurses myself? */

/* check term size */
if(COLS<80 || LINES<13)
  {
  ui_end();
  fprintf(stderr,"aylet: "
  		"sorry, need a terminal with at least 80 cols and 13 lines.\n");
  exit(1);
  }

frame_y=(LINES-13)/2;

clearscreen();
}


void ui_end(void)
{
if(!use_ui || !used_curses_ui) return;

clearscreen();
move(LINES-1,0);
refresh();
nodelay(stdscr,0);
echo(); nocbreak();
endwin();
putchar('\n');

/* in case of error exit, since stderr will be used immediately after... */
fflush(stdout);
}
