#include "MoveNeighborhood.hpp"
#include "Move/SimpleMove.hpp"

MoveNeighborhood::MoveNeighborhood(const TaskData& _data)
    : data(_data)
{}

std::vector<std::unique_ptr<IMove>> MoveNeighborhood::getMoves(const ISolution& solution) const
{
    std::vector< std::unique_ptr<IMove> > result;
    std::vector<size_t> distribution = solution.getDistribution();

    for (size_t i = 0; i < distribution.size(); ++i)
    {
        for (size_t j = 0; j < data.getNumberOfServers(); ++j)
        {
            if (distribution[i] != j)
            {
                std::unique_ptr<IMove> move(new SimpleMove(j, distribution[i], i));
                if (solution.moveIsCorrect(*move))
                {
                    result.emplace_back(std::move(move));
                }
            }
        }
    }

    return result;
}
