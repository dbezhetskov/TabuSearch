#include "VectorSolution.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace {

size_t hash(size_t first, size_t second, size_t third)
{
    return std::hash<size_t>()(first) ^ std::hash<int>()(second) ^ std::hash<int>()(third);
}

}

size_t VectorSolution::TripleHash::operator()(const VectorSolution::MatrixIdx &idx) const
{
    return hash(idx.server, idx.resource, idx.time);
}

size_t VectorSolution::MoveHash::operator ()(const IMove::AtomMove& atomMove) const
{
    return hash(atomMove.destination, atomMove.source, atomMove.diskId);
};

VectorSolution::VectorSolution(const std::shared_ptr<const TaskData> _data)
    : data(_data)
    , distribution(data->initialDistribution.cbegin(), data->initialDistribution.cend())
    , matrixCapacity(new std::vector<double>(_data->numberOfServers * _data->numberOfTimes * _data->numberOfResource))
    , thresholdOverheadsServerInsert(data->thresholdOverheadsInsert.cbegin(), data->thresholdOverheadsInsert.cend())
    , thresholdOverheadsServerErase(data->thresholdOverheadsErase.cbegin(), data->thresholdOverheadsErase.cend())
    , thresholdOverheadsServerInsertRedundancy(data->thresholdOverheadsInsert.cbegin(), data->thresholdOverheadsInsert.cend())
    , thresholdOverheadsServerEraseRedundancy(data->thresholdOverheadsErase.cbegin(), data->thresholdOverheadsErase.cend())
{
    // -1 means that no valid pair
    objectiveValue = fillObjectiveValueMatrix(std::vector<size_t>(), std::pair<int, size_t>(-1, 0));
}

VectorSolution::VectorSolution(const VectorSolution& other)
    : data(other.data)
    , distribution(other.distribution)
    , matrixCapacity(new std::vector<double> (other.matrixCapacity->begin(), other.matrixCapacity->end()))
    , thresholdOverheadsServerInsert(other.thresholdOverheadsServerInsert)
    , thresholdOverheadsServerErase(other.thresholdOverheadsServerErase)
    , thresholdOverheadsServerInsertRedundancy(other.thresholdOverheadsServerInsertRedundancy)
    , thresholdOverheadsServerEraseRedundancy(other.thresholdOverheadsServerEraseRedundancy)
    , objectiveValue(other.objectiveValue)
{
}

void VectorSolution::swap(VectorSolution &other)
{
    std::swap(data, other.data);
    distribution.swap(other.distribution);
    thresholdOverheadsServerInsert.swap(other.thresholdOverheadsServerInsert);
    thresholdOverheadsServerErase.swap(other.thresholdOverheadsServerErase);
    thresholdOverheadsServerInsertRedundancy.swap(other.thresholdOverheadsServerInsertRedundancy);
    thresholdOverheadsServerEraseRedundancy.swap(other.thresholdOverheadsServerEraseRedundancy);
    std::swap(objectiveValue, other.objectiveValue);
    matrixCapacity.swap(other.matrixCapacity);
}

VectorSolution& VectorSolution::operator=(VectorSolution& rhs)
{
    if (&rhs != this)
    {
        VectorSolution tmp(rhs);
        swap(tmp);
    }
    return *this;
}

double& VectorSolution::getElemCapacityMatrix(VectorSolution::TMap* distinct, size_t server, size_t time, size_t resource) const
{
    auto it = distinct->find(MatrixIdx(server, time, resource));
    if (distinct->end() != it)
    {
        return it->second;
    }
    return (*matrixCapacity)[server * data->numberOfTimes * data->numberOfResource + time * data->numberOfResource + resource];
}

bool VectorSolution::recalcOverheadsRedundancyByResource(const IMove::AtomMove& atomMove, const size_t resource) const
{
    bool correct = true;

    // erase disk
    getOverheadsRedundancy(TaskData::ERASE, atomMove.source, resource) -=
            data->getOverheadsDisk(TaskData::ERASE, atomMove.source, atomMove.diskId, resource);
    if (getOverheadsRedundancy(TaskData::ERASE, atomMove.source, resource) < 0)
    {
        correct = false;
    }

    // insert disk
    getOverheadsRedundancy(TaskData::INSERT, atomMove.destination, resource) -=
            data->getOverheadsDisk(TaskData::INSERT, atomMove.destination, atomMove.diskId, resource);
    if (getOverheadsRedundancy(TaskData::INSERT, atomMove.destination, resource) < 0)
    {
        correct = false;
    }

    return correct;
}

bool VectorSolution::recalcOverheadsRedundancyByResourceInverse(const IMove::AtomMove& atomMove, const size_t resource) const
{
    // erase disk
    getOverheadsRedundancy(TaskData::ERASE, atomMove.destination, resource) +=
            data->getOverheadsDisk(TaskData::ERASE, atomMove.destination, atomMove.diskId, resource);

    // insert disk
    getOverheadsRedundancy(TaskData::INSERT, atomMove.source, resource) +=
            data->getOverheadsDisk(TaskData::INSERT, atomMove.source, atomMove.diskId, resource);

    return true;
}

bool VectorSolution::atomMoveIsCorrect(const IMove::AtomMove& atomMove) const
{
    bool correct = true;
    bool movePreviouslyMet = moves.end() != moves.find(atomMove);
    for (size_t resource = 0; resource < data->numberOfResource && correct; ++resource)
    {
        correct = movePreviouslyMet ? recalcOverheadsRedundancyByResourceInverse(atomMove, resource)
                                    : recalcOverheadsRedundancyByResource(atomMove, resource);
    }

    return correct;
}

bool VectorSolution::moveIsCorrect(const IMove &move) const
{
    bool correct = true;

    for (auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);

        if (!atomMoveIsCorrect(atomMove))
        {
            correct = false;
            break;
        }
    }

    // return to a consistent state
    std::copy(thresholdOverheadsServerInsert.cbegin(), thresholdOverheadsServerInsert.cend(), thresholdOverheadsServerInsertRedundancy.begin());
    std::copy(thresholdOverheadsServerErase.cbegin(), thresholdOverheadsServerErase.cend(), thresholdOverheadsServerEraseRedundancy.begin());

    return correct;
}

std::pair<VectorSolution::Changes, double> VectorSolution::tryOnAtomeMove(
        VectorSolution::TMap* distinct
        , double startObjectiveValue
        , const IMove::AtomMove& atomMove
        ) const
{
    assert(atomMove.source != atomMove.destination);
    assert(atomMove.source == distribution[atomMove.diskId]);

    Changes changes;
    MatrixElem matrixElem;

    if (atomMove.source == atomMove.destination)
    {
        return std::pair<VectorSolution::Changes, double>(changes, startObjectiveValue);
    }

    for (size_t time = 0; time < data->numberOfTimes; ++time)
    {
        for (size_t resource = 0; resource < data->numberOfResource; ++resource)
        {
            // for destination
            if (getElemCapacityMatrix(distinct, atomMove.destination, time, resource) <= 0)
            {
                double newValueWithDisk = getElemCapacityMatrix(distinct, atomMove.destination, time, resource)
                        + data->getCapacity(atomMove.diskId, resource, time);
                if (newValueWithDisk > 0)
                {
                    startObjectiveValue += newValueWithDisk;
                }
            }
            else
            {
                startObjectiveValue += data->getCapacity(atomMove.diskId, resource, time);
            }

            matrixElem.first = MatrixIdx(atomMove.destination, time, resource);
            matrixElem.second = data->getCapacity(atomMove.diskId, resource, time) + getElemCapacityMatrix(distinct, atomMove.destination, time, resource);
            changes.push_back(matrixElem);

            // for source
            if (getElemCapacityMatrix(distinct, atomMove.source, time, resource) > 0)
            {
                double newValueWithoutDisk = getElemCapacityMatrix(distinct, atomMove.source, time, resource)
                        - data->getCapacity(atomMove.diskId, resource, time);
                if (newValueWithoutDisk <= 0)
                {
                    startObjectiveValue -= getElemCapacityMatrix(distinct, atomMove.source, time, resource);
                }
                else
                {
                    startObjectiveValue -= data->getCapacity(atomMove.diskId, resource, time);
                }
            }

            matrixElem.first = MatrixIdx(atomMove.source, time, resource);
            matrixElem.second = getElemCapacityMatrix(distinct, atomMove.source, time, resource) - data->getCapacity(atomMove.diskId, resource, time);
            changes.push_back(matrixElem);
        }
    }

    return std::pair<VectorSolution::Changes, double>(changes, startObjectiveValue);
}

double VectorSolution::tryOnMove(const IMove& move) const
{
    TMap distinct;
    double previousObjectiveValue = objectiveValue;

    for (auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);

        auto result = tryOnAtomeMove(&distinct, previousObjectiveValue, atomMove);
        Changes changes = result.first;
        previousObjectiveValue = result.second;
        distinct.insert(changes.begin(), changes.end());
    }

    return previousObjectiveValue;
}

void VectorSolution::recalcOverheads(const IMove::AtomMove& atomMove, const size_t resource)
{
    // erase disk
    getOverheads(TaskData::ERASE, atomMove.source, resource) -=
            data->getOverheadsDisk(TaskData::ERASE, atomMove.source, atomMove.diskId, resource);
    getOverheadsRedundancy(TaskData::ERASE, atomMove.source, resource) = getOverheads(TaskData::ERASE, atomMove.source, resource);

    // insert disk
    getOverheads(TaskData::INSERT, atomMove.destination, resource) -=
            data->getOverheadsDisk(TaskData::INSERT, atomMove.destination, atomMove.diskId, resource);
    getOverheadsRedundancy(TaskData::INSERT, atomMove.destination, resource) = getOverheads(TaskData::INSERT, atomMove.destination, resource);
}

void VectorSolution::recalcOverheadsInverse(const IMove::AtomMove& atomMove, const size_t resource)
{
    // erase disk
    getOverheads(TaskData::ERASE, atomMove.destination, resource) +=
            data->getOverheadsDisk(TaskData::ERASE, atomMove.destination, atomMove.diskId, resource);
    getOverheadsRedundancy(TaskData::ERASE, atomMove.destination, resource) = getOverheads(TaskData::ERASE, atomMove.destination, resource);

    // insert disk
    getOverheads(TaskData::INSERT, atomMove.source, resource) +=
            data->getOverheadsDisk(TaskData::INSERT, atomMove.source, atomMove.diskId, resource);
    getOverheadsRedundancy(TaskData::INSERT, atomMove.source, resource) = getOverheads(TaskData::INSERT, atomMove.source, resource);
}

void VectorSolution::applyMove(const IMove& move)
{
    TMap clearMap;

    for (const auto atomMove : move.getAtomMove())
    {
        assert(atomMove.source != atomMove.destination);
        assert(atomMove.source == distribution[atomMove.diskId]);

        auto result = tryOnAtomeMove(&clearMap, objectiveValue, atomMove);
        for (auto change : result.first)
        {
            MatrixIdx idx = change.first;
            getElemCapacityMatrix(idx.server, idx.time, idx.resource) = change.second;
        }
        objectiveValue = result.second;
        moveDisk(atomMove.destination, atomMove.source, atomMove.diskId);

        // overheads
        auto iter = moves.find(atomMove);
        for (size_t resource = 0; resource < data->numberOfResource; ++resource)
        {
            if (moves.end() != iter)
            {
                recalcOverheadsInverse(atomMove, resource);
            }
            else
            {
                recalcOverheads(atomMove, resource);
            }

            assert(getOverheads(TaskData::ERASE, atomMove.destination, resource) >= 0);
            assert(getOverheads(TaskData::INSERT, atomMove.destination, resource) >= 0);

            assert(getOverheads(TaskData::ERASE, atomMove.source, resource) >= 0);
            assert(getOverheads(TaskData::INSERT, atomMove.source, resource) >= 0);
        }

        if (moves.end() != iter)
        {
            moves.erase(iter);
        }
        else
        {
            // save inverse atomMove
            moves.emplace(atomMove.source, atomMove.destination, atomMove.diskId);
        }
    }
}

double VectorSolution::getObjectiveValue() const
{
    return objectiveValue;
}

double VectorSolution::fillObjectiveValueMatrix(
        const std::vector<size_t>& removedDisks,
        const std::pair<int, size_t> insertedDisk
) const
{
    for (size_t server = 0; server < data->numberOfServers; ++server)
    {
        for (size_t time = 0; time < data->numberOfTimes; ++time)
        {
            for (size_t resource = 0; resource < data->numberOfResource; ++resource)
            {
                getElemCapacityMatrix(server, time, resource) = -1 * data->getThresholdCapacity(server, resource);
            }
        }
    }

    std::vector<bool> removed(data->numberOfDisks, false);
    for (auto diskId : removedDisks)
    {
        removed[diskId] = true;
    }

    for (size_t diskId = 0; diskId < distribution.size(); ++diskId)
    {
        if (removed[diskId] && insertedDisk.second != diskId)
        {
            continue;
        }

        size_t server = distribution[diskId];
        if (-1 != insertedDisk.first && insertedDisk.second == diskId)
        {
            server = insertedDisk.first;
        }

        for (size_t time = 0; time < data->numberOfTimes; ++time)
        {
            for (size_t resource = 0; resource < data->numberOfResource; ++resource)
            {
                getElemCapacityMatrix(server, time, resource) += data->getCapacity(diskId, resource, time);
            }
        }
    }

    double _objectiveValue = 0;
    for (size_t server = 0; server < data->numberOfServers; ++server)
    {
        for (size_t time = 0; time < data->numberOfTimes; ++time)
        {
            for (size_t resource = 0; resource < data->numberOfResource; ++resource)
            {
                if (getElemCapacityMatrix(server, time, resource) > 0)
                {
                    _objectiveValue += getElemCapacityMatrix(server, time, resource);
                }
            }
        }
    }

    return _objectiveValue;
}

double VectorSolution::tryOnInsertDisk(const size_t server, const size_t disk, const std::vector<size_t>& removedDisks) const
{
    // save old value
    std::unique_ptr< std::vector<double> > copyMatrixCapacity(new std::vector<double>(matrixCapacity->cbegin(), matrixCapacity->cend()));

    double objectiveValueWithoutRemovedDisks = fillObjectiveValueMatrix(removedDisks, std::pair<int, size_t>(server, disk));

    // restore
    matrixCapacity = std::move(copyMatrixCapacity);

    return objectiveValueWithoutRemovedDisks;
}

void VectorSolution::moveDisk(size_t destination, size_t source, size_t diskId)
{
    distribution[diskId] = destination;
}

std::vector<size_t> VectorSolution::getDistribution() const
{
    return distribution;
}

