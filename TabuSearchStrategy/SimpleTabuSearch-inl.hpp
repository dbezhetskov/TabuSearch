#include "SimpleTabuSearch.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
SimpleTabuSearch<TSolution, TNeighborhood, TTabuList, TAspirationCriteria>::SimpleTabuSearch
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

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
void SimpleTabuSearch<TSolution, TNeighborhood, TTabuList, TAspirationCriteria>::run(const size_t number_of_steps)
{
    // initialize current solution
    TSolution currentSolution(bestSolution);

    for (size_t i = 0; i < number_of_steps; ++i)
	{
        auto moves = neighborhood.getMoves(currentSolution);
        if (moves.empty())
        {
            std::cout << "Moves exhaustion, return in bestSolution" << '\n';
            currentSolution = bestSolution;
            continue;
        }

        std::vector<double> objectiveValues;
        std::vector<size_t> indexs;

        // generate all (non tabu and !aspiration) solutions from the current
        for (size_t i = 0; i < moves.size(); ++i)
        {
            if (!tabuList.isTabu(*moves[i]) || aspirationCriteria.overrideTabu(currentSolution, *moves[i]))
            {
                objectiveValues.push_back(currentSolution.tryOnMove(*moves[i]));
                indexs.push_back(i);
            }
        }

        if (objectiveValues.empty())
        {
            std::cout << "All solution tabu!" << '\n';
            continue;
        }

        // choice of the best solution
        auto localBestIterator = std::min_element(objectiveValues.begin(), objectiveValues.end());

        // apply move
        size_t idx = std::distance(objectiveValues.begin(), localBestIterator);
        currentSolution.applyMove(*moves[indexs[idx]]);

        std::cout << *localBestIterator << '\n';

        if (*localBestIterator <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            break;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(*moves[indexs[idx]]);
        aspirationCriteria.update(currentSolution);
	}
}

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
TSolution SimpleTabuSearch<TSolution, TNeighborhood, TTabuList, TAspirationCriteria>::getBestSolution()
{
    return bestSolution;
}

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
void SimpleTabuSearch<TSolution, TNeighborhood, TTabuList, TAspirationCriteria>::setStartSolution(TSolution solution)
{
    bestSolution = solution;
}
