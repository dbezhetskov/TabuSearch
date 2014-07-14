#ifndef ISCHEDULER_HPP
#define ISCHEDULER_HPP

#include <future>
#include <type_traits>

class IScheduler
{
    template<class TFunction, class... Args>
    virtual auto schedule(TFunction&& function, Args&&... args) -> std::future< typename std::result_of<TFunction(Args...)>::type > = 0;

    virtual ~IScheduler() {}
};

#endif // ISCHEDULER_HPP
