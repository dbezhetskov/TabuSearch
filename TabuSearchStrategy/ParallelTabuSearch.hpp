#ifndef PARALLELTABUSEARCH_HPP
#define PARALLELTABUSEARCH_HPP

#include "ITabuSearch.hpp"
#include "Scheduler/Scheduler.hpp"
#include "Solution/VectorSolution.hpp"
#include "AspirationCriteria/BestEverAspirationCriteria.hpp"
#include "TabuList/HashSetTabuList.hpp"
#include <memory>

template <class TNeighborhood, class TTabuList = HashSetTabuList, class TAspirationCriteria = BestEverAspirationCriteria, class TSolution = VectorSolution>
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

    virtual void setStartSolution(const TSolution& solution) override;

private:
    TNeighborhood neighborhood;

    TTabuList tabuList;

    TAspirationCriteria aspirationCriteria;

    TSolution bestSolution;

    std::shared_ptr<Scheduler> scheduler;

    const size_t blockSize;
};

#endif // PARALLELTABUSEARCH_HPP
