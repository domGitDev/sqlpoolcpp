#ifndef ATOMIC_LOCK_H_H__ // #include guards
#define ATOMIC_LOCK_H_H__

#include "atomicops.h"

class AtomicLock
{
public:
    AtomicLock() : flag(ATOMIC_FLAG_INIT) {}

    void lock() noexcept
    {
        while(flag.test_and_set());
    }

    void unlock() noexcept
    {
        flag.clear();
    }

private:
    std::atomic_flag flag;
};

#endif
