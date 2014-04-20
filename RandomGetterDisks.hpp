#ifndef RANDOMGETTERDISKS_HPP
#define RANDOMGETTERDISKS_HPP

#include "IGetterDisks.hpp"

class RandomGetterDisks : public IGetterDisks
{
public:
    virtual std::vector< std::pair< size_t, std::vector<size_t> > > getDisks(const ISolution& solution, const std::vector<size_t>& servers) const override;

private:
    std::vector<size_t> selectAllDiskForServer(const ISolution& solution, const size_t server) const;

    std::vector<size_t> eraseSomeDisks(const std::vector<size_t>& disks, const size_t numberOfErase) const;
};

#endif // RANDOMGETTERDISKS_HPP
