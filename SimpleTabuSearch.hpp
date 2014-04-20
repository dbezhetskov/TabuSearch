#ifndef _SIMPLE_TABU_SEARCH_H_
#define _SIMPLE_TABU_SEARCH_H_

#include "TaskData.hpp"
#include "ITabuSearch.hpp"
#include "INeighborhood.hpp"
#include "ITabuList.hpp"
#include "IAspirationCriteria.hpp"

template<class Solution, class Neighborhood, class TabuList, class AspirationCriteria>
class SimpleTabuSearch : public ITabuSearch<Solution>
{
public:
    SimpleTabuSearch
    (
            Solution&&              _initSolution,
            Neighborhood&&          _neighborhood,
            TabuList&&              _tabuList,
            AspirationCriteria&&    _aspirationCriteria
    );

	virtual void run(const size_t numberOfSteps) override;

    virtual Solution getBestSolution() override;

private:
    Neighborhood neighborhood;

    TabuList tabuList;

    AspirationCriteria aspirationCriteria;

    Solution bestSolution;
};

#include "SimpleTabuSearch-inl.hpp"

#endif // !_SIMPLE_TABU_SEARCH_H_
