#ifndef MOVENEIGHBORHOOD_H
#define MOVENEIGHBORHOOD_H

#include "INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"

class MoveNeighborhood : public INeighborhood
{
public:
    MoveNeighborhood(const TaskData &_data);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    const TaskData& data;
};

#endif // MOVENEIGHBORHOOD_H
