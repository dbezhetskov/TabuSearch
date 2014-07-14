#ifndef FUNCTIONWRAPPER_HPP
#define FUNCTIONWRAPPER_HPP

#include <memory>
#include <utility>

class FunctionWrapper
{
    struct impl_base
    {
        virtual void call() = 0;
    };

    std::unique_ptr<impl_base> impl;

    template<class F>
    struct impl_type : impl_base
    {
        void call() { f(); }

        impl_type(F&& f_) : f(std::move(f_)) {}

        F f;
    };

public:
    template<class F>
    FunctionWrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}

    void operator()() { impl->call(); }

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
