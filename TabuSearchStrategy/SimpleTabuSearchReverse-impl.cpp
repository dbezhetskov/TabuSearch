#include "SimpleTabuSearchReverse.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"
#include "Improvements/LinKernighan.hpp"

template class SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<MoveNeighborhood>>;
template class SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<RandomMoveNeighborhood>>;
template class SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<RandomSwapNeighborhood>>;
template class SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<UnionNeighborhood<MoveNeighborhood, SwapNeighborhood>>>;
template class SimpleTabuSearchReverse<SwapNeighborhood, LinKernighan<MoveNeighborhood>>;

template class SimpleTabuSearchReverse<UnionNeighborhood<MoveNeighborhood, SwapNeighborhood>, LinKernighan<MoveNeighborhood>>;

template class SimpleTabuSearchReverse<RandomMoveNeighborhood, LinKernighan<MoveNeighborhood>>;
template class SimpleTabuSearchReverse<RandomSwapNeighborhood, LinKernighan<MoveNeighborhood>>;
template class SimpleTabuSearchReverse<UnionNeighborhood<RandomMoveNeighborhood, RandomSwapNeighborhood>, LinKernighan<MoveNeighborhood>>;
