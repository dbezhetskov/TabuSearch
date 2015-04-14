#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"

#include "Scheduler/Scheduler.hpp"

// TABU SEARCHS
#include "TabuSearchStrategy/SimpleTabuSearch.hpp"
#include "TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp"
#include "TabuSearchStrategy/ParallelTabuSearch.hpp"
#include "TabuSearchStrategy/LandingTabuSearch.hpp"
#include "TabuSearchStrategy/SimpleTabuSearchReverse.hpp"

// ASPIRATION CRITERIA
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

// TABU LISTS
#include "TabuList/HashSetTabuList.hpp"

// REDUCER
#include "Reduction/RandomReduction.hpp"

// NEIGHBORHOODS
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"

// IMPROVEMENTS
#include "Improvements/LinKernighan.hpp"

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

namespace
{
void run(ITabuSearch<VectorSolution>* const tabu_search, const size_t MAX_STEP);
}

#include "Move/SimpleMove.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);

    //========INIT========
    const size_t MAX_STEP = 500;
    const size_t TENURE = 17;

    TaskData taskData;
    std::cin >> taskData;

    VectorSolution solution(taskData, 0);

    RandomReduction<VectorSolution> reductor(taskData);
    solution = reductor.reduce(solution);

    // typedef UnionNeighborhood<MoveNeighborhood, SwapNeighborhood> NeighborhoodType;

    MoveNeighborhood moveNeighborhood(taskData);
    // SwapNeighborhood swapNeighborhood;
    // auto neighborhood = NeighborhoodType(moveNeighborhood, swapNeighborhood);
    auto tabuList = HashSetTabuList(TENURE);
    auto aspirationCriteria = BestEverAspirationCriteria(solution.getObjectiveValue());

    auto improverNeighborhood = RandomMoveNeighborhood(taskData, 1000U);
    auto improver = LinKernighan<RandomMoveNeighborhood>(improverNeighborhood, 50);

    SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<RandomMoveNeighborhood>>
            tabu_search(solution, moveNeighborhood, tabuList, aspirationCriteria, improver, 10U);

//    auto scheduler = std::make_shared<Scheduler>();
//    LandingTabuSearch<SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<RandomMoveNeighborhood>>> tabuSearch
//            (10, 30, scheduler, scout_tabu_search, solution, taskData);
    //========INIT========

    run(&tabu_search, MAX_STEP);

	return 0;
}

namespace
{

void run(ITabuSearch<VectorSolution>* const tabu_search, const size_t MAX_STEP)
{
    std::cout << "Init value : " << tabu_search->getBestSolution().getObjectiveValue() << '\n';

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    tabu_search->run(MAX_STEP);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "\n Time: " << std::chrono::duration_cast< std::chrono::seconds >(end - start).count() << '\n' << '\n';
    std::cout << tabu_search->getBestSolution() << '\n';
}

}


// measure: how different random solution in depend DEPTH

// typedef UnionNeighborhood<FirstNeighborhoodType, SecondNeighborhoodType> UnionNeighborhoodType;
// auto union_neighborhood = UnionNeighborhoodType(first_neighborhood, second_neighborhood);

//    auto scheduler = std::make_shared<Scheduler>();
//    typedef ParallelTabuSearch < VectorSolution, FirstNeighborhoodType, HashSetTabuList, BestEverAspirationCriteria > ParallelTabuSearchMoveSwap;

//    ParallelTabuSearchMoveSwap tabu_search(scheduler, solution, 1000, first_neighborhood, tabu_list, aspiration_criteria);
