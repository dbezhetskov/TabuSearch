#ifndef _SIMPLE_TABU_SEARCH_H_
#define _SIMPLE_TABU_SEARCH_H_

#include "ITabuSearch.hpp"

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
class SimpleTabuSearch : public ITabuSearch<TSolution>
{
public:
    SimpleTabuSearch
    (
            const TSolution&                        _initSolution,
            const TNeighborhood&                    _neighborhood,
            const TTabuList&                        _tabuList,
            const TAspirationCriteria&              _aspirationCriteria
    );

    virtual void run(const size_t number_of_steps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(TSolution solution) override;

private:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;
};

#include "SimpleTabuSearch-inl.hpp"

#endif // !_SIMPLE_TABU_SEARCH_H_
