#ifndef PARALLELTABUSEARCH_HPP
#define PARALLELTABUSEARCH_HPP

#include "ITabuSearch.hpp"
#include "Neighborhood/INeighborhood.hpp"
#include "TabuList/ITabuList.hpp"
#include "AspirationCriteria/IAspirationCriteria.hpp"
#include "Scheduler/Scheduler.hpp"
#include <memory>

template< class TSolution, class TNeighborhood, class TTabuList, class TAspirationCriteria >
class ParallelTabuSearch : public ITabuSearch<TSolution>
{
public:
    ParallelTabuSearch
    (
            std::shared_ptr<Scheduler>              _scheduler,
            TSolution                               &initialSolution,
            const size_t                            _block_size,
            TNeighborhood                           &_neighborhood,
            TTabuList                               &_tabuList,
            TAspirationCriteria                     &_aspirationCriteria
    );

    virtual void run(const size_t numberOfSteps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(TSolution solution) override;

private:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;

    std::shared_ptr<Scheduler> scheduler;

    size_t block_size;
};

#include "ParallelTabuSearch-inl.hpp"

#endif // PARALLELTABUSEARCH_HPP
