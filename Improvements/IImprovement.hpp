#ifndef IMPROVEMENT_HPP
#define IMPROVEMENT_HPP

#include "Solution/VectorSolution.hpp"

template <class TSolution = VectorSolution>
class IImprovement
{
public:
    virtual TSolution improve(const TSolution& solution) = 0;

    virtual ~IImprovement() {}
};

#endif // IMPROVEMENT_HPP

