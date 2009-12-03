#ifndef __TIMER_H__
#define __TIMER_H__

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#define _USE_CLOCK_GETTIME
#pragma message "Use clock_gettime"
#else
#include <SDL.h>
#define _USE_SDLTIMER
#pragma message "Use sdltimer"
#endif

#include <iostream>

class Timer
{
	private:
		#ifndef _USE_SDLTIMER
		unsigned int refTime;
		#endif
		unsigned int startTime;
		unsigned int workingTime;
		
		bool started;
		bool paused;
			
	public:
		inline Timer(): startTime(0), workingTime(0), started(false), paused(false)
		{
			#ifndef _USE_SDLTIMER
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			refTime = t.tv_sec;
			#endif
		};

		inline void start()
		{
			if(started == false || paused == true)
			{
				started = true;
				paused = false;
				startTime = currentTime();
			}
		}
		inline void stop()
		{
			if(started == true && paused == false)
			{
				workingTime = workingTime + currentTime() - startTime;
			}
			started = false;
			paused = false;
		};
		inline void pause()
		{
			if(paused == false && started == true)
			{
				paused = true;
				workingTime = workingTime + currentTime() - startTime;
				startTime = 0;
			}
		}
		
		inline unsigned int getTime()
		{
			if(paused == true)
			{
				return workingTime;
			}
			else
			{
				return workingTime + currentTime() - startTime;
			}
		}
		
		inline unsigned int currentTime()
		{
			#ifdef _USE_SDLTIMER
			return SDL_GetTicks();
			#else
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			return 1000*(t.tv_sec - refTime) + t.tv_nsec/1000000;
			#endif
		}
		
		inline bool is_started() { return started; }
		inline bool is_paused() { return paused; }
};

#endif
