#ifndef LANDINGTABUSEARCH_HPP
#define LANDINGTABUSEARCH_HPP

#include "ITabuSearch.hpp"
#include "Neighborhood/INeighborhood.hpp"
#include "TabuList/ITabuList.hpp"
#include "AspirationCriteria/IAspirationCriteria.hpp"
#include "Scheduler/Scheduler.hpp"
#include "InitialStandardData/TaskData.hpp"
#include <memory>

template< class TSolution, class TTabuSearch >
class LandingTabuSearch : public ITabuSearch<TSolution>
{
public:
    LandingTabuSearch
    (
            size_t                                  _number_of_landings,
            size_t                                  _depth,
            std::shared_ptr<Scheduler>              _scheduler,
            const TTabuSearch&                      _tabu_search,
            const TSolution&                        initialSolution,
            const std::shared_ptr<const TaskData>   _data
    );

    virtual void run(const size_t number_of_steps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(TSolution solution) override;

private:
    TSolution bestSolution;

    std::shared_ptr<Scheduler> scheduler;

    size_t number_of_landings;

    size_t depth;

    TTabuSearch tabu_search;

    const std::shared_ptr<const TaskData> data;
};

#include "LandingTabuSearch-inl.hpp"

#endif // LANDINGTABUSEARCH_HPP
