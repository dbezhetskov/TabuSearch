#include "RandomReduction.hpp"
#include <ctime>
#include <cstdlib>

template <class TSolution>
RandomReduction<TSolution>::RandomReduction(const TaskData &_data)
    : data(_data)
{
    std::srand(time(NULL));
}

template <class TSolution>
TSolution RandomReduction<TSolution>::reduce(const TSolution& solution)
{
    TSolution normalizeSolution(solution);
    std::vector<bool> colored;

    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        colored.push_back(solution.getServerColor(server) == Color::RED);
    }

    // for red disks
    std::vector<SimpleMove> moves;
    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        if (solution.getServerColor(server) == Color::RED)
        {
            auto movesForScatter = scatterDiskFromServer(Color::RED, server, &colored, solution);
            moves.insert(moves.end(), movesForScatter.begin(), movesForScatter.end());
        }
    }

    for (auto& move : moves) { normalizeSolution.applyMove(move); }
    moves.clear();

    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        if (colored[server])
        {
            auto movesForUnload = unloadDiskFromServer(Color::RED, server, &colored, normalizeSolution);
            moves.insert(moves.end(), movesForUnload.begin(), movesForUnload.end());
        }
    }

    for (auto& move : moves) { normalizeSolution.applyMove(move); }
    moves.clear();
    normalizeSolution.paintServers();

    // for yellow
    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        if (normalizeSolution.getServerColor(server) == Color::YELLOW)
        {
            colored[server] = true;
        }
    }

    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        if (normalizeSolution.getServerColor(server) == Color::YELLOW)
        {
            auto movesForScatter = scatterDiskFromServer(Color::YELLOW, server, &colored, normalizeSolution);
            moves.insert(moves.end(), movesForScatter.begin(), movesForScatter.end());
        }
    }
    for (auto& move : moves) { normalizeSolution.applyMove(move); }

    normalizeSolution.paintServers();

    return normalizeSolution;
}

template <class TSolution>
std::vector<SimpleMove> RandomReduction<TSolution>::scatterDiskFromServer(const RandomReduction::Color color,
                                                                          const size_t server, std::vector<bool> *colored,
                                                                          const TSolution& solution)
{
    std::vector<SimpleMove> moves;
    auto disks = getAllDiskFromServer(solution, server);

    // first -> survivor disk
    size_t survivorDiskIdx = 0;
    while (survivorDiskIdx < disks.size() && solution.getDiskColor(disks[survivorDiskIdx]) != color) { ++survivorDiskIdx; }

    for (size_t i = 0; i < disks.size(); ++i)
    {
        if (i != survivorDiskIdx && solution.getDiskColor(disks[i]) == color)
        {
            size_t newServer = getRandomServer(server, *colored);
            (*colored)[newServer] = true;
            moves.emplace_back(newServer, server, disks[i]);
        }
    }

    return moves;
}

template <class TSolution>
std::vector<SimpleMove> RandomReduction<TSolution>::unloadDiskFromServer(const RandomReduction::Color color,
                                                                         const size_t server,
                                                                         std::vector<bool>* colored,
                                                                         const TSolution& solution)
{
    std::vector<SimpleMove> moves;
    auto disks = getAllDiskFromServer(solution, server);
    for (size_t i = 0; i < disks.size(); ++i)
    {
        if (solution.getDiskColor(disks[i]) != color) // only one eq
        {
            size_t newServer = getRandomServer(server, *colored);
            moves.emplace_back(newServer, server, disks[i]);
        }
    }

    return moves;
}

template <class TSolution>
size_t RandomReduction<TSolution>::getRandomServer(const size_t tabuServer, const std::vector<bool>& colored)
{
    for (size_t i = 0; i < NUMBER_OF_RANDOM_ATTEMPTS; ++i)
    {
        size_t server = rand() % data.getNumberOfServers();

        if (server != tabuServer && !colored[server])
        {
            return server;
        }
    }

    assert(0);
    return 0;
}

template <class TSolution>
std::vector<size_t> RandomReduction<TSolution>::getAllDiskFromServer(const TSolution &solution, const size_t server)
{
    std::vector<size_t> disks;
    auto distribution = solution.getDistribution();

    for (size_t diskId = 0; diskId < distribution.size(); ++diskId)
    {
        if (distribution[diskId].serverId == server)
        {
            disks.push_back(diskId);
        }
    }

    return disks;
}
