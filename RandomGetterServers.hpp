#ifndef RANDOMGETTERSERVERS_HPP
#define RANDOMGETTERSERVERS_HPP

#include "IGetterServers.hpp"

class RandomGetterServers : public IGetterServers
{
public:
    virtual std::vector<size_t> getServers(const size_t _numberOfServers, const size_t numberAllServers) const override;
};

#endif // RANDOMGETTERSERVERS_HPP
