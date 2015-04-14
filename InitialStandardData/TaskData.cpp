#include "TaskData.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

namespace
{
    void readInitialDistribution(std::vector<TaskData::Disk>* vector, size_t size, std::istream* stream)
	{
		for (size_t i = 0; i < size; ++i)
		{
            size_t value = 0;
			*stream >> value;
            assert(0 != value);
            vector->emplace_back(value - 1, TaskData::GREEN);
		}

        (*vector)[0].color = TaskData::RED;
        (*vector)[1].color = TaskData::RED;
        (*vector)[2].color = TaskData::GREEN;

        (*vector)[3].color = TaskData::GREEN;
        (*vector)[4].color = TaskData::YELLOW;
        (*vector)[5].color = TaskData::YELLOW;

        (*vector)[6].color = TaskData::GREEN;
        (*vector)[7].color = TaskData::GREEN;

        (*vector)[8].color = TaskData::GREEN;
        (*vector)[9].color = TaskData::GREEN;
	}
}

std::istream& operator>>(std::istream& stream, TaskData& taskData)
{
    // skip fisrt number of steps
    int t = 0;
    stream >> t;

    stream >> taskData.numberOfDisks >> taskData.numberOfServers >> taskData.numberOfResource >> taskData.numberOfTimes;

    // initialize matrixes
    taskData.thresholdCapacity.init(taskData.numberOfServers, taskData.numberOfResource);
    taskData.thresholdOverheadsServers.init(taskData.numberOfServers, taskData.numberOfResource);
    taskData.disksCapacity.init(taskData.numberOfDisks, taskData.numberOfResource, taskData.numberOfTimes);
    taskData.disksOverheadInsert.init(taskData.numberOfDisks, taskData.numberOfServers, taskData.numberOfResource);
    taskData.disksOverheadErase.init(taskData.numberOfDisks, taskData.numberOfServers, taskData.numberOfResource);

    // read matrixes
    stream >> taskData.thresholdCapacity; // c_{sr}
    stream >> taskData.thresholdOverheadsServers; // B^w_{sr}
    TwoDimensionalMatrix<double> temp(taskData.numberOfServers, taskData.numberOfResource);
    stream >> temp; // B^e_{sr}
    taskData.thresholdOverheadsServers += temp; // B^w_{sr} + B^e_{sr}
    stream >> taskData.disksCapacity; // c_{drt}
    stream >> taskData.disksOverheadInsert; // b^w_{dsr}
    stream >> taskData.disksOverheadErase; // b^e_{dsr}
    readInitialDistribution(&taskData.initialDistribution, taskData.numberOfDisks, &stream); // notice numbering from zero
	return stream;
}


size_t TaskData::getNumberOfDisks() const
{
    return numberOfDisks;
}

size_t TaskData::getNumberOfServers() const
{
    return numberOfServers;
}

size_t TaskData::getNumberOfResource() const
{
    return numberOfResource;
}

size_t TaskData::getNumberOfTimes() const
{
    return numberOfTimes;
}

std::vector<TaskData::Disk> TaskData::getInitialDistribution() const
{
    return initialDistribution;
}

TwoDimensionalMatrix<double> TaskData::getThresholdOverheads() const
{
    return thresholdOverheadsServers;
}

TaskData::TaskData()
    : numberOfDisks(0)
    , numberOfServers(0)
    , numberOfResource(0)
    , numberOfTimes(0)
{

}

double TaskData::getCapacity(size_t disk, size_t resource, size_t time) const
{
    return disksCapacity.get(disk, resource, time);
}

double TaskData::getThresholdCapacity(size_t server, size_t resource) const
{
    return thresholdCapacity.get(server, resource);
}

double TaskData::getDiskCosts(const TypeOperation typeOperation, size_t diskId, size_t server, size_t resource) const
{
    auto& overheadsDiskMatrix = (ERASE == typeOperation ? disksOverheadErase : disksOverheadInsert);
    return overheadsDiskMatrix.get(diskId, server, resource);
}

TaskData::Disk::Disk(size_t _id, Color _color)
    : serverId(_id)
    , color(_color)
{}
