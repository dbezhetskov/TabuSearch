#ifndef SIMPLEMOVE_H
#define SIMPLEMOVE_H

#include "IMove.hpp"

class SimpleMove : public IMove
{
public:
    SimpleMove(size_t _destination, size_t _source, size_t _diskId);

    virtual std::vector<AtomMove> getAtomMove() const override;

private:
    size_t destination;
    size_t source;
    size_t diskId;
};

#endif // SIMPLEMOVE_H
