#ifndef UNIONNEIGHBORHOOD_HPP
#define UNIONNEIGHBORHOOD_HPP

#include "INeighborhood.hpp"
#include <vector>
#include <memory>

class UnionNeighborhood : public INeighborhood
{
public:
    UnionNeighborhood(std::vector< std::unique_ptr<INeighborhood> >&& _neighborhoods);

    virtual std::vector< std::unique_ptr<IMove> > getMoves(const ISolution& solution) const override;

private:
    std::vector< std::unique_ptr<INeighborhood> > neighborhoods;
};

#endif // UNIONNEIGHBORHOOD_HPP
