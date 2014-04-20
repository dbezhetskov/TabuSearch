#include "RandomOneDiskByServerGetter.hpp"
#include <random>
#include <chrono>

std::vector< std::pair< size_t, std::vector<size_t> > > RandomOneDiskByServerGetter::getDisks(const ISolution& solution, const std::vector<size_t>& servers) const
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
            std::uniform_int_distribution<int> uniformDistribution(0, server.second.size() - 1);
            size_t diskIndex = uniformDistribution(generator);

            server.second = std::vector<size_t>(1, server.second[diskIndex]);
        }
    }

    return serversWithDisks;
}

std::vector<size_t> RandomOneDiskByServerGetter::selectAllDiskForServer(const ISolution &solution, const size_t server) const
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
