#include "SimpleAssignmentProblemTabuSearch.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

template<class Solution, class Neighborhood, class AssignmentProblemNeighborhood, class TabuList, class AspirationCriteria>
SimpleAssignmentProblemTabuSearch<Solution, Neighborhood, AssignmentProblemNeighborhood, TabuList, AspirationCriteria>::SimpleAssignmentProblemTabuSearch
(
        Solution&&                        _initSolution,
        Neighborhood&&                    _neighborhood,
        AssignmentProblemNeighborhood&&   _assignmentProblemNeighborhood,
        TabuList&&                        _tabuList,
        AspirationCriteria&&              _aspirationCriteria
)
    : neighborhood(std::move(_neighborhood))
    , assignmentProblemNeighborhood(std::move(_assignmentProblemNeighborhood))
    , tabuList(std::move(_tabuList))
    , aspirationCriteria(std::move(_aspirationCriteria))
    , bestSolution(std::move(_initSolution))
    , badSolutionCount(0)
{
}

template<class Solution, class Neighborhood, class AssignmentProblemNeighborhood, class TabuList, class AspirationCriteria>
void SimpleAssignmentProblemTabuSearch<Solution, Neighborhood, AssignmentProblemNeighborhood, TabuList, AspirationCriteria>::run(const size_t numberOfSteps)
{
    // initialize current solution
    Solution currentSolution(bestSolution);

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        auto moves = neighborhood.getMoves(currentSolution);

        if (moves.empty() || badSolutionCount == 20)
        {
            std::cout << "Moves exhaustion, return in bestSolution" << std::endl;
            auto moves = assignmentProblemNeighborhood.getMoves(currentSolution);
            for (int i = 0; i < 1000; ++i)
            {
                if (!moves.empty())
                {
                    break;
                }
                moves = assignmentProblemNeighborhood.getMoves(currentSolution);
            }
            std::cout << "BEFORE assignmentProblem : " << bestSolution.getObjectiveValue() << std::endl;
            if (!moves.empty())
            {
                bestSolution.applyMove(*moves[0]);
            }
            std::cout << "AFTER assignmentProblem : " << bestSolution.getObjectiveValue() << std::endl;
            currentSolution = bestSolution;
            badSolutionCount = 0;
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

        if (*localBestIterator <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
            badSolutionCount = 0;
        }
        else
        {
            ++badSolutionCount;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(*moves[indexs[idx]]);
        aspirationCriteria.update(currentSolution);
    }
}

template<class Solution, class Neighborhood, class AssignmentProblemNeighborhood, class TabuList, class AspirationCriteria>
Solution SimpleAssignmentProblemTabuSearch<Solution, Neighborhood, AssignmentProblemNeighborhood, TabuList, AspirationCriteria>::getBestSolution()
{
    return bestSolution;
}
