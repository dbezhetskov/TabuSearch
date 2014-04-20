#ifndef RANDOMONEDISKBYSERVERGETTER_HPP
#define RANDOMONEDISKBYSERVERGETTER_HPP

#include "IGetterDisks.hpp"

class RandomOneDiskByServerGetter : public IGetterDisks
{
public:
    virtual std::vector< std::pair< size_t, std::vector<size_t> > > getDisks(const ISolution& solution, const std::vector<size_t>& servers) const override;

private:
    std::vector<size_t> selectAllDiskForServer(const ISolution &solution, const size_t server) const;
};

#endif // RANDOMONEDISKBYSERVERGETTER_HPP
