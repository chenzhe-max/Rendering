#include <iostream>
#include <glm/glm.hpp>
#include "thread_pool.hpp"

class SimpleTask : public Task
{
public:
    void run() override
    {
        std::cout << "hello, rendering" <<std::endl;
    }
};
int main()
{
    ThreadPool thread_pool{};
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());

    return 0;
}


