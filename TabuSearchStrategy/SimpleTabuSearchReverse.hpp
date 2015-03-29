#ifndef SIMPLE_TABU_SEARCH_REVERSE_HPP
#define SIMPLE_TABU_SEARCH_REVERSE_HPP

#include "ITabuSearch.hpp"
#include "Solution/VectorSolution.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"
#include "Move/IMove.hpp"

template <class TNeighborhood, class TTabuList = HashSetTabuList, class TAspirationCriteria = BestEverAspirationCriteria, class TSolution = VectorSolution>
class SimpleTabuSearchReverse : public ITabuSearch<TSolution>
{
public:
    SimpleTabuSearchReverse
    (
            const TSolution&                        _initSolution,
            const TNeighborhood&                    _neighborhood,
            const TTabuList&                        _tabuList,
            const TAspirationCriteria&              _aspirationCriteria,
            const size_t                            _numberOfUnchanged
    );

    virtual void run(const size_t numberOfSteps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(const TSolution& solution) override;

private:
    std::unique_ptr<IMove> getBestMove(const TSolution& currentSolution);

private:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;

    const size_t numberOfUnchanged;
};

#endif // SIMPLE_TABU_SEARCH_REVERSE_HPP
