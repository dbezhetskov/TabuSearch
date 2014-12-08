#ifndef _I_SOLUTION_H_
#define _I_SOLUTION_H_

#include <cstddef>
#include <vector>
#include <utility>
#include "Move/IMove.hpp"

class ISolution
{
public:

    /**
     * @brief get objective value
     * @return objective value this solution
     */
    virtual double getObjectiveValue() const = 0;

    /**
     * @brief try on move
     * @param move
     * @return objective value received after applying of the move
     *         without modifying this solution
     */
    virtual double tryOnMove(const IMove& move) const = 0;

    /**
     * @brief applying move in this solution
     * @param move
     */
    virtual void applyMove(const IMove& move) = 0;

    /**
     * @brief move is correct?
     * @param move
     * @return true if applying move don't break restrictions
     *         false otherwise
     */
    virtual bool moveIsCorrect(const IMove& move) const = 0;

    /**
     * @brief get distribution
     * @return vector which specifies for each disk server
     */
    virtual std::vector<size_t> getDistribution() const = 0;

    /**
     * @brief get move history
     * @return all moves applied to this solution
     */
    virtual std::vector<IMove::AtomMove> getMoveHistory() const = 0;

    /**
     * @brief operator <
     * @param right
     * @return true if this solution objective value less then right solution objective value
     *         false otherwise
     */
    bool operator< (const ISolution& right) const;

	virtual ~ISolution() {}
};

inline bool ISolution::operator< (const ISolution& right) const
{
    return getObjectiveValue() < right.getObjectiveValue();
}

#endif // !_I_SOLUTION_H_
