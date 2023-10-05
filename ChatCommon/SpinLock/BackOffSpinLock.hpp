#pragma once
#include <atomic>
#include "Lock.hpp"

class BackOffSpinLock : public Lock {
public:
    BackOffSpinLock() noexcept;
    
    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;
    
private:
    int getRandomDelay(int currentDelay) noexcept;
    
#if defined _HAS_CXX20 && _HAS_CXX20 > 0
    std::atomic_flag lockFlag{};
#else
    std::atomic_flag lockFlag = ATOMIC_FLAG_INIT;
#endif
    const int minDelay;
    const int maxDelay;
    int currentDelay;
};