
class Lock {
public:
    virtual void lock() noexcept = 0;
    virtual bool try_lock() noexcept = 0;
    virtual void unlock() noexcept = 0;
};
