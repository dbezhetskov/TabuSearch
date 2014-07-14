#ifndef PARALLELTABUSEARCH_HPP
#define PARALLELTABUSEARCH_HPP

#include "ITabuSearch.hpp"
#include "Neighborhood/INeighborhood.hpp"
#include "TabuList/ITabuList.hpp"
#include "AspirationCriteria/IAspirationCriteria.hpp"
#include "Scheduler/Scheduler.hpp"
#include <memory>

template<class TSolution>
class ParallelTabuSearch : public ITabuSearch<TSolution>
{
public:
    ParallelTabuSearch
    (
            std::shared_ptr<Scheduler>              _scheduler,
            TSolution                               initialSolution,
            const size_t                            _block_size,
            std::unique_ptr<INeighborhood>&&        _neighborhood,
            std::unique_ptr<ITabuList>&&            _tabuList,
            std::unique_ptr<IAspirationCriteria>&&  _aspirationCriteria
    );

    virtual void run(const size_t numberOfSteps) override;

    virtual TSolution getBestSolution() override;

private:
    std::unique_ptr<INeighborhood> neighborhood;

    std::unique_ptr<ITabuList> tabuList;

    std::unique_ptr<IAspirationCriteria> aspirationCriteria;

    TSolution bestSolution;

    std::shared_ptr<Scheduler> scheduler;

    size_t block_size;
};

#include "ParallelTabuSearch-inl.hpp"

#endif // PARALLELTABUSEARCH_HPP
