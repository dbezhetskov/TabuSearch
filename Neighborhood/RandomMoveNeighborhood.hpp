#ifndef RANDOMMOVENEIGHBORHOOD_HPP
#define RANDOMMOVENEIGHBORHOOD_HPP

#include "Neighborhood/INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"

class RandomMoveNeighborhood : public INeighborhood
{
public:
    RandomMoveNeighborhood(const TaskData& _data, size_t _numberOfAttempts);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    /** diskID, destination */
    std::vector< std::pair<size_t, size_t> > getSomeMoves(const ISolution& solution) const;

private:
    const TaskData& data;

    size_t numberOfAttempts;
};

#endif // RANDOMMOVENEIGHBORHOOD_HPP
