#ifndef REDUCTION_HPP
#define REDUCTION_HPP

#include "Solution/ISolution.hpp"

template <class TSolution>
class IReduction
{
public:
    virtual TSolution reduce(const TSolution& solution) = 0;

    virtual ~IReduction() {}
};

#endif // REDUCTION_HPP

