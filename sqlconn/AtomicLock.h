#ifndef ATOMIC_LOCK_H_H__ // #include guards
#define ATOMIC_LOCK_H_H__

#include "atomicops.h"

struct AtomicLock
{
    void lock() noexcept
    {
        while(flag.test_and_set());
    }

    void unlock() noexcept
    {
        flag.clear();
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

#endif
