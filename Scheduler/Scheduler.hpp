#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "FunctionWrapper.hpp"
#include <future>
#include <type_traits>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <thread>

class Scheduler
{
public:
    Scheduler(unsigned numberOfThreads = 0);

    virtual ~Scheduler();

    template<class TFunction, class... Args>
    auto schedule(TFunction&& function, Args&&... args) -> std::future<typename std::result_of<TFunction(Args...)>::type>;

private:
    std::condition_variable condition;

    std::mutex queueMutex;

    bool stop;

    std::vector<std::thread> threads;

    std::queue<FunctionWrapper> tasks;
};

template<class TFunction, class... Args>
auto Scheduler::schedule(TFunction&& function, Args&&... args) -> std::future< typename std::result_of<TFunction(Args...)>::type >
{
    std::lock_guard<std::mutex> lock(queueMutex);

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

#endif // SCHEDULER_HPP
