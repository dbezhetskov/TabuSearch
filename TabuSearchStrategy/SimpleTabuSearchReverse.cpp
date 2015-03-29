#include "SimpleTabuSearchReverse.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
SimpleTabuSearchReverse<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::SimpleTabuSearchReverse
(
        const TSolution&                        _initSolution,
        const TNeighborhood&                    _neighborhood,
        const TTabuList&                        _tabuList,
        const TAspirationCriteria&              _aspirationCriteria,
        const size_t                            _numberOfUnchanged
)
    : neighborhood(_neighborhood)
    , tabuList(_tabuList)
    , aspirationCriteria(_aspirationCriteria)
    , bestSolution(_initSolution)
    , numberOfUnchanged(_numberOfUnchanged)
{
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
void SimpleTabuSearchReverse<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::run(const size_t numberOfSteps)
{
    bool isMoveUp = false;
    TSolution currentSolution(bestSolution);
    size_t currentNumberOfUnchanged = 0;

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        auto bestLocalMovePtr = getBestMove(currentSolution);

        if (nullptr == bestLocalMovePtr)
        {
            continue;
        }

        // apply move
        currentSolution.applyMove(*bestLocalMovePtr);

        ++currentNumberOfUnchanged;
        std::cout << "number of unchanhed " << currentNumberOfUnchanged << std::endl;

        if (currentSolution.getObjectiveValue() <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
            currentNumberOfUnchanged = 0;
        }

        if (numberOfUnchanged <= currentNumberOfUnchanged)
        {
            isMoveUp = true;
        }

        if (isMoveUp)
        {
            std::cout << "MOVE UP!!!" << std::endl;
            double overheadsCoefficient = currentSolution.getOverheadsCoefficient() + 0.1;
            currentSolution.setOverheadsCoefficient(overheadsCoefficient);
            bestSolution.setOverheadsCoefficient(overheadsCoefficient);
        }

        if (0 == bestSolution.getObjectiveValue())
        {
            // the objectives are achieved
            break;
        }

        // update tabuList and AspirationCriteria
        tabuList.update(*bestLocalMovePtr);
        aspirationCriteria.update(currentSolution);
    }
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
TSolution SimpleTabuSearchReverse<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestSolution()
{
    return bestSolution;
}

template <class TNeighborhood, class TTabuList, class TAspirationCriteria, class TSolution>
void SimpleTabuSearchReverse<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::setStartSolution(const TSolution &solution)
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
std::unique_ptr<IMove> SimpleTabuSearchReverse<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>::getBestMove(const TSolution& currentSolution)
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
