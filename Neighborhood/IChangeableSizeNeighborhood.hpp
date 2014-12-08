#ifndef ICHANGEABLESIZENEIGHBORHOOD_HPP
#define ICHANGEABLESIZENEIGHBORHOOD_HPP

#include "INeighborhood.hpp"

class IChangeableSizeNeighborhood : public INeighborhood
{
public:
    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const = 0;

    virtual void setSize(const size_t _size) = 0;
};

#endif // ICHANGEABLESIZENEIGHBORHOOD_HPP
