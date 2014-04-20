#ifndef ASSIGNMENTPROBLEMRANDOMNEIGHBORHOOD_HPP
#define ASSIGNMENTPROBLEMRANDOMNEIGHBORHOOD_HPP

#include "Neighborhood/INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"
#include <vector>

template<class GetterServers, class GetterDisksFromServers>
class AssignmentProblemRandomNeighborhood : public INeighborhood, private GetterServers, private GetterDisksFromServers
{
public:
    AssignmentProblemRandomNeighborhood(const TaskData& _data, const size_t _minNumberOfServers);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    using GetterDisksFromServers::getDisks;

    using GetterServers::getServers;

    std::vector< std::vector<int> > calcCost(const std::vector< std::pair< size_t, std::vector<size_t> > >& serversWithDisks, const ISolution& solution, const std::vector<size_t>& disks) const;

private:
    const TaskData& data;

    const size_t minNumberOfServers;
};

#include "AssignmentProblemRandomNeighborhood-inl.hpp"

#endif // ASSIGNMENTPROBLEMRANDOMNEIGHBORHOOD_HPP
