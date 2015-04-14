#ifndef UNIONNEIGHBORHOOD_HPP
#define UNIONNEIGHBORHOOD_HPP

#include "INeighborhood.hpp"
#include <vector>
#include <memory>

template< class TFirstNeighborhood, class TSecondNeighborhood >
class UnionNeighborhood : public INeighborhood
{
public:
    UnionNeighborhood(const TFirstNeighborhood& first, const TSecondNeighborhood& second);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    const TFirstNeighborhood& first_neighborhood;
    const TSecondNeighborhood& second_neighborhood;
};

#endif // UNIONNEIGHBORHOOD_HPP
