#include "UnionNeighborhood.hpp"

UnionNeighborhood::UnionNeighborhood(std::vector<std::unique_ptr<INeighborhood> >&& _neighborhoods)
    : neighborhoods(std::move(_neighborhoods))
{
}

std::vector< std::unique_ptr<IMove> > UnionNeighborhood::getMoves(const ISolution& solution) const
{
    std::vector< std::unique_ptr<IMove> > moves;
    for (auto& neighborhood : neighborhoods)
    {
        auto anotherMoves = neighborhood->getMoves(solution);
        moves.insert(moves.end(), std::make_move_iterator(anotherMoves.begin()), std::make_move_iterator(anotherMoves.end()));
    }
    return moves;
}
