#ifndef _SIMPLE_TABU_SEARCH_H_
#define _SIMPLE_TABU_SEARCH_H_

#include "ITabuSearch.hpp"
#include "Solution/VectorSolution.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

template <class TNeighborhood, class TTabuList = HashSetTabuList, class TAspirationCriteria = BestEverAspirationCriteria, class TSolution = VectorSolution>
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

    virtual void run(const size_t numberOfSteps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(const TSolution& solution) override;

protected:
    std::unique_ptr<IMove> getBestMove(const TSolution &currentSolution);

protected:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;
};

#endif // !_SIMPLE_TABU_SEARCH_H_
