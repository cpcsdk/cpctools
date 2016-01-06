/*
 *   Reloded, an Amstrad CPC emulator
 *   Copyright (C) 2015 cpcsdk crew
 *
 *   This file is distributed under the terms of the MIT license
 */

#ifdef _MSC_VER
#include <windows.h>
#else
#include <semaphore.h>
#endif

class Semaphore {
	public:
		inline Semaphore() {
#ifdef _MSC_VER
			// FIXME what is a reasonable max value here? Is INT_MAX ok?
			fNativeSem = CreateSemaphore(NULL, 0, INT_MAX, NULL);
#else
			sem_init(&fNativeSem, 0, 0);
#endif
		}

		inline ~Semaphore() {
#ifdef _MSC_VER
			CloseHandle(fNativeSem);
#else
			sem_destroy(&fNativeSem);
#endif
		}

		inline void Wait() {
#ifdef _MSC_VER
			WaitForSingleObject(fNativeSem, INFINITE);
#else
			sem_wait(&fNativeSem);
#endif
		}

		inline void Post() {
#ifdef _MSC_VER
			ReleaseSemaphore(fNativeSem, 1, NULL);
#else
			sem_post(&fNativeSem);
#endif
		}

	private:
#if _MSC_VER
		HANDLE fNativeSem;
#else
		sem_t fNativeSem;
#endif
};
