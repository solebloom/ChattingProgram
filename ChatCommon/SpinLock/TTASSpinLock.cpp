#include "TTASSpinLock.hpp"
#include "../CommonUtil.hpp"

void TTASSpinLock::lock() noexcept {
    while (true) {
        while (test()) {
            CommonUtil::yieldThread();
        }

        if (try_lock()) {
            break;
        }
    }
}

bool TTASSpinLock::test() noexcept {
#ifdef _USE_ATOMIC_FLAG
    return lockFlag.test(std::memory_order_acquire);
#else
    return lockFlag.load(std::memory_order_acquire);
#endif
}

bool TTASSpinLock::try_lock() noexcept {
#ifdef _USE_ATOMIC_FLAG
    return lockFlag.test_and_set(std::memory_order_acquire);
#else
    return lockFlag.exchange(true, std::memory_order_acquire);
#endif
}

void TTASSpinLock::unlock() noexcept {
#ifdef _USE_ATOMIC_FLAG
    lockFlag.clear(std::memory_order_release);
#else
    lockFlag.store(false, std::memory_order_release);
#endif
}
