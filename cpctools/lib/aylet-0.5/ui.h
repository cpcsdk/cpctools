/* aylet 0.1, a .AY music file player.
 * Copyright (C) 2001 Russell Marks and Ian Collier. See main.c for licence.
 *
 * ui.h
 */

extern int ui_frame(void);
extern void ui_change_notify(void);
extern void ui_reset_notify(void);
extern void ui_init(int argc,char **argv);
extern void ui_end(void);
