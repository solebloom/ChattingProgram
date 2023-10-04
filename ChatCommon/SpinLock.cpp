#include "SpinLock.hpp"

void SpinLock::lock() {
    while (lock_flag.test_and_set(std::memory_order_acquire)) {
        // Spin Lock
    }
}

void SpinLock::unlock() {
    lock_flag.clear(std::memory_order_release);
}