#include "BackOffSpinLock.hpp"
#include "../CommonUtil.hpp"

BackOffSpinLock::BackOffSpinLock() noexcept : minDelay(1), maxDelay(64), currentDelay(minDelay) {
    
}

void BackOffSpinLock::lock() noexcept {
    while (!try_lock()) {
        // Sleep for a random duration (exponential back-off)
#ifdef _PLATFORM_INDEPENDENCE_AT_THREAD
        CommonUtil::sleepThread(std::chrono::milliseconds(getRandomDelay(currentDelay)));
#else
        CommonUtil::sleepThread(struct timespec { 0, getRandomDelay(currentDelay) * 1000000 });
#endif
        currentDelay = std::min(2 * currentDelay, maxDelay);
    }
}

bool BackOffSpinLock::try_lock() noexcept {
    return lockFlag.test_and_set(std::memory_order_acquire);
}

void BackOffSpinLock::unlock() noexcept {
    lockFlag.clear(std::memory_order_release);
}

int BackOffSpinLock::getRandomDelay(int currentDelay) noexcept {
    return CommonUtil::getRandomNumber(currentDelay / 2, currentDelay);
}
