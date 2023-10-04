#pragma once
#include <boost/core/noncopyable.hpp>
#include <atomic>

class SpinLock : private boost::noncopyable {
public:
    void lock();
    void unlock();

private:
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
};