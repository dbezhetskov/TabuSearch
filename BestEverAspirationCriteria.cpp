#include "BestEverAspirationCriteria.hpp"

BestEverAspirationCriteria::BestEverAspirationCriteria(double _bestObjectiveValue)
{
    bestObjectiveValue = _bestObjectiveValue;
}

bool BestEverAspirationCriteria::overrideTabu(const ISolution& solution, const IMove& move) const
{
    if (bestObjectiveValue > solution.tryOnMove(move))
    {
        return true;
    }
    return false;
}

void BestEverAspirationCriteria::update(const ISolution& decision)
{
    if (decision.getObjectiveValue() < bestObjectiveValue)
    {
        bestObjectiveValue = decision.getObjectiveValue();
    }
}
