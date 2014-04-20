#include "SimpleMove.hpp"

SimpleMove::SimpleMove(size_t _destination, size_t _source, size_t _diskId)
    : destination(_destination)
    , source(_source)
    , diskId(_diskId)
{
}

std::vector<IMove::AtomMove> SimpleMove::getAtomMove() const
{
    return std::vector<IMove::AtomMove> (1, IMove::AtomMove(destination, source, diskId));
}
