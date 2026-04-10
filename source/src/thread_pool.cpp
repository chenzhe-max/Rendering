#include "thread_pool.hpp"

void ThreadPool::WorkerThread(ThreadPool *master)
{
    while(master -> alive)
    {
        Task *task = master -> getTask();
        if(task != nullptr)
        {
            task-> run();
        }else{
            std::this_thread::yield(); //告诉系统我现在不急，可以让别的线程先跑
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count) //实现一下ThreadPool的构造函数和析构函数
{
    alive = true;
    if(thread_count == 0)
    {
        thread_count = std::thread::hardware_concurrency();
    }
    for(size_t i = 0; i < thread_count; i++)
    {
        threads.push_back(std::thread(ThreadPool::WorkerThread, this));
    }
}

ThreadPool::~ThreadPool()
{
    while(!tasks.empty())
    {
        std::this_thread::yield(); //把线程的控制权给操作系统，操作系统就会去选择下一个线程来执行
    }
    alive = false;
    for(auto &thread : threads)
    {
        thread.join();
    }
    threads.clear();
}
void ThreadPool::addTask(Task *task)
{
    std::lock_guard<std::mutex> guard(lock); //进入函数自动获取锁，推出函数自动把锁释放
    tasks.push_back(task);
}
Task *ThreadPool::getTask()
{
    std::lock_guard<std::mutex> guard(lock); //std::deque和std::list不是线程安全的
    if(tasks.empty())
    {
        return nullptr;
    }
    Task* task = tasks.front();
    tasks.pop_front();  //队首的任务拿出来然后删掉
    return task;
}