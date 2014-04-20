#ifndef _TASK_DATA_H_
#define _TASK_DATA_H_

#include <vector>
#include <iostream>

struct TaskData
{
public:
    enum TypeOperation
    {
        ERASE,
        INSERT
    };

public:
	size_t numberOfDisks;
	size_t numberOfServers;
	size_t numberOfResource;
	size_t numberOfTimes;

	std::vector<double> thresholdCapacity;
	std::vector<double> thresholdOverheadsInsert;
	std::vector<double> thresholdOverheadsErase;
	
	std::vector<double> disksCapacity;
	std::vector<double> disksOverheadInsert;
	std::vector<double> disksOverheadErase;

    std::vector<size_t> initialDistribution;

public:
	double getCapacity(size_t disk, size_t resource, size_t time) const;

	double getThresholdCapacity(size_t server, size_t resource) const;

    double getOverheadsDisk(const TypeOperation typeOperation, size_t server, size_t diskId, size_t resource) const;

private:
	friend std::istream& operator>>(std::istream& stream, TaskData& taskData);
};

#endif // !_TASK_DATA_H_
