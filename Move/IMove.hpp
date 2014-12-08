#ifndef _I_MOVE_H_
#define _I_MOVE_H_

#include <cstddef>
#include <vector>
#include <iostream>

class IMove
{
public:
    struct AtomMove
    {
        AtomMove(size_t _destination, size_t _source, size_t _diskId)
        {
            destination = _destination;
            source = _source;
            diskId = _diskId;
        }

        bool operator==(const AtomMove& rhs) const
        {
            return destination == rhs.destination
                    && source == rhs.source
                    && diskId == rhs.diskId;
        }

        size_t destination;
        size_t source;
        size_t diskId;

    private:
        friend std::ostream& operator<<(std::ostream& outStream, const AtomMove& atomMove)
        {
            return (outStream << "move " << atomMove.destination << ' ' << atomMove.source << ' ' << atomMove.diskId);
        }
    };

public:

    /**
     * @brief get atom moves: all moves are presented in the sequence move form
     * @return sequence of moves
     */
    virtual std::vector<AtomMove> getAtomMove() const = 0;

    virtual ~IMove() {}
};

#endif // !_I_MOVE_H_
