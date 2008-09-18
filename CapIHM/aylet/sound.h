/* aylet 0.2, a .AY music file player.
 * Copyright (C) 2001 Russell Marks and Ian Collier. See main.c for licence.
 *
 * sound.h
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int sound_enabled;
extern int sound_freq;
extern int sound_stereo;
extern int sound_stereo_beeper;
extern int sound_stereo_ay;
extern int sound_stereo_ay_abc;
extern int sound_stereo_ay_narrow;

extern int soundfd;
extern int sixteenbit;
extern int play_to_stdout;

extern int sound_init(void);
extern void sound_end(void);
extern int sound_frame(int really_play);
extern int sound_frameExt(int nbSample, int really_play);
extern void sound_frame_blank(void);
extern void sound_start_fade(int fadetime_in_sec);
extern void sound_ay_write(int reg,int val,unsigned long tstates);
extern int sound_ay_read(int reg);
extern void sound_ay_reset(void);
extern void sound_ay_reset_cpc(void);
extern void sound_beeper(int on);

#ifdef __cplusplus
}
#endif
