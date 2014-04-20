#ifndef _I_NEIGHBORHOOD_H_
#define _I_NEIGHBORHOOD_H_

#include "IMove.hpp"
#include "ISolution.hpp"
#include <vector>
#include <memory>

class INeighborhood
{
public:
    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const = 0;

    virtual ~INeighborhood() {}
};

#endif // !_I_NEIGHBORHOOD_H_
