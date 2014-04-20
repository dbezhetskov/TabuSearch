#ifndef _I_TABU_SEARCH_H_
#define _I_TABU_SEARCH_H_

#include "Solution/ISolution.hpp"

template<class SolutionType>
class ITabuSearch
{
public:
	virtual void run(const size_t numberOfSteps) = 0;

    virtual SolutionType getBestSolution() = 0;

	virtual ~ITabuSearch() {}
};

#endif // !_I_TABU_SEARCH_H_
