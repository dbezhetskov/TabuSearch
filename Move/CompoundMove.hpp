#ifndef COMPOUNDMOVE_HPP
#define COMPOUNDMOVE_HPP

#include "IMove.hpp"

class CompoundMove : public IMove
{
public:
    CompoundMove(std::vector<IMove::AtomMove>&& _moves);

    virtual std::vector<IMove::AtomMove> getAtomMove() const override;

private:
    std::vector<IMove::AtomMove> moves;
};

#endif // COMPOUNDMOVE_HPP
