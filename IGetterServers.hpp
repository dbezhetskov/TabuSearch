#ifndef IGETTERSERVERS_HPP
#define IGETTERSERVERS_HPP

#include <vector>
#include <cstddef>

class IGetterServers
{
public:
    virtual std::vector<size_t> getServers(const size_t numberOfServers, const size_t numberAllServers) const = 0;

    virtual ~IGetterServers() {}
};

#endif // IGETTERSERVERS_HPP
