#ifndef PARALLELTABUSEARCHINL_HPP
#define PARALLELTABUSEARCHINL_HPP

#include "ParallelTabuSearch.hpp"
#include <utility>
#include <algorithm>
#include <limits>
#include <functional>
#include <iterator>
#include <cassert>
#include <iostream>

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
ParallelTabuSearch< TSolution, TNeighborhood, TTabuList, TAspirationCriteria >::ParallelTabuSearch
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
    , block_size(_block_size)
{}

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
void ParallelTabuSearch< TSolution, TNeighborhood, TTabuList, TAspirationCriteria >::run(const size_t numberOfSteps)
{
    // initialize current solution
    TSolution currentSolution(bestSolution);

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        auto moves = neighborhood.getMoves(currentSolution);
        if (moves.empty())
        {
            std::cout << "Moves exhaustion, return in bestSolution" << std::endl;
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
        size_t number_of_block = ((moves.size() + block_size) / block_size);
        std::vector< std::future< std::pair<int, double> > > index_of_best_moves(number_of_block - 1);

        for (size_t index_of_block = 0; index_of_block + 1 < number_of_block; ++index_of_block)
        {
            index_of_best_moves[index_of_block] = scheduler->schedule(task, std::ref(tabuList), std::ref(aspirationCriteria), currentSolution,
                                                                      std::ref(moves), index_of_block * block_size, (index_of_block + 1) * block_size);
        }
        size_t last = (number_of_block - 1) * block_size + block_size;
        if (last > moves.size())
        {
            last = moves.size();
        }
        auto last_index = task(tabuList, aspirationCriteria, currentSolution, moves, (number_of_block - 1) * block_size, last);

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
            std::cout << "all moves is tabu" << std::endl;
            break;
        }

        // apply move
        currentSolution.applyMove(*moves[index_of_best]);
        std::cout << best_obj_val << std::endl;

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

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
TSolution ParallelTabuSearch< TSolution, TNeighborhood, TTabuList, TAspirationCriteria >::getBestSolution()
{
    return bestSolution;
}

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
void ParallelTabuSearch< TSolution, TNeighborhood, TTabuList, TAspirationCriteria >::setStartSolution(TSolution solution)
{
    bestSolution = solution;
}

#endif // PARALLELTABUSEARCHINL_HPP
