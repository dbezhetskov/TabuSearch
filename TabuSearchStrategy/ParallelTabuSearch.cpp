#include "ParallelTabuSearch.hpp"
#include <utility>
#include <algorithm>
#include <limits>
#include <functional>
#include <iterator>
#include <cassert>
#include <iostream>

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
ParallelTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::ParallelTabuSearch
(
        std::shared_ptr<Scheduler>              _scheduler,
        TSolution                               &initialSolution,
        const size_t                            _block_size,
        TNeighborhood                           &_neighborhood,
        TTabuList                               &_tabuList,
        TAspirationCriteria                     &_aspirationCriteria
)
    : neighborhood(_neighborhood)
    , tabuList(_tabuList)
    , aspirationCriteria(_aspirationCriteria)
    , bestSolution(initialSolution)
    , scheduler(_scheduler)
    , blockSize(_block_size)
{}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
void ParallelTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::run(const size_t numberOfSteps)
{
    // initialize current solution
    TSolution currentSolution(bestSolution);

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        auto moves = neighborhood.getMoves(currentSolution);
        if (moves.empty())
        {
            std::cout << "Moves exhaustion, return in bestSolution" << '\n';
            currentSolution = bestSolution;
            continue;
        }

        auto task = [](const ITabuList& tabu_list, const IAspirationCriteria& aspiration_criteria,
                const TSolution solution, const std::vector< std::unique_ptr<IMove> >& moves, size_t first, size_t last) -> std::pair<int, double>
        {
            //std::cout << "Work " << std::this_thread::get_id() << " thread" << std::endl;
            std::vector<double> objectiveValues;
            std::vector<size_t> indexs;

            // generate all (!tabu or aspiration) solutions from the current
            for (size_t i = first; i < last; ++i)
            {
                if (!tabu_list.isTabu(*moves[i]) || aspiration_criteria.overrideTabu(solution, *moves[i]))
                {
                    objectiveValues.push_back(solution.tryOnMove(*moves[i]));
                    indexs.push_back(i);
                }
            }

            if (objectiveValues.empty())
            {
                // -1 => nonexistent index
                return std::make_pair (-1, -1.0);
            }

            // choice of the local best solution
            auto local_best = std::min_element(objectiveValues.begin(), objectiveValues.end());
            size_t idx = std::distance(objectiveValues.begin(), local_best);

            return std::make_pair (static_cast<int>(indexs[idx]), *local_best);
        };

        // for each block we calculate an index of the best in the array of moves
        size_t number_of_block = ((moves.size() + blockSize) / blockSize);
        std::vector< std::future< std::pair<int, double> > > index_of_best_moves(number_of_block - 1);

        for (size_t index_of_block = 0; index_of_block + 1 < number_of_block; ++index_of_block)
        {
            index_of_best_moves[index_of_block] = scheduler->schedule(task, std::ref(tabuList), std::ref(aspirationCriteria), currentSolution,
                                                                      std::ref(moves), index_of_block * blockSize, (index_of_block + 1) * blockSize);
        }
        size_t last = (number_of_block - 1) * blockSize + blockSize;
        if (last > moves.size())
        {
            last = moves.size();
        }
        auto last_index = task(tabuList, aspirationCriteria, currentSolution, moves, (number_of_block - 1) * blockSize, last);

        // choice of the best solution
        int index_of_best = -1;
        double best_obj_val = -1.0;
        for (size_t j = 0; j < number_of_block - 1; ++j)
        {
            auto result_of_calculations = index_of_best_moves[j].get();
            if (-1 != result_of_calculations.first)
            {
                if (-1 == index_of_best)
                {
                    index_of_best = result_of_calculations.first;
                    best_obj_val = result_of_calculations.second;
                }
                else if (best_obj_val > result_of_calculations.second)
                {
                    index_of_best = result_of_calculations.first;
                    best_obj_val = result_of_calculations.second;
                }
            }
        }

        // handle last index
        if (-1 != last_index.first)
        {
            if (-1 == index_of_best)
            {
                index_of_best = last_index.first;
                best_obj_val = last_index.second;
            }
            else if (best_obj_val > last_index.second)
            {
                index_of_best = last_index.first;
                best_obj_val = last_index.second;
            }
        }

        if (-1 == index_of_best)
        {
            std::cout << "all moves is tabu" << '\n';
            break;
        }

        // apply move
        currentSolution.applyMove(*moves[index_of_best]);
        //std::cout << best_obj_val << '\n';

        if (best_obj_val <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            break;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(*moves[index_of_best]);
        aspirationCriteria.update(currentSolution);
    }
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
TSolution ParallelTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestSolution()
{
    return bestSolution;
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
void ParallelTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::setStartSolution(const TSolution& solution)
{
    bestSolution = solution;
}
