#ifndef IASPIRATIONCRITERIA_HPP
#define IASPIRATIONCRITERIA_HPP

#include "Solution/ISolution.hpp"
#include "Move/IMove.hpp"

class IAspirationCriteria
{
public:
    virtual bool overrideTabu(const ISolution& solution, const IMove& move) const = 0;

    virtual void update(const ISolution& decision) = 0;

    virtual ~IAspirationCriteria() {}
};

#endif // IASPIRATIONCRITERIA_HPP
