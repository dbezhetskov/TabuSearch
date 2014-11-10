#ifndef UNIONNEIGHBORHOODINL_HPP
#define UNIONNEIGHBORHOODINL_HPP

#include "UnionNeighborhood.hpp"

template< class TFirstNeighborhood, class TSecondNeighborhood >
UnionNeighborhood< TFirstNeighborhood, TSecondNeighborhood >::UnionNeighborhood(const TFirstNeighborhood& first, const TSecondNeighborhood& second)
    : first_neighborhood(first)
    , second_neighborhood(second)
{}

template< class TFirstNeighborhood, class TSecondNeighborhood >
std::vector< std::unique_ptr<IMove> > UnionNeighborhood< TFirstNeighborhood, TSecondNeighborhood >::getMoves(const ISolution& solution) const
{
    std::vector< std::unique_ptr<IMove> > all_moves;

    auto first_moves = first_neighborhood.getMoves(solution);
    all_moves.insert(all_moves.end(), std::make_move_iterator(first_moves.begin()), std::make_move_iterator(first_moves.end()));

    auto second_moves = second_neighborhood.getMoves(solution);
    all_moves.insert(all_moves.end(), std::make_move_iterator(second_moves.begin()), std::make_move_iterator(second_moves.end()));

    return all_moves;
}

#endif // UNIONNEIGHBORHOODINL_HPP
