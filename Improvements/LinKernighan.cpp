#include "LinKernighan.hpp"
#include "TabuSearchStrategy/SimpleTabuSearch.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

template <class TNeighborhood, class TSolution>
LinKernighan<TNeighborhood, TSolution>::LinKernighan(const TNeighborhood& _neighborhood, const size_t _numberOfSteps)
    : neighborhood(_neighborhood)
    , numberOfSteps(_numberOfSteps)
{}

template <class TNeighborhood, class TSolution>
TSolution LinKernighan<TNeighborhood, TSolution>::improve(const TSolution& solution)
{
    auto tabuList = HashSetTabuList(numberOfSteps);
    auto aspirationCriteria = BestEverAspirationCriteria(solution.getObjectiveValue());

    SimpleTabuSearch<TNeighborhood> tabuSearch(solution, neighborhood, tabuList, aspirationCriteria);
    tabuSearch.run(numberOfSteps);

    auto receivedSolution = tabuSearch.getBestSolution();

    if (receivedSolution.getObjectiveValue() < solution.getObjectiveValue())
    {
        return receivedSolution;
    }

    return solution;
}

