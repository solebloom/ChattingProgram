#pragma once
#include <atomic>
#include "Lock.hpp"

class TTASSpinLock : public Lock {
public:
    void lock() noexcept;
    bool test() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

private:
#if defined _HAS_CXX20 && _HAS_CXX20 > 0
#define _USE_ATOMIC_FLAG
    std::atomic_flag lockFlag{ ATOMIC_FLAG_INIT };
#else
    std::atomic<bool> lockFlag{ false };
#endif
};