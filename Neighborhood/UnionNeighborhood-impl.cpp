#include "UnionNeighborhood.cpp"
#include "MoveNeighborhood.hpp"
#include "SwapNeighborhood.hpp"
#include "RandomMoveNeighborhood.hpp"
#include "RandomSwapNeighborhood.hpp"

template class UnionNeighborhood<MoveNeighborhood, SwapNeighborhood>;
template class UnionNeighborhood<SwapNeighborhood, MoveNeighborhood>;
template class UnionNeighborhood<RandomMoveNeighborhood, RandomSwapNeighborhood>;
template class UnionNeighborhood<RandomSwapNeighborhood, RandomMoveNeighborhood>;
