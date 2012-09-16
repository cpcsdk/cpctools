#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <cerrno>

// Interface of synchronization object
class Sync
{
    public:
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual bool tryLock() = 0;

		virtual ~Sync();
};


// SpinLock
// Simple lock object with active waiting and no priority
class SpinSync : public Sync
{
    public:
        SpinSync(): flag(0) {}
        void lock();
        void unlock();
        bool tryLock();
    private:
        volatile unsigned int flag;
};

// SysSync
// Use of system sync object
class SysSync : public Sync
{
    public:
        SysSync();
        ~SysSync();
        void lock();
        void unlock();
        bool tryLock();
    private:
#if defined _WIN32 // Use native object on Windows
        CRITICAL_SECTION mlock;
# if defined VISTA_AND_HIGHER //TODO: See how to detect VISTA and higher system
        CONDITION_VARIABLE cond;
# endif
#else // Use pthread on other system
        pthread_mutex_t mlock;
        pthread_cond_t cond;
#endif
};

#endif /* #ifndef _THREAD_H_ */
