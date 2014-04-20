#ifndef MIXTWONEIGHBORHOODONETABULISTTABUSEARCH_HPP
#define MIXTWONEIGHBORHOODONETABULISTTABUSEARCH_HPP

#include "ITabuSearch.hpp"

template<class Solution, class Neighborhood1, class Neighborhood2, class TabuList, class AspirationCriteria>
class MixTwoNeighborhoodOneTabuListTabuSearch : public ITabuSearch<Solution>
{
public:
    MixTwoNeighborhoodOneTabuListTabuSearch
    (
            Solution                _initSolution,
            Neighborhood1            _neighborhood1,
            Neighborhood2            _neighborhood2,
            TabuList                _tabuList,
            AspirationCriteria      _aspirationCriteria
    );

    virtual void run(const size_t numberOfSteps) override;

    virtual Solution getBestSolution() override;

private:
    Neighborhood1 neighborhood1;

    Neighborhood2 neighborhood2;

    TabuList tabuList;

    AspirationCriteria aspirationCriteria;

    Solution bestSolution;
};

#include "MixTwoNeighborhoodOneTabuListTabuSearch-inl.hpp"

#endif // MIXTWONEIGHBORHOODONETABULISTTABUSEARCH_HPP
