#pragma once
#include "CLHLock.hpp"

CLHLock::CLHLock() noexcept {
    // All of 'locked' will be inited 'true', tail's locked must be false because of 'pred->locked' condition at while loop in lock function.
    // If not, it will be endless loop.
    tail.load()->locked = false;
}

CLHLock::~CLHLock() noexcept {
    // Deallocate memory used for CLHNode objects in the lock queue.
    CLHNode* currTail = tail.load();
    while (currTail != nullptr) {
        CLHNode* prevTail = currTail->prev;
        delete currTail;
        currTail = prevTail;
    }
}

void CLHLock::lock() noexcept {
    // Swap the tail and myNode, making myNode the new tail.
    // The previous myNode becomes the tail's previous node.
    CLHNode* pred = tail.exchange(&myNode, std::memory_order_acquire);
    myNode.prev = pred;
    
    // Spin until all previous nodes(tail's prevNodes) are unlocked. (because they will be unlocked in order.)
    while (pred->locked) {
        CommonUtil::yieldThread();
    }
}

void CLHLock::unlock() noexcept {
    // After lock function, previous tail and myNode was swaped and the previous myNode became the tail's previous node.
    // So, current myNode's prevNode is myNode before lock function was called.
    myNode.locked = false;
    myNode = *myNode.prev;
}

thread_local CLHLock::CLHNode CLHLock::myNode = CLHLock::CLHNode{};