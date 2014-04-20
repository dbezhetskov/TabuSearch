#ifndef IGETTERDISKS_HPP
#define IGETTERDISKS_HPP

#include "Solution/ISolution.hpp"
#include <vector>
#include <utility>

class IGetterDisks
{
public:
    virtual std::vector< std::pair< size_t, std::vector<size_t> > > getDisks(const ISolution& solution, const std::vector<size_t>& servers) const = 0;

    virtual ~IGetterDisks() {}
};

#endif // IGETTERDISKS_HPP
