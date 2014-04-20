#ifndef RANDOMALIGNMENTNEIGHBORHOOD_HPP
#define RANDOMALIGNMENTNEIGHBORHOOD_HPP

#include "INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"

class RandomAlignmentNeighborhood : public INeighborhood
{
public:
    RandomAlignmentNeighborhood(const TaskData& _data, size_t _numberOfAttempts);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    const TaskData& data;

    const size_t numberOfAttempts;

    std::vector<double> weightDisk;

    double maxWeight;
};

#endif // RANDOMALIGNMENTNEIGHBORHOOD_HPP
