#ifndef SIMPLETABULIST_HPP
#define SIMPLETABULIST_HPP

#include "ITabuList.hpp"
#include <unordered_map>
#include <memory>

class HashSetTabuList : public ITabuList
{
public:
    HashSetTabuList(size_t _tenure);

    virtual bool isTabu(const IMove& move) const override;

    virtual void update(const IMove& decisionMove) override;

    virtual void clear() override;

    class AtomMoveHash
    {
    public:
        std::size_t operator()(const IMove::AtomMove& atomMove) const;
    };

private:
    const size_t tenure;

    std::unordered_map<IMove::AtomMove, size_t, AtomMoveHash> tabuList;
};

#endif // SIMPLETABULIST_HPP
