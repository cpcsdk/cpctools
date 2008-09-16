/* aylet 0.3, a .AY music file player.
 * Copyright (C) 2001-2002 Russell Marks and Ian Collier.
 * See main.c for licence.
 *
 * uigtk.c - GTK+ UI code.
 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "main.h"

#include "ui.h"

/* button pixmaps */
#include "button1.xpm"
#include "button2.xpm"
#include "button3.xpm"
#include "button4.xpm"
#include "button5.xpm"


static GtkWidget *window,*vbox,*detailstbl,*detaillabel[5];
static GtkWidget *label_for_status,*label_for_time;
static GtkWidget *highspeed_widget;
static GtkWidget *label_for_stopafter,*label_for_fadetime;

static int used_ui=0,need_update=1;
static int retval=1;


static void action(enum cb_action_tag a)
{
/* only set retval if we get a zero return, i.e. we need to
 * stop current track.
 */
if(!action_callback(a))
  retval=0;
}


static void cb_button_prev_track(void) { action(cb_prev_track); }
static void cb_button_next_track(void) { action(cb_next_track); }
static void cb_button_play(void) { action(cb_play); }
static void cb_button_pause(void) { action(cb_pause); }
static void cb_button_stop(void) { action(cb_stop); }
static void cb_button_restart(void) { action(cb_restart); }
static void cb_button_prev_file(void) { action(cb_prev_file); }
static void cb_button_next_file(void) { action(cb_next_file); }

static void cb_toggle_highspeed(void)
{
action(cb_highspeed);
}

static void cb_doquit(void)
{
window=NULL;
action(cb_quit);
}


/* shouldn't really be needing this, but I had trouble
 * with a few bits and pieces.
 */
gint keypress(GtkWidget *widget,GdkEventKey *event)
{
switch(event->keyval)
  {
  case GDK_BackSpace: case GDK_Delete:
    action(cb_prev_file);
    break;

  case GDK_space:
    action(cb_next_file);
    break;

  case GDK_q: case GDK_Escape:
    cb_doquit();
    break;

  case GDK_s: case GDK_S:
    action((event->state&GDK_SHIFT_MASK)?cb_dec_stopafter:cb_inc_stopafter);
    break;
    
  case GDK_f: case GDK_F:
    action((event->state&GDK_SHIFT_MASK)?cb_dec_fadetime:cb_inc_fadetime);
    break;
    
  default:
    return(FALSE);	/* don't stop event if not handled */
  }

/* if we handled it, stop anything else getting the event. */
gtk_signal_emit_stop_by_name(GTK_OBJECT(widget),"key_press_event");

return(TRUE);
}


static void ui_initwin(void)
{
GtkWidget *statustbl,*buttontbl;
GtkWidget *sep,*label,*hbox,*button,*ebox;
GtkWidget *pixmap;
GdkBitmap *bitmap,*mask;
int tbl_row,width;

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_signal_connect(GTK_OBJECT(window),"destroy",
                   GTK_SIGNAL_FUNC(cb_doquit),NULL);
gtk_signal_connect(GTK_OBJECT(window),"key_press_event",
                   GTK_SIGNAL_FUNC(keypress),NULL);
gtk_widget_set_events(window,GDK_KEY_PRESS_MASK);
gtk_window_set_title(GTK_WINDOW(window),"xaylet");
gtk_window_set_policy(GTK_WINDOW(window),FALSE,TRUE,FALSE);	/* XXX? */

/* main vbox */
vbox=gtk_vbox_new(FALSE,0);
gtk_container_add(GTK_CONTAINER(window),vbox);
gtk_container_set_border_width(GTK_CONTAINER(vbox),0);
gtk_widget_show(vbox);

/* add file details table */
/* 6 columns used to get the proportions nice - left one is for
 * description (e.g. "Filename:"), right N for value.
 */
detailstbl=gtk_table_new(5,width=6,TRUE);
gtk_box_pack_start(GTK_BOX(vbox),detailstbl,TRUE,TRUE,0);
gtk_table_set_col_spacings(GTK_TABLE(detailstbl),10);
gtk_table_set_row_spacings(GTK_TABLE(detailstbl),2);
gtk_container_set_border_width(GTK_CONTAINER(detailstbl),5);
gtk_widget_show(detailstbl);

/* couldn't get gtk_label_set_justify() to work (perhaps this is
 * only relevant to multi-line labels?), but gtk_misc_set_alignment() is ok.
 */
#define DO_TBL_LEFT(table,row,start,name) \
  label=gtk_label_new(name);				\
  gtk_misc_set_alignment(GTK_MISC(label),1.,0.5);	\
  gtk_table_attach_defaults(GTK_TABLE(table),label,	\
                            (start),(start)+1, (row),(row)+1); \
  gtk_widget_show(label)

#define DO_TBL_RIGHT(table,row,start,end,name) \
  ebox=gtk_event_box_new();				\
  label=gtk_label_new(name);				\
  gtk_misc_set_alignment(GTK_MISC(label),0.,0.5);	\
  gtk_container_add(GTK_CONTAINER(ebox),label);		\
  gtk_widget_set_usize(label,5,5);			\
  gtk_table_attach_defaults(GTK_TABLE(table),ebox,	\
                            (start),(end), (row),(row)+1); \
  gtk_widget_show(label),gtk_widget_show(ebox)

tbl_row=0;
DO_TBL_LEFT(detailstbl,tbl_row,0,"File:");
DO_TBL_RIGHT(detailstbl,tbl_row,1,width,"");
detaillabel[tbl_row]=label;
tbl_row++;
DO_TBL_LEFT(detailstbl,tbl_row,0,"Misc:");
DO_TBL_RIGHT(detailstbl,tbl_row,1,width,"");
detaillabel[tbl_row]=label;
tbl_row++;
DO_TBL_LEFT(detailstbl,tbl_row,0,"Author:");
DO_TBL_RIGHT(detailstbl,tbl_row,1,width,"");
detaillabel[tbl_row]=label;
tbl_row++;
DO_TBL_LEFT(detailstbl,tbl_row,0,"Tracks:");
DO_TBL_RIGHT(detailstbl,tbl_row,1,width,"");
detaillabel[tbl_row]=label;
tbl_row++;
DO_TBL_LEFT(detailstbl,tbl_row,0,"Playing:");
DO_TBL_RIGHT(detailstbl,tbl_row,1,width,"");
detaillabel[tbl_row]=label;


sep=gtk_hseparator_new();
gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,TRUE,0);
gtk_widget_show(sep);


statustbl=gtk_table_new(2,6,TRUE);
gtk_box_pack_start(GTK_BOX(vbox),statustbl,TRUE,TRUE,0);
gtk_table_set_col_spacings(GTK_TABLE(statustbl),10);
gtk_table_set_row_spacings(GTK_TABLE(statustbl),2);
gtk_container_set_border_width(GTK_CONTAINER(statustbl),5);
gtk_widget_show(statustbl);

DO_TBL_LEFT(statustbl,0, 0,"Status:");
DO_TBL_RIGHT(statustbl,0, 1,2,"");
label_for_status=label;
DO_TBL_LEFT(statustbl,1, 0,"Time:");
DO_TBL_RIGHT(statustbl,1, 1,2,"");
label_for_time=label;

highspeed_widget=gtk_check_button_new_with_label("High-speed");
/* don't allow focus, looks too weird and we have shortcut */
GTK_WIDGET_UNSET_FLAGS(highspeed_widget,GTK_CAN_FOCUS);
gtk_table_attach_defaults(GTK_TABLE(statustbl),highspeed_widget, 2,4, 0,2);
gtk_signal_connect(GTK_OBJECT(highspeed_widget),"clicked",
                   GTK_SIGNAL_FUNC(cb_toggle_highspeed),NULL);
gtk_widget_add_accelerator(highspeed_widget,"clicked",
                           gtk_accel_group_get_default(),
                           GDK_h,0,0);
gtk_widget_show(highspeed_widget);


/* XXX these should really be spin buttons */
DO_TBL_LEFT(statustbl,0, 4,"Stop after:");
DO_TBL_RIGHT(statustbl,0, 5,6,"");
label_for_stopafter=label;
DO_TBL_LEFT(statustbl,1, 4,"Fade time:");
DO_TBL_RIGHT(statustbl,1, 5,6,"");
label_for_fadetime=label;


sep=gtk_hseparator_new();
gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,TRUE,0);
gtk_widget_show(sep);


buttontbl=gtk_table_new(2,4,TRUE);
gtk_box_pack_start(GTK_BOX(vbox),buttontbl,TRUE,TRUE,0);
gtk_table_set_col_spacings(GTK_TABLE(buttontbl),5);
gtk_table_set_row_spacings(GTK_TABLE(buttontbl),5);
gtk_container_set_border_width(GTK_CONTAINER(buttontbl),5);
gtk_widget_show(buttontbl);


hbox=gtk_hbox_new(TRUE,5);
gtk_table_attach_defaults(GTK_TABLE(buttontbl),hbox, 0,4, 0,1);
gtk_container_set_border_width(GTK_CONTAINER(hbox),0);
gtk_widget_show(hbox);

/* XXX this seems to be needed here, but is it ok? */
gtk_widget_realize(window);

#define ADD_PIXMAP_TO_BUTTON(button,xpm) \
  bitmap=gdk_pixmap_create_from_xpm_d(window->window,&mask,	\
                                      &(window->style->black),	\
                                      (xpm));			\
  pixmap=gtk_pixmap_new(bitmap,mask);				\
  gtk_container_add(GTK_CONTAINER(button),pixmap);		\
  gtk_widget_show(pixmap)

button=gtk_button_new();
ADD_PIXMAP_TO_BUTTON(button,button1);
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_prev_track),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_z,0,0);
/* since I use Space, there's no point having these focusable... */
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new();
ADD_PIXMAP_TO_BUTTON(button,button2);
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_play),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_x,0,0);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new();
ADD_PIXMAP_TO_BUTTON(button,button3);
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_pause),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_c,0,0);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new();
ADD_PIXMAP_TO_BUTTON(button,button4);
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_stop),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_v,0,0);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new();
ADD_PIXMAP_TO_BUTTON(button,button5);
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_next_track),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_b,0,0);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new_with_label("Restart");
gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_restart),NULL);
gtk_widget_add_accelerator(button,"clicked",gtk_accel_group_get_default(),
                           GDK_r,0,0);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);


/* second row, these go straight on the table */
/* backspace/delete/space are dealt with by keypress() */
button=gtk_button_new_with_label("Prev File");
gtk_table_attach_defaults(GTK_TABLE(buttontbl),button, 1,2, 1,2);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_prev_file),NULL);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);

button=gtk_button_new_with_label("Next File");
gtk_table_attach_defaults(GTK_TABLE(buttontbl),button, 2,3, 1,2);
gtk_signal_connect(GTK_OBJECT(button),"clicked",
                   GTK_SIGNAL_FUNC(cb_button_next_file),NULL);
GTK_WIDGET_UNSET_FLAGS(button,GTK_CAN_FOCUS);
gtk_widget_show(button);


gtk_widget_show(window);
}


static void ui_draw_status(char *filename,char *misc,
                           char *author,int track,char *playingstr)
{
static gchar buf[256];
char *ptr=strrchr(filename,'/');
int tbl_row=0;

if(!window) return;

gtk_label_set_text(GTK_LABEL(detaillabel[tbl_row]),ptr?ptr+1:filename);
tbl_row++;
gtk_label_set_text(GTK_LABEL(detaillabel[tbl_row]),misc);
tbl_row++;
gtk_label_set_text(GTK_LABEL(detaillabel[tbl_row]),author);
tbl_row++;
g_snprintf(buf,sizeof(buf),"%d",aydata.num_tracks);
gtk_label_set_text(GTK_LABEL(detaillabel[tbl_row]),buf);
tbl_row++;
g_snprintf(buf,sizeof(buf),"%d - %s",track,playingstr);
gtk_label_set_text(GTK_LABEL(detaillabel[tbl_row]),buf);

gtk_label_set_text(GTK_LABEL(label_for_status),
                   paused?"paused ":(playing?"playing":"stopped"));

if(!stopafter)
  gtk_label_set_text(GTK_LABEL(label_for_stopafter),"--:--");
else
  {
  g_snprintf(buf,sizeof(buf),"%2d:%02d",stopafter/60,stopafter%60);
  gtk_label_set_text(GTK_LABEL(label_for_stopafter),buf);
  }

g_snprintf(buf,sizeof(buf),"%2d sec",fadetime);
gtk_label_set_text(GTK_LABEL(label_for_fadetime),buf);
}


static void ui_draw_status_timeonly(void)
{
static char buf[32];

if(!window) return;

g_snprintf(buf,sizeof(buf),"%2d:%02d",tunetime.min,tunetime.sec);
gtk_label_set_text(GTK_LABEL(label_for_time),buf);
}


/* called per 1/50th, this deals with the usual events.
 * returns zero if we want to stop the current track.
 */
int ui_frame(void)
{
if(!window) return(0);

retval=1;

if(need_update)
  {
  need_update=0;
  ui_draw_status(ay_filenames[ay_file],aydata.miscstr,aydata.authorstr,
                 ay_track+1,aydata.tracks[ay_track].namestr);
  }

/* update time display */
ui_draw_status_timeonly();

/* this should be the only place retval gets modified by actions */
while(gtk_events_pending())
  gtk_main_iteration();

return(retval);
}


/* called if playback status has changed without us knowing. */
void ui_change_notify(void)
{
need_update=1;
}


void ui_init(int argc,char **argv)
{
used_ui=1;

if(!use_ui)
  {
  fprintf(stderr,"xaylet: warning: "
          "non-UI mode not supported by GTK+ version, use `aylet'.\n");
  use_ui=1;
  }

gtk_init(&argc,&argv);
ui_initwin();
}


void ui_end(void)
{
if(!used_ui) return;

/* XXX can't call gtk_exit(), ui_end() is meant to return!
 * Oh well, it seems to be coping without... :-/
 */
}
