//用原子操作来实现的自旋锁
#pragma once

#include <atomic>
#include <thread>

class SpinLock  //自旋锁  std::mutex是互斥锁
{
public:
    void acquire() 
    { 
        while (flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }
    void release() { flag.clear(std::memory_order_release); }
private:
    std::atomic_flag flag {};
};

class Guard  //构造函数的时候获得锁，析构函数的时候释放锁
{
public:
    Guard(SpinLock &spin_lock) : spin_lock(spin_lock) { spin_lock.acquire(); }
    ~Guard() { spin_lock.release(); }
private:
    SpinLock &spin_lock;
};