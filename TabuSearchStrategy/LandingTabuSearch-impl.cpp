#include "LandingTabuSearch.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "SimpleTabuSearchReverse.hpp"

template class LandingTabuSearch<SimpleTabuSearchReverse<MoveNeighborhood>>;
