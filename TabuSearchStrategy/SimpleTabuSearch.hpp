#ifndef _SIMPLE_TABU_SEARCH_H_
#define _SIMPLE_TABU_SEARCH_H_

#include "ITabuSearch.hpp"
#include "Solution/VectorSolution.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"

template < class TNeighborhood, class TTabuList = HashSetTabuList, class TAspirationCriteria = BestEverAspirationCriteria, class TSolution = VectorSolution >
class SimpleTabuSearch : public ITabuSearch<TSolution>
{
public:
    SimpleTabuSearch
    (
            const TSolution&                        _initSolution,
            const TNeighborhood&                    _neighborhood,
            const TTabuList&                        _tabuList,
            const TAspirationCriteria&              _aspirationCriteria
    );

    virtual void run(const size_t numbersOfSteps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(TSolution solution) override;

private:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;

private:
    struct ObjectiveValueWithIndex
    {
        ObjectiveValueWithIndex(double _objectiveValue, size_t _index)
            : objectiveValue(_objectiveValue)
            , index(_index)
        {}

        bool operator < (const ObjectiveValueWithIndex& other) const { return objectiveValue < other.objectiveValue; }

        const double objectiveValue;
        const size_t index;
    };
};

#include "SimpleTabuSearch-inl.hpp"

#endif // !_SIMPLE_TABU_SEARCH_H_
