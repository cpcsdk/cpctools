#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_

#include <pthread.h>
#include <cerrno>


class Sync
{
    public:
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual bool tryLock() = 0;
};


// TODO: Write
// SpinLock
class SpinSync : public Sync
{
    private:
        volatile unsigned int flag;
    public:
        SpinSync(): flag(0) {}
        ~SpinSync();
};


class SysSync : public Sync
{
    public:
        SysSync();
        ~SysSync();
        void lock();
        void unlock();
        bool tryLock();
    private:
#if defined _WIN32
        CRITICAL_SECTION mlock;
# if defined VISTA_AND_HIGHER
        CONDITION_VARIABLE cond;
# endif
#else
        pthread_mutex_t mlock;
        pthread_cond_t cond;
#endif
};

#endif /* #ifndef _THREAD_H_ */
