/* aylet 0.2, a .AY music file player.
 * Copyright (C) 2001 Russell Marks and Ian Collier. See main.c for licence.
 *
 * driver.h - function prototypes for audio drivers.
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int driver_init(int *freqptr,int *stereoptr);
extern void driver_end(void);
extern void driver_frame(signed short *data,int len);

#ifdef __cplusplus
}
#endif
