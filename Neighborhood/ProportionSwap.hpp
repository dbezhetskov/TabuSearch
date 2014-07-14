#ifndef PROPORTIONSWAP_HPP
#define PROPORTIONSWAP_HPP

#include "INeighborhood.hpp"
#include "InitialStandardData/TaskData.hpp"

class ProportionSwap : public INeighborhood
{
public:
    ProportionSwap(const TaskData& _data);

    // INeighborhood interface
    virtual std::vector<std::unique_ptr<IMove> > getMoves(const ISolution &solution) const override;

private:
    const TaskData& data;
};

#endif // PROPORTIONSWAP_HPP
