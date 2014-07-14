#include "SimpleTabuSearch.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template<class Solution>
SimpleTabuSearch<Solution>::SimpleTabuSearch
(
        Solution&&                              _initSolution,
        std::unique_ptr<INeighborhood>&&        _neighborhood,
        std::unique_ptr<ITabuList>&&            _tabuList,
        std::unique_ptr<IAspirationCriteria>&&  _aspirationCriteria
)
    : neighborhood(std::move(_neighborhood))
    , tabuList(std::move(_tabuList))
    , aspirationCriteria(std::move(_aspirationCriteria))
    , bestSolution(std::move(_initSolution))
{
}

template<class Solution>
void SimpleTabuSearch<Solution>::run(const size_t numberOfSteps)
{
    // initialize current solution
    Solution currentSolution(bestSolution);

	for (size_t i = 0; i < numberOfSteps; ++i)
	{
        auto moves = neighborhood->getMoves(currentSolution);
        if (moves.empty())
        {
            std::cout << "Moves exhaustion, return in bestSolution" << std::endl;
            currentSolution = bestSolution;
            continue;
        }

        std::vector<double> objectiveValues;
        std::vector<size_t> indexs;

        // generate all (non tabu and !aspiration) solutions from the current
        for (size_t i = 0; i < moves.size(); ++i)
        {
            if (!tabuList->isTabu(*moves[i]) || aspirationCriteria->overrideTabu(currentSolution, *moves[i]))
            {
                objectiveValues.push_back(currentSolution.tryOnMove(*moves[i]));
                indexs.push_back(i);
            }
        }

        if (objectiveValues.empty())
        {
            std::cout << "All solution tabu!" << std::endl;
            continue;
        }

        // choice of the best solution
        auto localBestIterator = std::min_element(objectiveValues.begin(), objectiveValues.end());

        // apply move
        size_t idx = std::distance(objectiveValues.begin(), localBestIterator);
        currentSolution.applyMove(*moves[indexs[idx]]);

        std::cout << *localBestIterator << std::endl;

        if (*localBestIterator <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            break;
        }

        // update tabuList and AspirationCriteria
        tabuList->update(*moves[indexs[idx]]);
        aspirationCriteria->update(currentSolution);
	}
}

template<class Solution>
Solution SimpleTabuSearch<Solution>::getBestSolution()
{
    return bestSolution;
}
