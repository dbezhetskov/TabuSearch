#include "SimpleTabuSearch.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "Neighborhood/SwapNeighborhood.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"
#include "Neighborhood/RandomSwapNeighborhood.hpp"
#include "Neighborhood/UnionNeighborhood.hpp"

template class SimpleTabuSearch<MoveNeighborhood>;
template class SimpleTabuSearch<SwapNeighborhood>;
template class SimpleTabuSearch<UnionNeighborhood<MoveNeighborhood, SwapNeighborhood>>;

template class SimpleTabuSearch<RandomMoveNeighborhood>;
template class SimpleTabuSearch<RandomSwapNeighborhood>;
template class SimpleTabuSearch<UnionNeighborhood<RandomMoveNeighborhood, RandomSwapNeighborhood>>;

