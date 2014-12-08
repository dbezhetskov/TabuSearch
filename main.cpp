#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"

#include "Scheduler/Scheduler.hpp"

// TABU SEARCHS
#include "TabuSearchStrategy/SimpleTabuSearch.hpp"
#include "TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp"
#include "TabuSearchStrategy/ParallelTabuSearch.hpp"
#include "TabuSearchStrategy/LandingTabuSearch.hpp"

// ASPIRATION CRITERIA
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

// TABU LISTS
#include "TabuList/HashSetTabuList.hpp"

// NEIGHBORHOODS
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

static void run(ITabuSearch<VectorSolution>* const tabu_search, const size_t MAX_STEP);

#include "Move/SimpleMove.hpp"

int main()
{
    // measure: how different random solution in depend DEPTH

    //========INIT========
    const size_t MAX_STEP = 30;
    const size_t TENURE = 17;
	std::ios_base::sync_with_stdio(false);

    TaskData taskData;
    std::cin >> taskData;

    VectorSolution solution(taskData);

    typedef MoveNeighborhood FirstNeighborhoodType;
    typedef SwapNeighborhood SecondNeighborhoodType;
    typedef UnionNeighborhood<FirstNeighborhoodType, SecondNeighborhoodType> UnionNeighborhoodType;

    auto first_neighborhood = FirstNeighborhoodType(taskData);
    auto second_neighborhood = SecondNeighborhoodType();

    auto union_neighborhood = UnionNeighborhoodType(first_neighborhood, second_neighborhood);
    auto tabu_list = HashSetTabuList(TENURE);
    auto aspiration_criteria = BestEverAspirationCriteria(solution.getObjectiveValue());

    typedef SimpleTabuSearch < VectorSolution, UnionNeighborhoodType, HashSetTabuList, BestEverAspirationCriteria > SimpleTabuSearchMoveSwap;

    SimpleTabuSearchMoveSwap scout_tabu_search(solution, union_neighborhood, tabu_list, aspiration_criteria);

//    auto scheduler = std::make_shared<Scheduler>();
//    typedef ParallelTabuSearch < VectorSolution, UnionNeighborhoodType, HashSetTabuList, BestEverAspirationCriteria > ParallelTabuSearchMoveSwap;

//    ParallelTabuSearchMoveSwap tabu_search(scheduler, solution, 1000, union_neighborhood, tabu_list, aspiration_criteria);

    const size_t NUMBER_OF_LANDING = 3;
    const size_t DEPTH = 30;
    auto scheduler = std::make_shared<Scheduler>();
    LandingTabuSearch<VectorSolution, SimpleTabuSearchMoveSwap > tabu_search(
                NUMBER_OF_LANDING, DEPTH, scheduler, scout_tabu_search, solution, taskData
    );
    //========INIT========

    run(&tabu_search, MAX_STEP);

	return 0;
}

static void run(ITabuSearch<VectorSolution>* const tabu_search, const size_t MAX_STEP)
{
    std::cout << "Init value : " << tabu_search->getBestSolution().getObjectiveValue() << '\n';

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    tabu_search->run(MAX_STEP);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast< std::chrono::seconds >(end - start).count() << '\n' << '\n';
    std::cout << tabu_search->getBestSolution() << '\n';
}
