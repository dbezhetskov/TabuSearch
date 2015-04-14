#ifndef SIMPLE_TABU_SEARCH_REVERSE_HPP
#define SIMPLE_TABU_SEARCH_REVERSE_HPP

#include "SimpleTabuSearch.hpp"
#include "Solution/VectorSolution.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"
#include "Move/IMove.hpp"

template <class TNeighborhood, class TImprovement, class TTabuList = HashSetTabuList,
          class TAspirationCriteria = BestEverAspirationCriteria, class TSolution = VectorSolution>
class SimpleTabuSearchReverse : public SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>
{
public:
    SimpleTabuSearchReverse
    (
            const TSolution&                        _initSolution,
            const TNeighborhood&                    _neighborhood,
            const TTabuList&                        _tabuList,
            const TAspirationCriteria&              _aspirationCriteria,
            const TImprovement&                     _improver,
            const size_t                            _numberOfUnchanged
    );

    virtual void run(const size_t numberOfSteps) override;

private:
    using SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::bestSolution;
    using SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::tabuList;
    using SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::aspirationCriteria;
    using SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestMove;

private:
    TImprovement improver;

    const size_t numberOfUnchanged;
};

#endif // SIMPLE_TABU_SEARCH_REVERSE_HPP
