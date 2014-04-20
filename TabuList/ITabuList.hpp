#ifndef ITABULIST_HPP
#define ITABULIST_HPP

#include "Move/IMove.hpp"
#include <memory>

class ITabuList
{
public:
    virtual bool isTabu(const IMove& move) const = 0;

    virtual void update(const IMove& decisionMove) = 0;

    virtual void clear() = 0;

    virtual ~ITabuList() {}
};

#endif // ITABULIST_HPP
