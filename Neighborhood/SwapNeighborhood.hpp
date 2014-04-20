#ifndef SWAPNEIGHBORHOOD_HPP
#define SWAPNEIGHBORHOOD_HPP

#include "INeighborhood.hpp"

class SwapNeighborhood : public INeighborhood
{
public:
    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;
};

#endif // SWAPNEIGHBORHOOD_HPP
