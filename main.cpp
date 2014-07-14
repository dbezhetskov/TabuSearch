#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"

#include "Scheduler/Scheduler.hpp"

// TABU SEARCHS
#include "TabuSearchStrategy/SimpleTabuSearch.hpp"
#include "TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp"
#include "TabuSearchStrategy/ParallelTabuSearch.hpp"

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
#include "Neighborhood/ProportionSwap.hpp"

// Assignment Problem policy - to do with factory
#include "Neighborhood/AssignmentProblem/RandomGetterDisks.hpp"
#include "Neighborhood/AssignmentProblem/RandomGetterServers.hpp"
#include "Neighborhood/AssignmentProblem/RandomOneDiskByServerGetter.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <thread>

#include "Move/SimpleMove.hpp"

int main()
{
    //========INIT========
    const size_t MAX_STEP = 1;
    const size_t TENURE = 10;
	std::ios_base::sync_with_stdio(false);

    std::shared_ptr<TaskData> taskData(new TaskData());
    std::cin >> *taskData;

    VectorSolution solution(taskData);

    std::vector< std::unique_ptr<INeighborhood> > neighborhoods;
    neighborhoods.emplace_back(new MoveNeighborhood(taskData));
    neighborhoods.emplace_back(new SwapNeighborhood());
    neighborhoods.emplace_back(new ProportionSwap(*taskData));
    auto unionNeighborhood_ptr = std::unique_ptr<UnionNeighborhood>(new UnionNeighborhood(std::move(neighborhoods)));
    auto tabuList_ptr = std::unique_ptr<HashSetTabuList>(new HashSetTabuList(TENURE));
    auto aspirationCriteria_ptr = std::unique_ptr<BestEverAspirationCriteria>(new BestEverAspirationCriteria(solution.getObjectiveValue()));

    SimpleTabuSearch<VectorSolution>
    tabuSearch(std::move(solution), std::move(unionNeighborhood_ptr), std::move(tabuList_ptr), std::move(aspirationCriteria_ptr));

//    auto scheduler = std::make_shared<Scheduler>(1);
//    ParallelTabuSearch<VectorSolution> tabuSearch(scheduler, solution, 106000,
//                                                  std::move(unionNeighborhood_ptr), std::move(tabuList_ptr), std::move(aspirationCriteria_ptr));

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
    // --------------------------------------------------------
    //========RUN========

	return 0;
}
