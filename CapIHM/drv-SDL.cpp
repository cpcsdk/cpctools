/* aylet 0.4, a .AY music file player.
 * Copyright (C) 2001-2004 Russell Marks and Ian Collier. See main.c for licence.
 *
 */

#include "audio.h"
#include <iostream>
#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRAME_STATES_CPC	(4000000/50)

unsigned long tstates=0,tsmax=FRAME_STATES_CPC;

int driver_init (int *freqptr, int *stereoptr)
{
	return 1;
}

void driver_end (void)
{
}

void driver_frame(signed short *data,int len)
{
//    std::cout << "Writing sample : " << len/2 << " at " << SDL_GetTicks() << std::endl;
    for (int i = 0; i < len; i++)
    {
        *((signed short*)pbSndBufferPtr) = data[i];

        pbSndBufferPtr += 2;  // 16 bits stereo
        if (pbSndBufferPtr >= pbSndBufferEnd)
        {
            pbSndBufferPtr = pbSndBuffer;
        }
    }
}

#ifdef __cplusplus
}
#endif
