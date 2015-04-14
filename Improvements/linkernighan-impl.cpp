#include "LinKernighan.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"

template class LinKernighan<MoveNeighborhood>;
template class LinKernighan<SwapNeighborhood>;
template class LinKernighan<UnionNeighborhood<MoveNeighborhood, SwapNeighborhood>>;

template class LinKernighan<RandomMoveNeighborhood>;
template class LinKernighan<RandomSwapNeighborhood>;
template class LinKernighan<UnionNeighborhood<RandomMoveNeighborhood, RandomSwapNeighborhood>>;
