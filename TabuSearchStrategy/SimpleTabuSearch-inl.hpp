#include "SimpleTabuSearch.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template < class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution >
SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::SimpleTabuSearch
(
        const TSolution&                        _initSolution,
        const TNeighborhood&                    _neighborhood,
        const TTabuList&                        _tabuList,
        const TAspirationCriteria&              _aspirationCriteria
)
    : neighborhood(_neighborhood)
    , tabuList(_tabuList)
    , aspirationCriteria(_aspirationCriteria)
    , bestSolution(_initSolution)
{
}

template < class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution >
void SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::run(const size_t numbersOfSteps)
{
    // initialize current solution
    TSolution currentSolution(bestSolution);

    for (size_t i = 0; i < numbersOfSteps; ++i)
	{
        auto moves = neighborhood.getMoves(currentSolution);

        if (moves.empty())
        {
            std::cout << "Moves exhaustion, return in bestSolution" << '\n';
            currentSolution = bestSolution;
            continue;
        }

        std::vector<ObjectiveValueWithIndex> objectiveValuesWithIndex;
        // generate all (non tabu and !aspiration) solutions from the current
        for (size_t i = 0; i < moves.size(); ++i)
        {
            if (!tabuList.isTabu(*moves[i]) || aspirationCriteria.overrideTabu(currentSolution, *moves[i]))
            {
                objectiveValuesWithIndex.emplace_back(currentSolution.tryOnMove(*moves[i]), i);
            }
        }

        if (objectiveValuesWithIndex.empty())
        {
            std::cout << "All solution tabu!" << '\n';
            continue;
        }

        // choice of the best solution
        auto localBestObjectiveValue = std::min_element(objectiveValuesWithIndex.begin(), objectiveValuesWithIndex.end());
        const auto& localBestMove = *moves[localBestObjectiveValue->index];

        // apply move
        currentSolution.applyMove(localBestMove);

        std::cout << localBestObjectiveValue->objectiveValue << '\n';

        if (localBestObjectiveValue->objectiveValue <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            break;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(localBestMove);
        aspirationCriteria.update(currentSolution);
	}
}

template < class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution >
TSolution SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestSolution()
{
    return bestSolution;
}

template < class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution >
void SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::setStartSolution(TSolution solution)
{
    bestSolution = solution;
}
