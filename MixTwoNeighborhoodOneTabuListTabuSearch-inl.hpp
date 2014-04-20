#ifndef MIXTWONEIGHBORHOODONETABULISTTABUSEARCHINL_HPP
#define MIXTWONEIGHBORHOODONETABULISTTABUSEARCHINL_HPP

#include "MixTwoNeighborhoodOneTabuListTabuSearch.hpp"

template<class Solution, class Neighborhood1, class Neighborhood2, class TabuList, class AspirationCriteria>
MixTwoNeighborhoodOneTabuListTabuSearch<Solution, Neighborhood1, Neighborhood2, TabuList, AspirationCriteria>::MixTwoNeighborhoodOneTabuListTabuSearch
(
        Solution            _initSolution,
        Neighborhood1       _neighborhood1,
        Neighborhood2       _neighborhood2,
        TabuList            _tabuList,
        AspirationCriteria  _aspirationCriteria
)
    : neighborhood1(_neighborhood1)
    , neighborhood2(_neighborhood2)
    , tabuList(_tabuList)
    , aspirationCriteria(_aspirationCriteria)
    , bestSolution(_initSolution)
{
}

template<class Solution, class Neighborhood1, class Neighborhood2, class TabuList, class AspirationCriteria>
void MixTwoNeighborhoodOneTabuListTabuSearch<Solution, Neighborhood1, Neighborhood2, TabuList, AspirationCriteria>::run(const size_t numberOfSteps)
{
    // initialize current solution
    Solution currentSolution(bestSolution);

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        auto moves1 = neighborhood1.getMoves(currentSolution);
        auto moves2 = neighborhood2.getMoves(currentSolution);
        decltype(moves1) moves;
        moves.reserve(moves1.size() + moves2.size());
        moves.insert(moves.end(), std::make_move_iterator(moves1.begin()), std::make_move_iterator(moves1.end()));
        moves.insert(moves.end(), std::make_move_iterator(moves2.begin()), std::make_move_iterator(moves2.end()));

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
            if (!tabuList.isTabu(*moves[i]) || aspirationCriteria.overrideTabu(currentSolution, *moves[i]))
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

        if (*localBestIterator < bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(*moves[indexs[idx]]);
        aspirationCriteria.update(currentSolution);
    }
}

template<class Solution, class Neighborhood1, class Neighborhood2, class TabuList, class AspirationCriteria>
Solution MixTwoNeighborhoodOneTabuListTabuSearch<Solution, Neighborhood1, Neighborhood2, TabuList, AspirationCriteria>::getBestSolution()
{
    return bestSolution;
}

#endif // MIXTWONEIGHBORHOODONETABULISTTABUSEARCHINL_HPP
