#ifndef RANDOMSWAPNEIGHBORHOOD_HPP
#define RANDOMSWAPNEIGHBORHOOD_HPP

#include "Neighborhood/INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"

class RandomSwapNeighborhood : public INeighborhood
{
public:
    RandomSwapNeighborhood(const TaskData& _data, size_t _numberOfAttempts);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    std::vector< std::pair<size_t, size_t> > getSomePairDisk(const ISolution &solution) const;

private:
    const TaskData& data;

    size_t numberOfAttempts;
};

#endif // RANDOMSWAPNEIGHBORHOOD_HPP
