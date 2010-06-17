/* Synchronisation object */

#include "synchro.h"
#include <iostream>

SysSync::SysSync()
{
#if defined _WIN32
    InitializeCriticalSection(&this->mlock);
# if defined VISTA_AND_HIGHER
    InitializeConditionVariable(&this->cond);
# endif
#else
//  int r = pthread_mutex_init(&this->lock,(pthread_mutexattr_t *)NULL);
    if(int r = pthread_mutex_init(&this->mlock,NULL))
    {
        std::cerr << "pthread_mutex_init() failure: " << r << std::endl;
    }
    if(int r = pthread_cond_init(&this->cond,NULL))
    {
        std::cerr << "pthread_cond_init() failure: " << r << std::endl;
    }
#endif
}

SysSync::~SysSync()
{
#if defined _WIN32
    DeleteCriticalSection(&this->lock);
#else
    if(int r = pthread_cond_destroy(&this->cond))
    {
        std::cerr << "pthread_cond_destroy() failure: " << r << std::endl;
    }
    if(int r = pthread_mutex_destroy(&this->mlock))
    {
        std::cerr << "pthread_mutex_destroy() failure: " << r << std::endl;
    }
#endif
}

void SysSync::lock()
{
#if defined _WIN32
    EnterCriticalSection(&this->mlock);
#else
    if(int r = pthread_mutex_lock(&this->mlock))
    {
        std::cerr << "pthread_mutex_lock() failure: " << r << std::endl;
    }
#endif
}

void SysSync::unlock()
{
#if defined _WIN32
    LeaveCriticalSection(&this->mlock);
#else
    if(int r = pthread_mutex_unlock(&this->mlock))
    {
        std::cerr << "pthread_mutex_unlock() failure: " << r << std::endl;
    }
#endif
}

bool SysSync::tryLock()
{
#if defined _WIN32
    return TryEnterCriticalSection(&this->mlock) ? true : false;
#else
    switch(int r = pthread_mutex_trylock(&this->mlock))
    {
        case 0:
            return true;
        case EBUSY:
            return false;
        default:
            std::cerr << "pthread_mutex_trylock() failure: " << r << std::endl;
    }
#endif
}
