#ifndef LINKERNIGHAN_H
#define LINKERNIGHAN_H

#include "IImprovement.hpp"
#include "Solution/VectorSolution.hpp"

template <class TNeighborhood, class TSolution = VectorSolution>
class LinKernighan : public IImprovement<TSolution>
{
public:
    LinKernighan(const TNeighborhood& _neighborhood, const size_t _numberOfSteps);

    virtual TSolution improve(const TSolution& solution) override;

    virtual ~LinKernighan() {}

private:
    TNeighborhood neighborhood;

    const size_t numberOfSteps;
};

#endif // LINKERNIGHAN_H
