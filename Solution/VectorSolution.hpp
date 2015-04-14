#ifndef _VECTOR_SOLUTION_H_
#define _VECTOR_SOLUTION_H_

#include "ISolution.hpp"
#include "InitialStandardData/TaskData.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "Utils/ThreeDimensionalMatrix.hpp"
#include "Utils/TwoDimensionalMatrix.hpp"

class VectorSolution : public ISolution
{
public:
    VectorSolution(const TaskData& _data, const double _overheadsCoefficient);

    VectorSolution(const VectorSolution& other);

    VectorSolution& operator=(const VectorSolution& rhs);

    void swap(VectorSolution& other);

    virtual double tryOnMove(const IMove& move) const override;

    virtual void applyMove(const IMove& move) override;

    virtual bool moveIsCorrect(const IMove& move) const override;

    virtual double getObjectiveValue() const override;

    virtual double getOverheads() const override;

    virtual std::vector<TaskData::Disk> getDistribution() const override;

    virtual std::vector<IMove::AtomMove> getMoveHistory() const override;

    virtual void setOverheadsCoefficient(const double value) override;

    virtual double getOverheadsCoefficient() override;

    size_t getServerForDisk(size_t diskId) const;

    TaskData::Color getServerColor(const size_t server) const;

    TaskData::Color getDiskColor(const size_t disk) const;

    void paintServers();

private:

    struct MoveHash
    {
        std::size_t operator()(const IMove::AtomMove& atomMove) const;
    };

    typedef TaskData::Color Color;

private:
    double fillObjectiveValueMatrix();

    void moveDisk(size_t destination, size_t source, size_t diskId);

    double tryOnAtomMove(ThreeDimensionalMatrix<double>* const matrixCapacity, double startObjectiveValue, const IMove::AtomMove& atomMove) const;

    double getOverheads(const IMove::AtomMove& atomMove,
                         std::unordered_set<IMove::AtomMove, MoveHash>* const appliedMoves,
                         TwoDimensionalMatrix<double>* const thresholdOverheadsServer, double startOverheads) const;

    friend std::ostream& operator<<(std::ostream& outStream, const VectorSolution& solution);

private:
    const TaskData& data;

    std::vector<TaskData::Disk> distribution;

    ThreeDimensionalMatrix<double> matrixCapacity;

    TwoDimensionalMatrix<double> thresholdOverheadsServer;

    double objectiveValue;

    double overheads;

    std::unordered_set<IMove::AtomMove, MoveHash> appliedMoves;

    std::vector<IMove::AtomMove> moveHistory;

    double overheadsCoefficient;

    std::vector<TaskData::Color> serversColor;
};

#endif // !_VECTOR_SOLUTION_H_
