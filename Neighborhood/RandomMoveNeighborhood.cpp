#include "RandomMoveNeighborhood.hpp"
#include "Move/SimpleMove.hpp"
#include <ctime>

RandomMoveNeighborhood::RandomMoveNeighborhood(const TaskData &_data, size_t _numberOfAttempts)
    : data(_data)
    , numberOfAttempts(_numberOfAttempts)
{
}

void RandomMoveNeighborhood::setSize(const size_t _size)
{
    numberOfAttempts = _size;
}

std::vector<std::unique_ptr<IMove> > RandomMoveNeighborhood::getMoves(const ISolution& solution) const
{
    std::vector< std::unique_ptr<IMove> > result;
    auto distribution = solution.getDistribution();

    srand(time(0));
    auto moves = getSomeMoves(solution);
    for (auto pair : moves)
    {
        std::unique_ptr<IMove> move(new SimpleMove(pair.second, distribution[pair.first], pair.first));
        if (solution.moveIsCorrect(*move))
        {
            result.emplace_back(std::move(move));
        }
    }

    return result;
}

std::vector< std::pair<size_t, size_t> > RandomMoveNeighborhood::getSomeMoves(const ISolution& solution) const
{
    std::vector< std::pair<size_t, size_t> > result;
    auto distribution = solution.getDistribution();

    for (size_t i = 0; i < numberOfAttempts; ++i)
    {
        size_t diskId = rand() % distribution.size();
        size_t destination = rand() % data.getNumberOfServers();
        if (distribution[diskId] != destination)
        {
            result.emplace_back(diskId, destination);
        }
    }

    return result;
}
