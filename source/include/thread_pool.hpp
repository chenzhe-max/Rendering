#pragma once
#include <vector>
#include <thread>
#include <list>
#include <mutex>
class Task //Task就是每个线程会去执行的任务
{
public:
    virtual void run() = 0; //之后所有的任务都会去实现这个run函数
};

class ThreadPool
{
public:
    static void WorkerThread(ThreadPool *master);
    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();
    void addTask(Task *task); //添加一个任务
    Task *getTask();          //获取一个任务

private:
    std::vector<std::thread> threads; // 用vector来存放所有的线程
    std::list<Task *> tasks; //用list来存储所有的任务（list添加和删除都是O(1))
    std::mutex lock; //多个线程与主线程在获取和添加任务的时候会有资源的竞争，用锁来避免这种情况
    bool alive; //表示这个线程池是否还存在，防止线程死循环，构造的时候是true，析构的时候是false
};