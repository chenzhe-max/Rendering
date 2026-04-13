#include <iostream>
#include <glm/glm.hpp>
#include "thread_pool.hpp"
#include "film.hpp"

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
    Film film {1920, 1080};
    thread_pool.parallelFor(200, 100, [&](size_t x, size_t y){
        film.setPixel(x, y, {0.5, 0.7, 0.2});
    });

    thread_pool.wait();
    auto now = std::chrono::high_resolution_clock::now();
    film.save("test.ppm");
    auto time = std::chrono::high_resolution_clock::now() - now;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    std::cout << ms.count() << std::endl;
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    thread_pool.addTask(new SimpleTask());
    

    return 0;
}


