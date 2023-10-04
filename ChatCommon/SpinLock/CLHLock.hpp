#pragma once
#include <atomic>
#include <thread>
#include "../CommonUtil.hpp"
#include "Lock.hpp"

class CLHLock {
public:
    CLHLock() noexcept;
    ~CLHLock() noexcept;

    void lock() noexcept;
    void unlock() noexcept;

private:
    struct CLHNode {
        bool locked{ true };
        CLHNode* prev{ nullptr };
    private:
        uint8_t padding[CommonUtil::GetCacheLineSize() - sizeof(bool) - sizeof(CLHNode*)]{};
    };

    std::atomic<CLHNode*> tail{ new CLHNode{} };
    thread_local static CLHNode myNode;
};