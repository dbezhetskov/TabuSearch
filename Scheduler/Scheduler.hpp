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
    Scheduler(unsigned number_of_threads);

    ~Scheduler();

    template<class TFunction, class... Args>
    auto schedule(TFunction&& function, Args&&... args) -> std::future< typename std::result_of<TFunction(Args...)>::type >;

private:
    std::condition_variable condition;

    std::mutex queue_mutex;

    bool stop;

    std::vector<std::thread> threads;

    std::queue<FunctionWrapper> tasks;
};

#include "Scheduler-inl.hpp"

#endif // SCHEDULER_HPP
