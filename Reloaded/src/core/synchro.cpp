/* Synchronisation object */

#include "synchro.h"
#include <iostream>

#if defined _WIN32
#include <Winbase.h>
#endif

// SpinSync
// Actually work only on PPC and i386
// MAYBE: Fallback to SysSync on other architecutre ?

// TODO: See for other architecture
static inline unsigned int _compareAndExchange32(volatile unsigned int *addr, unsigned int newVal, unsigned int expectedOldVal)
{
    unsigned int oldVal;
# if defined __POWERPC__
    __asm__ __volatile__ ("\n"
            "0:\n\t"
            "   lwarx %0,0,%1\n\t"  // load and reserve
            "   cmpw %0,%3\n\t"     // compare the first 2 operands
            "   bne- 1f\n\t"        // skip if not equal
            "   stwcx. %2,0,%1\n\t" // store new value if still reserved
            "   bne- 0b\n"          // loop if lost reservation
            "1:"                    // exit label
            : "=&r"(oldVal)
            : "r"(addr), "r"(newVal), "r"(expectedOldVal)
            : "cr0", "memory");
# elif defined __i386__ || defined __x86_64__
    __asm__ __volatile__ ("lock cmpxchgl %2,%1"
            : "=a"(oldVal), "=m"(*addr)
            : "q"(newVal), "0"(expectedOldVal)
            : "memory");
#else
#error "Currently unsupported architecture"
#endif
    return oldVal;
}

// TODO: See for other architecture
static inline void _memoryFence(void)
{
#if defined __POWERPC__
    __asm__ __volatile__ ("sync": : :"memory");
#elif defined __i386__ || defined __x86_64__
    __asm__ __volatile__ ("mfence": : :"memory");
#else
    __asm__ __volatile__ ("": : :"memory");
#endif
}

// TODO: See for other architecture
static inline void _busyPause(unsigned int count)
{
    while(count--)
    {
#if defined __POWERPC__
        __asm__ __volatile__ ("ori r0,r0,0"); // fake nop
#elif defined __i386__ || defined __x86_64__
        __asm__ __volatile__ ("pause");
#else
#error "Currently unsupported architecture"
#endif
    }
}

void SpinSync::lock()
{
    unsigned int count = 1;
    while(!tryLock())
    {
        _busyPause(count);
        if(count < 128) count <<= 1;
    }
}

void SpinSync::unlock()
{
    _memoryFence();
    flag = 0;
}

bool SpinSync::tryLock()
{
    bool r = (flag || _compareAndExchange32(&flag, 1, 0)) ? false : true;
    _memoryFence();
    return r;
}

// SysSync

SysSync::SysSync()
{
#if defined _WIN32
    InitializeCriticalSection(&this->mlock);
# if defined VISTA_AND_HIGHER //TODO: See how to detect VISTA and higher system
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
    DeleteCriticalSection(&this->mlock);
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
