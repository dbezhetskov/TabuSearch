#ifndef LANDINGTABUSEARCH_HPP
#define LANDINGTABUSEARCH_HPP

#include "ITabuSearch.hpp"
#include "Scheduler/Scheduler.hpp"
#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"
#include <memory>

template <class TTabuSearch, class TSolution = VectorSolution>
class LandingTabuSearch : public ITabuSearch<TSolution>
{
public:
    LandingTabuSearch
    (
            const size_t                            _number_of_landings,
            const size_t                            _depth,
            const std::shared_ptr<Scheduler>        _scheduler,
            const TTabuSearch&                      _tabu_search,
            const TSolution&                        initialSolution,
            const TaskData&                         _data
    );

    virtual void run(const size_t numberOfSteps) override;

    virtual TSolution getBestSolution() override;

    virtual void setStartSolution(const TSolution& solution) override;

private:
    TSolution bestSolution;

    std::shared_ptr<Scheduler> scheduler;

    size_t numberOfLandings;

    size_t depth;

    TTabuSearch tabuSearch;

    const TaskData& data;
};

#endif // LANDINGTABUSEARCH_HPP
