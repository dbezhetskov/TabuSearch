#ifndef LANDINGTABUSEARCHINL_HPP
#define LANDINGTABUSEARCHINL_HPP

#include "LandingTabuSearch.hpp"
#include "Move/IMove.hpp"
#include "Move/SimpleMove.hpp"
#include <utility>
#include <algorithm>
#include <cassert>

namespace
{

template<class TSolution>
TSolution random_walk
(
        const TSolution& solution,
        const size_t number_of_random_move,
        const size_t number_of_servers,
        const size_t number_of_disks
)
{
    TSolution result_solution(solution);

    for (size_t i = 0; i < number_of_random_move; ++i)
    {
        size_t destination_server = rand() % number_of_servers;
        size_t disk = rand() % number_of_disks;

        if (destination_server == result_solution.getServerForDisk(disk))
        {
            continue;
        }

        auto move = SimpleMove(destination_server, result_solution.getServerForDisk(disk), disk);
        if (result_solution.moveIsCorrect(move))
        {
            result_solution.applyMove(move);
        }
    }

    return result_solution;
}

}

template< class TSolution, class TTabuSearch >
LandingTabuSearch<TSolution, TTabuSearch>::LandingTabuSearch
(
        size_t                                  _number_of_landings,
        size_t                                  _depth,
        std::shared_ptr<Scheduler>              _scheduler,
        const TTabuSearch&                      _tabu_search,
        const TSolution&                        initialSolution,
        const std::shared_ptr<const TaskData>   _data
)
    : bestSolution(initialSolution)
    , scheduler(_scheduler)
    , number_of_landings(_number_of_landings)
    , depth(_depth)
    , tabu_search(_tabu_search)
    , data(_data)
{}

template< class TSolution, class TTabuSearch >
void LandingTabuSearch<TSolution, TTabuSearch>::run(const size_t number_of_steps)
{
    // 1. create random solutions with random_walk procedure
    std::srand( unsigned (time(0)) );
    std::vector< TSolution > random_solutions;
    for (size_t i = 0; i < number_of_landings; ++i)
    {
        random_solutions.emplace_back(random_walk(bestSolution, depth, data->numberOfServers, data->numberOfDisks));
    }

    // 2. ditribute random solution
    auto task = [&random_solutions, number_of_steps](TTabuSearch tabu_search, size_t idx) -> void
    {
        tabu_search.setStartSolution(random_solutions[idx]);
        tabu_search.run(number_of_steps);

        assert(idx >= 0 && idx < random_solutions.size());
        random_solutions[idx] = tabu_search.getBestSolution();
    };

    std::vector< std::future< void > > futures(number_of_landings - 1);
    for (size_t i = 0; i < futures.size(); ++i)
    {
        futures[i] = scheduler->schedule(task, tabu_search, i);
    }
    tabu_search.run(number_of_steps);
    random_solutions.back() = tabu_search.getBestSolution();

    // 3. gather result and choise best solution
    for (auto& future : futures) { future.get(); }
    auto best = std::min_element(random_solutions.begin(), random_solutions.end());
    bestSolution = *best;
}

template< class TSolution, class TTabuSearch >
TSolution LandingTabuSearch<TSolution, TTabuSearch>::getBestSolution()
{
    return bestSolution;
}

template< class TSolution, class TTabuSearch >
void LandingTabuSearch<TSolution, TTabuSearch>::setStartSolution(TSolution solution)
{
    bestSolution = solution;
}

#endif // LANDINGTABUSEARCHINL_HPP
