#ifndef BESTEVERASPIRATIONCRITERIA_HPP
#define BESTEVERASPIRATIONCRITERIA_HPP

#include "IAspirationCriteria.hpp"

class BestEverAspirationCriteria : public IAspirationCriteria
{
public:
    BestEverAspirationCriteria(double _bestObjectiveValue);

    virtual bool overrideTabu(const ISolution& solution, const IMove& move) const override;

    virtual void update(const ISolution& decision) override;

private:
    double bestObjectiveValue;
};

#endif // BESTEVERASPIRATIONCRITERIA_HPP
