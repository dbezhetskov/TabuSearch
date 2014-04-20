#include "RandomAlignmentNeighborhood.hpp"
#include "CompoundMove.hpp"
#include <cstdlib>
#include <random>

RandomAlignmentNeighborhood::RandomAlignmentNeighborhood(const TaskData& _data, size_t _numberOfAttempts)
    : data(_data)
    , numberOfAttempts(_numberOfAttempts)
    , weightDisk(_data.numberOfDisks, 0)
{
    maxWeight = 0;

    // calc weight disks
    for (size_t diskId = 0; diskId < data.numberOfDisks; ++diskId)
    {
        double weight = 0;
        for (size_t resource = 0; resource < data.numberOfResource; ++ resource)
        {
            for (size_t time = 0; time < data.numberOfTimes; ++time)
            {
                weight += data.getCapacity(diskId, resource, time);
            }
        }
        if (maxWeight < weight)
        {
            maxWeight = weight;
        }

        weightDisk[diskId] = weight;
    }
}

std::vector<std::unique_ptr<IMove> > RandomAlignmentNeighborhood::getMoves(const ISolution& solution) const
{
    srand(time(0));

    // choose two distinct server
    size_t server1 = rand() % data.numberOfServers;

    size_t server2 = server1;
    while (server1 == (server2 = rand() % data.numberOfServers));

    std::vector<size_t> distributionDisk = solution.getDistribution();

    // shake
    std::vector< std::unique_ptr<IMove> > result;
    std::default_random_engine generator;
    for (size_t attempt = 0; attempt < numberOfAttempts; ++ attempt)
    {
        std::vector<IMove::AtomMove> moves;
        for (size_t diskId = 0; diskId < distributionDisk.size(); ++diskId)
        {
            if (distributionDisk[diskId] == server1 || distributionDisk[diskId] == server2)
            {
                std::bernoulli_distribution distribution((double)weightDisk[diskId] / maxWeight);
                size_t source = server2;
                size_t destination = server1;
                if (distributionDisk[diskId] == server1)
                {
                    std::swap(source, destination);
                }

                if (distribution(generator))
                {
                    moves.emplace_back(destination, source, diskId);
                }
            }
        }
        result.emplace_back(new CompoundMove(std::move(moves)));
    }

    return result;
}
