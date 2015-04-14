#include "SimpleTabuSearchReverse.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

template <class TNeighborhood, class TImprovement, class TTabuList, class TAspirationCriteria, class TSolution>
SimpleTabuSearchReverse<TNeighborhood, TImprovement, TTabuList, TAspirationCriteria, TSolution>::SimpleTabuSearchReverse
(
        const TSolution&                        _initSolution,
        const TNeighborhood&                    _neighborhood,
        const TTabuList&                        _tabuList,
        const TAspirationCriteria&              _aspirationCriteria,
        const TImprovement&                     _improver,
        const size_t                            _numberOfUnchanged
)
    : SimpleTabuSearch<TNeighborhood, TTabuList, TAspirationCriteria, TSolution>(_initSolution, _neighborhood, _tabuList, _aspirationCriteria)
    , improver(_improver)
    , numberOfUnchanged(_numberOfUnchanged)
{}

template <class TNeighborhood, class TImprovement, class TTabuList, class TAspirationCriteria, class TSolution>
void SimpleTabuSearchReverse<TNeighborhood, TImprovement, TTabuList, TAspirationCriteria, TSolution>::run(const size_t numberOfSteps)
{
    bool isMoveUp = false;
    TSolution currentSolution(bestSolution);
    size_t currentNumberOfUnchanged = 0;

    for (size_t i = 0; i < numberOfSteps; ++i)
    {
        std::cout << "Number of move " << i << std::endl;

        auto bestLocalMovePtr = getBestMove(currentSolution);

        if (nullptr == bestLocalMovePtr)
        {
            continue;
        }

        // apply move
        currentSolution.applyMove(*bestLocalMovePtr);

        ++currentNumberOfUnchanged;
        // std::cout << "number of unchanged " << currentNumberOfUnchanged << std::endl;

        if (currentSolution.getObjectiveValue() <= bestSolution.getObjectiveValue())
        {
            bestSolution = currentSolution;
            currentNumberOfUnchanged = 0;
        }

        if (i >= 50)
        {
            isMoveUp = true;
        }

        double overheadsCoefficient = currentSolution.getOverheadsCoefficient() + 0.1;
        if (isMoveUp)
        {
            std::cout << "MOVE UP!!!" << std::endl;
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
