#pragma once
#include <vector>
#include <thread>
#include <queue>
#include "spin_lock.hpp"
#include <functional>
class Task //Task就是每个线程会去执行的任务
{
public:
    virtual void run() = 0; //之后所有的任务都会去实现这个run函数
    virtual ~Task() = default;
};

class ThreadPool
{
public:
    static void WorkerThread(ThreadPool *master);
    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();
    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)> &lambda, bool complex = true);//并行for循环
    void wait() const; //让主线程等待所有任务执行完成
    void addTask(Task *task); //添加一个任务
    Task *getTask();          //获取一个任务

private:
    std::vector<std::thread> threads; // 用vector来存放所有的线程
    std::queue<Task *> tasks; //一开始用list来存储所有的任务（list添加和删除都是O(1))，后来换成了queue
    //std::mutex lock; 多个线程与主线程在获取和添加任务的时候会有资源的竞争，用锁来避免这种情况
    std::atomic<int> pending_task_count;//每新增一个任务，这个变量就加1，代表多了一个未完成的任务,执行完run函数就减1，代表完成了一个任务
    SpinLock spin_lock{};

    std::atomic<int> alive; //表示这个线程池是否还存在，防止线程死循环，构造的时候是true，析构的时候是false
};

extern ThreadPool thread_pool; //声明就行，别定义，全局变量是一次定义+多处声明


