#include "CompoundMove.hpp"
#include <memory>

CompoundMove::CompoundMove(std::vector<IMove::AtomMove>&& _moves)
    : moves(std::move(_moves))
{
}

std::vector<IMove::AtomMove> CompoundMove::getAtomMove() const
{
    return moves;
}
