#include "LandingTabuSearch.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "SimpleTabuSearchReverse.hpp"
#include "Improvements/LinKernighan.hpp"
#include "Neighborhood/RandomMoveNeighborhood.hpp"

template class LandingTabuSearch<SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<MoveNeighborhood>>>;
template class LandingTabuSearch<SimpleTabuSearchReverse<MoveNeighborhood, LinKernighan<RandomMoveNeighborhood>>>;
