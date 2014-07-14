#include "ProportionSwap.hpp"
#include "Move/CompoundMove.hpp"
#include "InitialStandardData/TaskData.hpp"

ProportionSwap::ProportionSwap(const TaskData& _data)
    : data(_data)
{
}

std::vector<std::unique_ptr<IMove> > ProportionSwap::getMoves(const ISolution &solution) const
{
    // for each server determine what disks belong it
    std::vector< std::vector<size_t> > serverWithDisks(data.numberOfServers);
    auto distribution = solution.getDistribution();
    for (size_t diskId = 0; diskId < distribution.size(); ++diskId)
    {
        serverWithDisks[distribution[diskId]].push_back(diskId);
    }

    std::vector< std::unique_ptr<IMove> > result;
    for (size_t server = 0; server < serverWithDisks.size(); ++server)
    {
        if (serverWithDisks[server].size() > 1)
        {
            // for each pair disks with server
            for (size_t i = 0; i < serverWithDisks[server].size(); ++i)
            {
                for (size_t j = i + 1; j < serverWithDisks[server].size(); ++j)
                {
                    // for each another server
                    for (size_t anotherServer = 0; anotherServer < data.numberOfServers; ++anotherServer)
                    {
                        if (anotherServer != server)
                        {
                            // for each disk on anotherServer
                            for (size_t k = 0; k < serverWithDisks[anotherServer].size(); ++k)
                            {
                                std::vector<IMove::AtomMove> moves;

                                // exchange the two disks on one
                                moves.emplace_back(anotherServer, server, serverWithDisks[server][i]);
                                moves.emplace_back(anotherServer, server, serverWithDisks[server][j]);
                                moves.emplace_back(server, anotherServer, serverWithDisks[anotherServer][k]);

                                std::unique_ptr<IMove> move(new CompoundMove(std::move(moves)));
                                if (solution.moveIsCorrect(*move))
                                {
                                    result.emplace_back(std::move(move));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}
