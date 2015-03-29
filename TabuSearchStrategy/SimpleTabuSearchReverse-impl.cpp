#include "SimpleTabuSearchReverse.cpp"
#include "Neighborhood/MoveNeighborhood.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

template class SimpleTabuSearchReverse<MoveNeighborhood>;
