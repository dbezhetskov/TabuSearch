#ifndef _TASK_DATA_H_
#define _TASK_DATA_H_

#include "Utils/TwoDimensionalMatrix.hpp"
#include "Utils/ThreeDimensionalMatrix.hpp"
#include <iostream>
#include <utility>

struct TaskData
{
public:
    enum TypeOperation
    {
        ERASE,
        INSERT
    };
public:
	double getCapacity(size_t disk, size_t resource, size_t time) const;

	double getThresholdCapacity(size_t server, size_t resource) const;

    double getDiskCosts(const TypeOperation typeOperation, size_t diskId, size_t server, size_t resource) const;

    size_t getNumberOfDisks() const;

    size_t getNumberOfServers() const;

    size_t getNumberOfResource() const;

    size_t getNumberOfTimes() const;

    std::vector<size_t> getInitialDistribution() const;

    TwoDimensionalMatrix<double> getThresholdOverheads() const;

private:
    size_t numberOfDisks;
    size_t numberOfServers;
    size_t numberOfResource;
    size_t numberOfTimes;

    TwoDimensionalMatrix<double> thresholdCapacity;
    TwoDimensionalMatrix<double> thresholdOverheadsServers;

    ThreeDimensionalMatrix<double> disksCapacity;
    ThreeDimensionalMatrix<double> disksOverheadInsert;
    ThreeDimensionalMatrix<double> disksOverheadErase;

    std::vector<size_t> initialDistribution;

private:
    friend std::istream& operator>>(std::istream& stream, TaskData& taskData);
};

#endif // !_TASK_DATA_H_
