#include "TaskData.hpp"
#include "VectorSolution.hpp"

// TABU SEARCHS
#include "SimpleTabuSearch.hpp"
#include "MixTwoNeighborhoodOneTabuListTabuSearch.hpp"
#include "SimpleAssignmentProblemTabuSearch.hpp"

// ASPIRATION CRITERIA
#include "BestEverAspirationCriteria.hpp"

// TABU LISTS
#include "HashSetTabuList.hpp"

// NEIGHBORHOODS
#include "MoveNeighborhood.hpp"
#include "RandomMoveNeighborhood.hpp"
#include "SwapNeighborhood.hpp"
#include "RandomSwapNeighborhood.hpp"
#include "AssignmentProblemRandomNeighborhood.hpp"
#include "UnionNeighborhood.hpp"

// Assignment Problem policy - to do with factory
#include "RandomGetterDisks.hpp"
#include "RandomGetterServers.hpp"
#include "RandomOneDiskByServerGetter.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "SimpleMove.hpp"

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
