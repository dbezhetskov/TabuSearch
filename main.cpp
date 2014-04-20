#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"

// TABU SEARCHS
#include "TabuSearchStrategy/SimpleTabuSearch.hpp"
#include "TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp"

// ASPIRATION CRITERIA
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

// TABU LISTS
#include "TabuList/HashSetTabuList.hpp"

// NEIGHBORHOODS
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"
#include "Neighborhood/AssignmentProblem/AssignmentProblemRandomNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"

// Assignment Problem policy - to do with factory
#include "Neighborhood/AssignmentProblem/RandomGetterDisks.hpp"
#include "Neighborhood/AssignmentProblem/RandomGetterServers.hpp"
#include "Neighborhood/AssignmentProblem/RandomOneDiskByServerGetter.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "Move/SimpleMove.hpp"

int main()
{
    //========INIT========
    const size_t MAX_STEP = 1000;
    const size_t TENURE = 11;
	std::ios_base::sync_with_stdio(false);

    std::shared_ptr<TaskData> taskData(new TaskData());
    std::cin >> *taskData;

    VectorSolution solution(taskData);

    std::vector< std::unique_ptr<INeighborhood> > neighborhoods;
    neighborhoods.emplace_back(new MoveNeighborhood(taskData));
    neighborhoods.emplace_back(new SwapNeighborhood());
    UnionNeighborhood unionNeighborhood(std::move(neighborhoods));
    //AssignmentProblemRandomNeighborhood<RandomGetterServers, RandomOneDiskByServerGetter> assignmentProblemNeighborhood(*taskData, 2);

    HashSetTabuList tabuList(TENURE);
    BestEverAspirationCriteria aspirationCriteria(solution.getObjectiveValue());

    SimpleTabuSearch
            <
                VectorSolution,
                UnionNeighborhood,                
                HashSetTabuList,
                BestEverAspirationCriteria
            >
    tabuSearch(std::move(solution), std::move(unionNeighborhood), std::move(tabuList), std::move(aspirationCriteria));
    //========INIT========


    //========RUN========
    std::cout << "Init value : " << solution.getObjectiveValue() << std::endl;

    clock_t start = clock();
    tabuSearch.run(MAX_STEP);

    std::cout << "Time : " << ((double)(clock() - start) / CLOCKS_PER_SEC) << std::endl;

    VectorSolution bestSolution(tabuSearch.getBestSolution());
    std::cout << bestSolution.getObjectiveValue() << std::endl;

    auto distribution = bestSolution.getDistribution();
    for (auto server : distribution)
    {
        std::cout << server + 1 << ' ';
    }
    //std::copy(distribution.begin(), distribution.end(), std::ostream_iterator<size_t>(std::cout, ""));
    // question : how to put vector in cin with with change element
    //========RUN========

	return 0;
}
