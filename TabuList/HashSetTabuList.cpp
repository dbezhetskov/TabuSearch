#include "HashSetTabuList.hpp"
#include <cassert>
#include <iostream>

std::size_t HashSetTabuList::AtomMoveHash::operator()(const IMove::AtomMove& atomMove) const
{
    std::size_t result = 0;
    result ^= std::hash<size_t>()(atomMove.destination) ^ std::hash<int>()(atomMove.source) ^ std::hash<int>()(atomMove.diskId);
    return result;
}

HashSetTabuList::HashSetTabuList(const size_t _tenure)
    : tenure(_tenure)
{}

void HashSetTabuList::clear()
{
    tabuList.clear();
}

bool HashSetTabuList::isTabu(const IMove &move) const
{
    for (auto atomMove : move.getAtomMove())
    {
        if (tabuList.end() != tabuList.find(atomMove))
        {
            return true;
        }
    }
    return false;
}

void HashSetTabuList::update(const IMove& decisionMove)
{
    for (auto atomMove : decisionMove.getAtomMove())
    {
        IMove::AtomMove inverseMove(atomMove.source, atomMove.destination, atomMove.diskId);

        if (tabuList.end() == tabuList.find(inverseMove))
        {
            tabuList[inverseMove] = 0;
        }
    }

    for (auto itr = tabuList.begin(); itr != tabuList.end(); )
    {
        ++(itr->second);

        if (itr->second >= tenure)
        {
            itr = tabuList.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}
