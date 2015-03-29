#include "LandingTabuSearch.hpp"
#include "Move/IMove.hpp"
#include "Move/SimpleMove.hpp"
#include <utility>
#include <algorithm>
#include <cassert>

namespace
{

template<class TSolution = VectorSolution>
TSolution randomWalk
(
        const TSolution& solution,
        const size_t numberOfRandomMove,
        const size_t numberOfServers,
        const size_t numberOfDisks
)
{
    TSolution result_solution(solution);

    for (size_t i = 0; i < numberOfRandomMove; ++i)
    {
        size_t destination_server = rand() % numberOfServers;
        size_t disk = rand() % numberOfDisks;

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

template <class TTabuSearch, class TSolution>
LandingTabuSearch<TTabuSearch, TSolution>::LandingTabuSearch
(
        const size_t                            _number_of_landings,
        const size_t                            _depth,
        const std::shared_ptr<Scheduler>        _scheduler,
        const TTabuSearch&                      _tabu_search,
        const TSolution&                        initialSolution,
        const TaskData&                         _data
)
    : bestSolution(initialSolution)
    , scheduler(_scheduler)
    , numberOfLandings(_number_of_landings)
    , depth(_depth)
    , tabuSearch(_tabu_search)
    , data(_data)
{}

template <class TTabuSearch, class TSolution>
void LandingTabuSearch<TTabuSearch, TSolution>::run(const size_t numberOfSteps)
{
    // 1. create random solutions via randomWalk procedure
    std::srand(unsigned (time(0)));
    std::vector<TSolution> randomSolutions;
    for (size_t i = 0; i < numberOfLandings; ++i)
    {
        randomSolutions.emplace_back(randomWalk(bestSolution, depth, data.getNumberOfServers(), data.getNumberOfDisks()));
    }

    // 2. ditribute random solution
    auto task = [&randomSolutions, numberOfSteps](TTabuSearch tabuSearch, size_t idx) -> void
    {
        tabuSearch.setStartSolution(randomSolutions[idx]);
        tabuSearch.run(numberOfSteps);

        assert(idx >= 0 && idx < randomSolutions.size());
        randomSolutions[idx] = tabuSearch.getBestSolution();
    };

    std::vector<std::future<void>> futures(numberOfLandings - 1);
    for (size_t i = 0; i < futures.size(); ++i)
    {
        futures[i] = scheduler->schedule(task, tabuSearch, i);
    }
    tabuSearch.run(numberOfSteps);
    randomSolutions.back() = tabuSearch.getBestSolution();

    // 3. gather results and choise best solution
    for (auto& future : futures) { future.get(); }
    auto best = std::min_element(randomSolutions.begin(), randomSolutions.end());
    bestSolution = *best;
}

template <class TTabuSearch, class TSolution>
TSolution LandingTabuSearch<TTabuSearch, TSolution>::getBestSolution()
{
    return bestSolution;
}

template <class TTabuSearch, class TSolution>
void LandingTabuSearch<TTabuSearch, TSolution>::setStartSolution(const TSolution& solution)
{
    bestSolution = solution;
}
