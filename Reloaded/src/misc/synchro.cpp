/* Synchronisation object */

#include "synchro.h"
#include <iostream>

#if defined _WIN32
#include <windows.h>
#include <intrin.h>
#endif

// SpinSync
// Actually work only on PPC and i386
// MAYBE: Fallback to SysSync on other architecutre ?

// TODO: See for other architecture
static inline unsigned int _compareAndExchange32(volatile unsigned int *addr, unsigned int newVal, unsigned int expectedOldVal)
{
    unsigned int oldVal;
#if defined __POWERPC__
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
#elif defined __i386__ || defined __x86_64__ // using gcc
    __asm__ __volatile__ ("lock cmpxchgl %2,%1"
            : "=a"(oldVal), "=m"(*addr)
            : "q"(newVal), "0"(expectedOldVal)
            : "memory");
#elif defined _M_IX86 // using VC++
    return _InterlockedCompareExchange((volatile long*)addr, newVal, expectedOldVal);
#elif defined __ARM__ || defined __ARMEL__ || defined __ARMEB__
    // This code doesn't garanty atomicity on SMP system
    // and work only on monoprocessor system
    // TODO: See how to detect ARMv6 to use new atomic system
    unsigned int tmp, cpsrSave;

    __asm__ __volatile__("\n"
            "   mrs %[cpsrSave], cpsr\n"
            "   orr %[tmp], %[cpsrSave], #128\n"
            "   msr cpsr_c, %[tmp]\n" //desactivate irq and save state
            "0:\n"
            "   ldr %[tmp],[%[addr]]\n"
            "   cmp %[tmp],%[expectedOldVal]\n"
            "   bne 1f\n"
            "   swp %[oldVal],%[newVal],[%[addr]]\n"
            "   cmp %[tmp],%[oldVal]\n"
            "   swpne %[tmp],%[oldVal],[%[addr]]\n"
            "   bne 0b\n"
            "1:\n"
            "   msr cpsr_c, %[cpsrSave]\n" //restore irq state
            : [oldVal] "=&r" (oldVal), [tmp] "=&r" (tmp), [cpsrSave] "=&r" (cpsrSave)
            : [addr] "r" (addr), [newVal] "r" (newVal), [expectedOldVal] "r" (expectedOldVal)
            : "cc", "memory");

    /*
    __asm__ __volatile__("\n"
            "mrs    %0, cpsr        @ local_irq_save\n"
            "orr    %1, %0, #128\n"
            "msr    cpsr_c, %1"
            : "=r" (x), "=r" (temp)
            :
            : "memory");

    unsigned long result, tmp;
    __asm__ __volatile__ ("\n"
            "0:\n"_InterlockedCompareExchange
            "   ldr     %1,_InterlockedCompareExchange[%2]\n"
            "   cmp     %1,%4\n"
            "   movne   %0,%1\n"
            "   bne     1f\n"
            "   swp     %0,%3,[%2]\n"
            "   cmp     %1,%0\n"
            "   swpne   %1,%0,[%2]\n"
            "   bne     0b\n"
            "1:"
            : "=&r" (result), "=&r" (tmp)
            : "r" (mem), "r" (newval), "r" (oldval)
            : "cc", "memory");

    __asm__ __volatile__("\n"
            "msr    cpsr_c, %0      @ local_irq_restore\n"
            :
            : "r" (x)
            : "memory");
            */
#else
# error "Currently unsupported architecture"
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
#elif defined _M_IX86
    _ReadWriteBarrier();
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
#elif defined _M_IX86
	YieldProcessor();
#elif defined __ARM__ || defined __ARMEL__ || defined __ARMEB__
        __asm__ __volatile__ ("mov r0,r0");
#else
# error "Currently unsupported architecture"
#endif
    }
}


Sync::~Sync()
{
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
            return false;
    }
#endif
}
