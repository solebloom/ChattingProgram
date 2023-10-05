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
    
    std::atomic_flag lockFlag{};
    const int minDelay;
    const int maxDelay;
    int currentDelay;
};