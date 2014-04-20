#ifndef _I_SOLUTION_H_
#define _I_SOLUTION_H_

#include <cstddef>
#include <vector>
#include <utility>
#include "Move/IMove.hpp"

class ISolution
{
public:
    virtual double getObjectiveValue() const = 0;

    virtual double tryOnMove(const IMove& move) const = 0;

    virtual double tryOnInsertDisk(const size_t server, const size_t disk, const std::vector<size_t>& removedDisks) const = 0;

    virtual void applyMove(const IMove& move) = 0;

    virtual bool moveIsCorrect(const IMove& move) const = 0;

    virtual std::vector<size_t> getDistribution() const = 0;

	virtual ~ISolution() {}
};

#endif // !_I_SOLUTION_H_
