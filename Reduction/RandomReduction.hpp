#ifndef RANDOMREDUCTION_H
#define RANDOMREDUCTION_H

#include "IReduction.hpp"
#include "InitialStandardData/TaskData.hpp"
#include "Solution/VectorSolution.hpp"
#include "Move/SimpleMove.hpp"

template <class TSolution = VectorSolution>
class RandomReduction : public IReduction<TSolution>
{
public:
    RandomReduction(const TaskData& _data);

    virtual TSolution reduce(const TSolution& solution) override;

private:
    typedef TaskData::Color Color;

private:
    std::vector<SimpleMove> unloadDisks(const TSolution &solution);

    std::vector<SimpleMove> scatterDiskFromServer(const Color color, const size_t server, std::vector<bool>* colored, const TSolution &solution);

    std::vector<SimpleMove> unloadDiskFromServer(const Color color, const size_t server, std::vector<bool> *colored, const TSolution &solution);

    size_t getRandomServer(const size_t tabuServer, const std::vector<bool> &colored);

    std::vector<size_t> getAllDiskFromServer(const TSolution& solution, const size_t server);

private:
    static const size_t NUMBER_OF_RANDOM_ATTEMPTS = 100;

    const TaskData& data;
};

#endif // RANDOMREDUCTION_H
