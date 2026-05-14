#include "thread/thread_pool.hpp"
ThreadPool thread_pool{}; //全局变量
void ThreadPool::WorkerThread(ThreadPool *master)
{
    while(master -> alive == 1)
    {
         if (master->tasks.empty()) //防止工作线程和BVH线程抢cpu的资源
         {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }
        Task *task = master -> getTask();
        if(task != nullptr)
        {
            task-> run();
            delete task;
            master->pending_task_count--;
        }else{
            std::this_thread::yield(); //告诉系统现在不急，可以让别的线程先跑
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count) //实现一下ThreadPool的构造函数和析构函数
{
    alive = 1;
    pending_task_count = 0;
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
    /*while(!tasks.empty())
    {
        std::this_thread::yield(); //把线程的控制权给操作系统，操作系统就会去选择下一个线程来执行
    }*/
    wait();
    alive = 0;
    for(auto &thread : threads)
    {
        thread.join();
    }
    threads.clear();
}

class ParallelForTask : public Task{
public:
      ParallelForTask(size_t x, size_t y, size_t chunk_width, size_t chunk_height, const std::function<void(size_t, size_t)> &lambda)
        : x(x), y(y), chunk_width(chunk_width), chunk_height(chunk_height), lambda(lambda) {}

    void run() override{
          for (size_t idx_x = 0; idx_x < chunk_width; idx_x ++) {
            for (size_t idx_y = 0; idx_y < chunk_height; idx_y ++) {
                lambda(x + idx_x, y + idx_y);
            }
        }
    }
private:
    size_t x, y, chunk_width, chunk_height;
    std::function<void(size_t, size_t)> lambda;

   
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)> &lambda, bool complex) //判断是不是复杂函数，复杂函数就每个线程16个任务，不复杂就每个线程1个任务
{
    Guard guard(spin_lock);
   float chunk_width_float = static_cast<float>(width) / sqrt(threads.size());
    float chunk_height_float = static_cast<float>(height) / sqrt(threads.size());
    if (complex) {
        chunk_width_float /= sqrt(16);
        chunk_height_float /= sqrt(16);
    }
    size_t chunk_width = std::ceil(chunk_width_float);
    size_t chunk_height = std::ceil(chunk_height_float);

    for (size_t x = 0; x < width; x += chunk_width) {
        for (size_t y = 0; y < height; y += chunk_height) {
            pending_task_count++;
             if (x + chunk_width > width) {
                chunk_width = width - x;
            }
            if (y + chunk_height > height) {
                chunk_height = height - y;
            }
            tasks.push(new ParallelForTask(x, y, chunk_width, chunk_height, lambda));
        }
    }
}

void ThreadPool::wait() const
{
    while(pending_task_count > 0)
    {
        std::this_thread::yield();
    }
}

void ThreadPool::addTask(Task *task)
{
    Guard guard(spin_lock); //用自旋锁了不用mutex了
    //std::lock_guard<std::mutex> guard(lock); 进入函数自动获取锁，推出函数自动把锁释放
    pending_task_count++;
    tasks.push(task);
}
Task *ThreadPool::getTask()
{
    Guard guard(spin_lock);
    //std::lock_guard<std::mutex> guard(lock); std::deque和std::list不是线程安全的
    if(tasks.empty())
    {
        return nullptr;
    }
    Task* task = tasks.front();
    tasks.pop();  //队首的任务拿出来然后删掉
    return task;
}