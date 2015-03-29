#include "Scheduler.hpp"
#include <utility>
#include <thread>
#include <iostream>

Scheduler::Scheduler(unsigned numberOfThreads)
    : stop(false)
{
    if (0 == numberOfThreads)
    {
        numberOfThreads = std::thread::hardware_concurrency();
    }

    for (unsigned i = 0; i < numberOfThreads; ++i)
    {
        threads.emplace_back(
            [this]
            {
                while (true)
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    while(!stop && tasks.empty())
                    {
                        condition.wait(lock);
                    }

                    if (stop && tasks.empty())
                    {
                        return;
                    }

                    FunctionWrapper task = std::move(tasks.front());
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            }
        );
    }
}

Scheduler::~Scheduler()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (unsigned i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
}
