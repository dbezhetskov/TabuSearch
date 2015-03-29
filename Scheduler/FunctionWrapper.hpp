#ifndef FUNCTIONWRAPPER_HPP
#define FUNCTIONWRAPPER_HPP

#include <memory>
#include <utility>

class FunctionWrapper
{
    struct implBase
    {
        virtual void call() = 0;
    };

    std::unique_ptr<implBase> impl;

    template<class F>
    struct implType : implBase
    {
        void call() { func(); }

        implType(F&& f_)
            : func(std::move(f_))
        {}

        F func;
    };

public:
    template <class F>
    FunctionWrapper(F&& f)
        : impl(new implType<F>(std::move(f)))
    {}

    void operator() () { impl->call(); }

    FunctionWrapper() = default;

    FunctionWrapper(FunctionWrapper&& other)
        : impl(std::move(other.impl))
    {}

    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl = std::move(other.impl);
        return *this;
    }

    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

#endif // FUNCTIONWRAPPER_HPP
