#include "Scheduler.hpp"
#include <utility>
#include <thread>
#include <iostream>

Scheduler::Scheduler(unsigned number_of_threads = 0)
    : stop(false)
{
    if (0 == number_of_threads)
    {
        unsigned hardware_concurrency = std::thread::hardware_concurrency();
        number_of_threads = hardware_concurrency <= 1 ? 1 : hardware_concurrency - 1;
    }

    for (unsigned i = 0; i < number_of_threads; ++i)
    {
        threads.emplace_back(
            [this]
            {
                while (true)
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
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

template<class TFunction, class... Args>
auto Scheduler::schedule(TFunction&& function, Args&&... args) -> std::future< typename std::result_of<TFunction(Args...)>::type >
{
    std::lock_guard<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if (stop)
    {
        throw std::runtime_error("schedule on destroyed scheduler");
    }

    typedef typename std::result_of<TFunction(Args...)>::type result_type;

    std::packaged_task<result_type()> task(std::bind(std::forward<TFunction>(function), std::forward<Args>(args)...));

    std::future<result_type> result(task.get_future());
    tasks.push(std::move(task));
    condition.notify_one();

    return result;
}

Scheduler::~Scheduler()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (unsigned i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
}
