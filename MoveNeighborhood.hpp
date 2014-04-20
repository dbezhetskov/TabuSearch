#ifndef MOVENEIGHBORHOOD_H
#define MOVENEIGHBORHOOD_H

#include "INeighborhood.hpp"
#include "TaskData.hpp"

class MoveNeighborhood : public INeighborhood
{
public:
    MoveNeighborhood(const std::shared_ptr<const TaskData> _data);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    std::shared_ptr<const TaskData> data;
};

#endif // MOVENEIGHBORHOOD_H
