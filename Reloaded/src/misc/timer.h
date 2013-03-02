#ifndef __TIMER_H__
#define __TIMER_H__

#if _POSIX_C_SOURCE >= 199309L
	#include <ctime>
	#define _USE_CLOCK_GETTIME
//	#pragma message "Use clock_gettime"
#elif defined(__APPLE__)
	#include "clock_gettime_stub.h"
	#define _USE_CLOCK_GETTIME
//	#pragma message "Use clock_gettime"
#elif defined(__WIN32__) || defined(_WIN32)
	#include <windows.h>
	#define _USE_QUERYPERFORMANCE
//	#pragma message "Use QueryPerformanceTimer"
#elif defined(__HAIKU__)
	#include <OS.h>
	#define _USE_BEAPI
#else
	#include <SDL_timer.h>
	#define _USE_SDLTIMER
	#pragma message "Use sdltimer"
#endif

#include <iostream>
#include <cstdio>

class Timer
{
	private:
		#ifdef _USE_CLOCK_GETTIME
		unsigned int refTime;
		#endif

		#ifdef _USE_QUERYPERFORMANCE
		LARGE_INTEGER ticksPerMillisecond;
		LARGE_INTEGER ticks;
		#endif

		#ifdef _USE_BEAPI
		bigtime_t refTime;
		#endif

		unsigned int startTime;
		unsigned int workingTime;

		bool started;
		bool paused;
	public:
		inline Timer(): startTime(0), workingTime(0), started(false), paused(false)
		{
			#ifdef _USE_CLOCK_GETTIME
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			refTime = t.tv_sec;
			#endif

			#ifdef _USE_QUERYPERFORMANCE
			QueryPerformanceFrequency(&ticksPerMillisecond);
			ticksPerMillisecond.QuadPart /= 1000;
			if (ticksPerMillisecond.QuadPart <1) printf("Your computer is too slow !\n");
			#endif

			#ifdef USE_BEAPI
			refTime = real_time_clock_usecs();
			#endif
		}

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
		}

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
			#endif

			#ifdef _USE_CLOCK_GETTIME
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			return 1000*(t.tv_sec - refTime) + t.tv_nsec/1000000;
			#endif

			#ifdef _USE_QUERYPERFORMANCE
			QueryPerformanceCounter(&ticks);
			return ticks.QuadPart/ticksPerMillisecond.QuadPart;
			#endif

			#ifdef _USE_BEAPI
			return (real_time_clock_usecs() - refTime) / 1000L;
			#endif
		}

		inline bool is_started() const { return started; }
		inline bool is_paused() const { return paused; }
};

// Stupid windows.h defines this to DrawTextA, making wxWidgets unhappy...
#undef DrawText

#endif
