#include "SimpleTabuSearch.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::SimpleTabuSearch
(
        const TSolution&                        _initSolution,
        const TNeighborhood&                    _neighborhood,
        const TTabuList&                        _tabuList,
        const TAspirationCriteria&              _aspirationCriteria
)
    : neighborhood(_neighborhood)
    , tabuList(_tabuList)
    , aspirationCriteria(_aspirationCriteria)
    , bestSolution(_initSolution)
{
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
void SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::run(const size_t numberOfSteps)
{
    TSolution currentSolution(bestSolution);

    for (size_t i = 0; i < numberOfSteps; ++i)
	{
        auto bestLocalMovePtr = getBestMove(currentSolution);

        if (nullptr == bestLocalMovePtr)
        {
            continue;
        }

        currentSolution.applyMove(*bestLocalMovePtr);

        if (currentSolution.getObjectiveValue() <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            break;
        }

        tabuList.update(*bestLocalMovePtr);
        aspirationCriteria.update(currentSolution);
	}
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
TSolution SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestSolution()
{
    return bestSolution;
}

template < class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution >
void SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::setStartSolution(const TSolution& solution)
{
    bestSolution = solution;
}

namespace
{

struct ObjectiveValueWithIndex
{
    ObjectiveValueWithIndex(double _objectiveValue, size_t _index)
        : objectiveValue(_objectiveValue)
        , index(_index)
    {}

    bool operator< (const ObjectiveValueWithIndex& other) const { return objectiveValue < other.objectiveValue; }

    const double objectiveValue;
    const size_t index;
};

}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
std::unique_ptr<IMove> SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestMove(const TSolution& currentSolution)
{
    auto moves = neighborhood.getMoves(currentSolution);

    if (moves.empty())
    {
        std::cout << "Moves exhaustion" << '\n';
        return nullptr;
    }

    // generate all (non Tabu(X) or Aspiration(X)) moves from the current
    std::vector<ObjectiveValueWithIndex> objectiveValuesWithIndex;
    for (size_t i = 0; i < moves.size(); ++i)
    {
        IMove& move = *moves[i];
        if (!tabuList.isTabu(move) || aspirationCriteria.overrideTabu(currentSolution, move))
        {
            objectiveValuesWithIndex.emplace_back(currentSolution.tryOnMove(move), i);
        }
    }

    if (objectiveValuesWithIndex.empty())
    {
        std::cout << "All solution tabu!" << '\n';
        return nullptr;
    }

    // choice of the best move
    auto bestObjectiveValue = std::min_element(objectiveValuesWithIndex.begin(), objectiveValuesWithIndex.end());
    return std::move(moves[bestObjectiveValue->index]);
}
