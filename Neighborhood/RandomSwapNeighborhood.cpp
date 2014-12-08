#include "RandomSwapNeighborhood.hpp"
#include "Move/CompoundMove.hpp"
#include <ctime>

RandomSwapNeighborhood::RandomSwapNeighborhood(const TaskData &_data, size_t _numberOfAttempts)
    : data(_data)
    , numberOfAttempts(_numberOfAttempts)
{
}

void RandomSwapNeighborhood::setSize(const size_t _size)
{
    numberOfAttempts = _size;
}

std::vector<std::unique_ptr<IMove> > RandomSwapNeighborhood::getMoves(const ISolution& solution) const
{
    auto distribution = solution.getDistribution();
    std::vector< std::unique_ptr<IMove> > result;

    auto swapPairs = getSomePairDisk(solution);

    for (auto pair : swapPairs)
    {
        std::vector<IMove::AtomMove> AtomMoves;
        AtomMoves.emplace_back(distribution[pair.second], distribution[pair.first], pair.first);
        AtomMoves.emplace_back(distribution[pair.first], distribution[pair.second], pair.second);
        std::unique_ptr<IMove> move(new CompoundMove(std::move(AtomMoves)));
        if (solution.moveIsCorrect(*move))
        {
            result.emplace_back(std::move(move));
        }
    }

    return result;
}

std::vector< std::pair<size_t, size_t> > RandomSwapNeighborhood::getSomePairDisk(const ISolution& solution) const
{
    std::vector< std::pair<size_t, size_t> > result;
    std::vector<bool> used(data.getNumberOfDisks() * data.getNumberOfDisks(), false);
    auto distribution = solution.getDistribution();

    srand(time(0));
    for (size_t i = 0; i < numberOfAttempts; ++i)
    {
        size_t diskId1 = 0;
        size_t diskId2 = 0;

        do
        {
            diskId1 = rand() % data.getNumberOfDisks();
            diskId2 = rand() % data.getNumberOfDisks();
        }
        while (distribution[diskId1] == distribution[diskId2] && !used[diskId1 * data.getNumberOfDisks() + diskId2]);

        used[diskId1 * data.getNumberOfDisks() + diskId2] = true;

        result.emplace_back(diskId1, diskId2);
    }

    return result;
}
