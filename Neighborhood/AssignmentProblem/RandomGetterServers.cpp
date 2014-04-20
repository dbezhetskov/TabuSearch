#include "RandomGetterServers.hpp"
#include <cassert>
#include <random>
#include <chrono>

std::vector<size_t> RandomGetterServers::getServers(const size_t _numberOfServers, const size_t numberAllServers) const
{
    assert(_numberOfServers > 0);

    std::vector<bool> used(numberAllServers, false);
    size_t uniqueServersCounter = 0;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, _numberOfServers - 1);

    std::vector<size_t> servers;

    while (uniqueServersCounter < _numberOfServers)
    {
        size_t server = distribution(generator);
        if (!used[server])
        {
            used[server] = true;
            servers.push_back(server);
            ++uniqueServersCounter;
        }
    }

    return servers;
}
