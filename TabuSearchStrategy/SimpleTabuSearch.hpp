#ifndef _SIMPLE_TABU_SEARCH_H_
#define _SIMPLE_TABU_SEARCH_H_

#include "InitialStandardData/TaskData.hpp"
#include "ITabuSearch.hpp"
#include "Neighborhood/INeighborhood.hpp"
#include "TabuList/ITabuList.hpp"
#include "AspirationCriteria/IAspirationCriteria.hpp"

template<class Solution>
class SimpleTabuSearch : public ITabuSearch<Solution>
{
public:
    SimpleTabuSearch
    (
            Solution&&                              _initSolution,
            std::unique_ptr<INeighborhood>&&        _neighborhood,
            std::unique_ptr<ITabuList>&&            _tabuList,
            std::unique_ptr<IAspirationCriteria>&&  _aspirationCriteria
    );

	virtual void run(const size_t numberOfSteps) override;

    virtual Solution getBestSolution() override;

private:
    std::unique_ptr<INeighborhood> neighborhood;

    std::unique_ptr<ITabuList> tabuList;

    std::unique_ptr<IAspirationCriteria> aspirationCriteria;

    Solution bestSolution;
};

#include "SimpleTabuSearch-inl.hpp"

#endif // !_SIMPLE_TABU_SEARCH_H_
