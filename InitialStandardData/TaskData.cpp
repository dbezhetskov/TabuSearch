#include "TaskData.hpp"
#include <iostream>
#include <vector>

namespace
{
    template<class T>
	void readVector(std::vector<T>* vector, size_t size, std::istream* stream)
	{
		for (size_t i = 0; i < size; ++i)
		{
			T value;
			*stream >> value;
			vector->push_back(value);
		}
	}

	void readTwoDimensionalMatrix(
		std::vector<double>* vector,
		size_t numberOfRow,
		size_t numberOfColumn,
		std::istream* stream
		)
	{
		for (size_t i = 0; i < numberOfRow; ++i)
		{
			readVector(vector, numberOfColumn, stream);
		}
	}

    void readThreeDimensionalMatrix(
		std::vector<double>* vector,
		size_t numberOfRow,
		size_t numberOfColumn,
		size_t numberOfLayer,
		std::istream* stream
        )
	{
        for (size_t i = 0; i < numberOfLayer; ++i)
		{
			readTwoDimensionalMatrix(vector, numberOfRow, numberOfColumn, stream);
		}
	}
}

std::istream& operator>>(std::istream& stream, TaskData& taskData)
{
	stream >> taskData.numberOfDisks
		>> taskData.numberOfServers
		>> taskData.numberOfResource
		>> taskData.numberOfTimes;

	readTwoDimensionalMatrix(&taskData.thresholdCapacity, taskData.numberOfServers, taskData.numberOfResource, &stream);

	readTwoDimensionalMatrix(&taskData.thresholdOverheadsInsert, taskData.numberOfServers, taskData.numberOfResource, &stream);

	readTwoDimensionalMatrix(&taskData.thresholdOverheadsErase, taskData.numberOfServers, taskData.numberOfResource, &stream);

    readThreeDimensionalMatrix(&taskData.disksCapacity, taskData.numberOfDisks, taskData.numberOfResource, taskData.numberOfTimes, &stream);

	readThreeDimensionalMatrix(&taskData.disksOverheadInsert, taskData.numberOfServers, taskData.numberOfDisks, taskData.numberOfResource, &stream);

	readThreeDimensionalMatrix(&taskData.disksOverheadErase, taskData.numberOfServers, taskData.numberOfDisks, taskData.numberOfResource, &stream);

	readVector(&taskData.initialDistribution, taskData.numberOfDisks, &stream);

    // numbering from zero
    for (auto& disk : taskData.initialDistribution)
    {
        --disk;
    }

	return stream;
}

double TaskData::getCapacity(size_t disk, size_t resource, size_t time) const
{
    return disksCapacity[disk * numberOfResource * numberOfTimes + resource * numberOfTimes + time];
}

double TaskData::getThresholdCapacity(size_t server, size_t resource) const
{
    return thresholdCapacity[server * numberOfResource + resource];
}

double TaskData::getOverheadsDisk(const TypeOperation typeOperation, size_t server, size_t diskId, size_t resource) const
{
    auto pointer = (ERASE == typeOperation ? &disksOverheadErase : &disksOverheadInsert);
    return (*pointer)[server * numberOfDisks * numberOfResource + diskId * numberOfResource + resource];
}
