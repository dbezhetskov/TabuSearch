#include "RandomGetterDisks.hpp"
#include <random>
#include <chrono>
#include <algorithm>
#include <cassert>

std::vector< std::pair< size_t, std::vector<size_t> > > RandomGetterDisks::getDisks(const ISolution& solution, const std::vector<size_t>& servers) const
{
    std::vector< std::pair< size_t, std::vector<size_t> > > serversWithDisks;

    // all disks from each server
    for (const auto server : servers)
    {
        serversWithDisks.emplace_back(server, selectAllDiskForServer(solution, server));
    }

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (auto& server : serversWithDisks)
    {
        if (server.second.size() > 1)
        {
            std::uniform_int_distribution<int> uniformDistribution(1, server.second.size());
            size_t numberOfErase = uniformDistribution(generator);

            server.second = eraseSomeDisks(server.second, numberOfErase);
        }
    }

    return serversWithDisks;
}

std::vector<size_t> RandomGetterDisks::eraseSomeDisks(const std::vector<size_t>& disks, const size_t numberOfErase) const
{
    assert((int)disks.size() - (int)numberOfErase >= 0);

    if (disks.empty() || 0 == numberOfErase)
    {
        return std::vector<size_t>();
    }

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(1, disks.size() - 1);
    std::vector<size_t> safePositions;

    size_t uniqueCountSafePosition = 0;
    std::vector<bool> used(disks.size(), false);

    while (uniqueCountSafePosition < disks.size() - numberOfErase)
    {
        size_t diskId = distribution(generator);
        if (!used[diskId])
        {
            safePositions.push_back(diskId);
            used[diskId] = true;
            ++uniqueCountSafePosition;
        }
    }

    std::vector<size_t> resultDisks;
    for (auto position : safePositions)
    {
        resultDisks.push_back(disks[position]);
    }

    return resultDisks;
}

std::vector<size_t> RandomGetterDisks::selectAllDiskForServer(const ISolution &solution, const size_t server) const
{
    std::vector<size_t> disks;

    auto distribution = solution.getDistribution();
    for (size_t diskId = 0; diskId < distribution.size(); ++diskId)
    {
        if (distribution[diskId] == server)
        {
            disks.push_back(diskId);
        }
    }

    return disks;
}
