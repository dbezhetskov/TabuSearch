#include "VectorSolution.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace
{
    size_t hash(size_t first, size_t second, size_t third)
    {
        return std::hash<size_t>()(first) ^ std::hash<int>()(second) ^ std::hash<int>()(third);
    }
}

size_t VectorSolution::MoveHash::operator() (const IMove::AtomMove& atomMove) const
{
    return hash(atomMove.destination, atomMove.source, atomMove.diskId);
};

VectorSolution::VectorSolution(const TaskData& _data)
    : data(_data)
    , distribution(data.getInitialDistribution())
    , matrixCapacity(_data.getNumberOfServers(), _data.getNumberOfTimes(), _data.getNumberOfResource())
    , thresholdOverheadsServer(data.getThresholdOverheads())
    , objectiveValue(0)
{
    objectiveValue = fillObjectiveValueMatrix();
}

VectorSolution::VectorSolution(const VectorSolution& other)
    : data(other.data)
    , distribution(other.distribution)
    , matrixCapacity(other.matrixCapacity)
    , thresholdOverheadsServer(other.thresholdOverheadsServer)
    , objectiveValue(other.objectiveValue)
    , moveHistory(other.moveHistory)
{}

void VectorSolution::swap(VectorSolution &other)
{
    distribution.swap(other.distribution);
    std::swap(thresholdOverheadsServer, other.thresholdOverheadsServer);
    std::swap(objectiveValue, other.objectiveValue);
    std::swap(matrixCapacity, other.matrixCapacity);
    moveHistory.swap(other.moveHistory);
}

VectorSolution& VectorSolution::operator=(const VectorSolution& rhs)
{
    if (&rhs != this)
    {
        VectorSolution tmp(rhs);
        swap(tmp);
    }
    return *this;
}

bool VectorSolution::atomMoveIsCorrect( const IMove::AtomMove& atomMove,
                                        std::unordered_set<IMove::AtomMove, MoveHash>* const appliedMoves,
                                        TwoDimensionalMatrix<double>* const thresholdOverheadsServer ) const
{
    bool correct = true;
    auto previouslyMove = appliedMoves->find(atomMove);
    for (size_t resource = 0; resource < data.getNumberOfResource() && correct; ++resource)
    {
        if (appliedMoves->end() != previouslyMove)
        {
            // anti insert disk
            thresholdOverheadsServer->get(atomMove.source, resource)
                    += data.getDiskCosts(TaskData::INSERT, atomMove.diskId, atomMove.source, resource);

            // anti erase disk
            thresholdOverheadsServer->get(atomMove.destination, resource)
                    += data.getDiskCosts(TaskData::ERASE, atomMove.diskId, atomMove.destination, resource);
        }
        else
        {
            double sourceCapacity = thresholdOverheadsServer->get(atomMove.source, resource);
            double destinationCapacity = thresholdOverheadsServer->get(atomMove.destination, resource);
            double eraseOverhead = data.getDiskCosts(TaskData::ERASE, atomMove.diskId, atomMove.source, resource);
            double insertOverhead = data.getDiskCosts(TaskData::INSERT, atomMove.diskId, atomMove.destination, resource);

            if ( (sourceCapacity - eraseOverhead) < 0 || (destinationCapacity - insertOverhead) < 0)
            {
                correct = false;
                break;
            }

            // erase
            thresholdOverheadsServer->get(atomMove.source, resource) -= eraseOverhead;
            // insert
            thresholdOverheadsServer->get(atomMove.destination, resource) -= insertOverhead;
        }
    }

    if (!correct)
    {
        return false;
    }

    if (appliedMoves->end() != previouslyMove)
    {
        // delete, because we undo this move
        appliedMoves->erase(previouslyMove);
    }
    else
    {
        // save inverse atomMove
        appliedMoves->emplace(atomMove.source, atomMove.destination, atomMove.diskId);
    }

    return true;
}

bool VectorSolution::moveIsCorrect(const IMove &move) const
{
    bool correct = true;

    // work with copy, don't change current state
    auto appliedMovesCopy = appliedMoves;
    auto thresholdOverheadsServerCopy = thresholdOverheadsServer;

    for (auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);

        if (!atomMoveIsCorrect(atomMove, &appliedMovesCopy, &thresholdOverheadsServerCopy))
        {
            correct = false;
            break;
        }
    }

    return correct;
}

double VectorSolution::tryOnAtomMove(ThreeDimensionalMatrix<double> *matrixCapacity, double startObjectiveValue, const IMove::AtomMove& atomMove ) const
{
    assert(atomMove.source != atomMove.destination);
    assert(atomMove.source == distribution[atomMove.diskId]);
    assert(atomMove.source < data.getNumberOfServers() && atomMove.destination < data.getNumberOfServers());

    if (atomMove.source == atomMove.destination)
    {
        return startObjectiveValue;
    }

    for (size_t time = 0; time < data.getNumberOfTimes(); ++time)
    {
        for (size_t resource = 0; resource < data.getNumberOfResource(); ++resource)
        {
            // for destination
            if (matrixCapacity->get(atomMove.destination, time, resource) <= 0)
            {
                double newValueWithDisk = matrixCapacity->get(atomMove.destination, time, resource) + data.getCapacity(atomMove.diskId, resource, time);
                if (newValueWithDisk > 0)
                {
                    startObjectiveValue += newValueWithDisk;
                }
            }
            else
            {
                startObjectiveValue += data.getCapacity(atomMove.diskId, resource, time);
            }
            matrixCapacity->get(atomMove.destination, time, resource) += data.getCapacity(atomMove.diskId, resource, time);

            // for source
            if (matrixCapacity->get(atomMove.source, time, resource) > 0)
            {
                double newValueWithoutDisk = matrixCapacity->get(atomMove.source, time, resource) - data.getCapacity(atomMove.diskId, resource, time);
                if (newValueWithoutDisk <= 0)
                {
                    startObjectiveValue -= matrixCapacity->get(atomMove.source, time, resource);
                }
                else
                {
                    startObjectiveValue -= data.getCapacity(atomMove.diskId, resource, time);
                }
            }
            matrixCapacity->get(atomMove.source, time, resource) -= data.getCapacity(atomMove.diskId, resource, time);
        }
    }

    return startObjectiveValue;
}

double VectorSolution::tryOnMove(const IMove& move) const
{
    auto matrixCapacityCopy = matrixCapacity;
    double oldObjectiveValue = objectiveValue;

    for (auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);
        assert(atomMove.source < data.getNumberOfServers() && atomMove.destination < data.getNumberOfServers());

        double newObjectiveValue = tryOnAtomMove(&matrixCapacityCopy, oldObjectiveValue, atomMove);
        oldObjectiveValue = newObjectiveValue;
    }

    return oldObjectiveValue;
}

void VectorSolution::applyMove(const IMove& move)
{
    for (const auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);
        assert(atomMove.source < data.getNumberOfServers() && atomMove.destination < data.getNumberOfServers());

        moveHistory.push_back(atomMove);
        objectiveValue = tryOnAtomMove(&matrixCapacity, objectiveValue, atomMove);

        // overheads
        bool correct = atomMoveIsCorrect(atomMove, &appliedMoves, &thresholdOverheadsServer);
        assert(correct);

        moveDisk(atomMove.destination, atomMove.source, atomMove.diskId);
    }
}

double VectorSolution::getObjectiveValue() const
{
    return objectiveValue;
}

double VectorSolution::fillObjectiveValueMatrix()
{
    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        for (size_t time = 0; time < data.getNumberOfTimes(); ++time)
        {
            for (size_t resource = 0; resource < data.getNumberOfResource(); ++resource)
            {
                matrixCapacity.get(server, time, resource) = -1 * data.getThresholdCapacity(server, resource);
            }
        }
    }

    for (size_t diskId = 0; diskId < distribution.size(); ++diskId)
    {
        for (size_t time = 0; time < data.getNumberOfTimes(); ++time)
        {
            for (size_t resource = 0; resource < data.getNumberOfResource(); ++resource)
            {
                matrixCapacity.get(distribution[diskId], time, resource) += data.getCapacity(diskId, resource, time);
            }
        }
    }

    double objectiveValue = 0;
    for (size_t server = 0; server < data.getNumberOfServers(); ++server)
    {
        for (size_t time = 0; time < data.getNumberOfTimes(); ++time)
        {
            for (size_t resource = 0; resource < data.getNumberOfResource(); ++resource)
            {
                if (matrixCapacity.get(server, time, resource) > 0)
                {
                    objectiveValue += matrixCapacity.get(server, time, resource);
                }
            }
        }
    }

    return objectiveValue;
}

void VectorSolution::moveDisk(size_t destination, size_t, size_t diskId)
{
    distribution[diskId] = destination;
}

std::vector<size_t> VectorSolution::getDistribution() const
{
    return distribution;
}

std::vector<IMove::AtomMove> VectorSolution::getMoveHistory() const
{
    return moveHistory;
}

size_t VectorSolution::getServerForDisk(size_t diskId)
{
    assert(diskId < distribution.size());
    return distribution[diskId];
}

std::ostream& operator<<(std::ostream& outStream, const VectorSolution& solution)
{
    outStream << "Objective value = " << solution.getObjectiveValue() << '\n';

    // numbering from zero -> numbering from one
    auto distribution = solution.getDistribution();
    for (auto server : distribution)
    {
        outStream << server + 1 << ' ';
    }
    outStream << '\n';

    /*auto moveHistory = solution.getMoveHistory();
    for (auto atomMove : moveHistory)
    {
        outStream << atomMove << '\n';
    }*/

    return outStream;
}
