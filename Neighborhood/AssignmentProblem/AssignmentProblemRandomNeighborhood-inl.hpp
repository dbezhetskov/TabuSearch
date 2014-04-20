#include "AssignmentProblemRandomNeighborhood.hpp"
#include "Move/SimpleMove.hpp"
#include "Move/CompoundMove.hpp"

#include <vector>
#include <limits>
#include <random>
#include <unordered_set>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <iostream>

namespace
{
    // http://e-maxx.ru/algo/assignment_hungary

    std::vector<int> assignmentProblem(const std::vector< std::vector<int> >& a)
    {
        int n = a.size() - 1;
        int m = a[0].size() - 1;
        const int INF = std::numeric_limits<int>::max();

        std::vector<int> u (n+1), v (m+1), p (m+1), way (m+1);
        for (int i=1; i<=n; ++i) {
            p[0] = i;
            int j0 = 0;
            std::vector<int> minv (m+1, INF);
            std::vector<char> used (m+1, false);
            do {
                used[j0] = true;
                int i0 = p[j0],  delta = INF,  j1;
                for (int j=1; j<=m; ++j)
                    if (!used[j]) {
                        int cur = a[i0][j]-u[i0]-v[j];
                        if (cur < minv[j])
                            minv[j] = cur,  way[j] = j0;
                        if (minv[j] < delta)
                            delta = minv[j],  j1 = j;
                    }
                for (int j=0; j<=m; ++j)
                    if (used[j])
                        u[p[j]] += delta,  v[j] -= delta;
                    else
                        minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        std::vector<int> ans (n+1);
        for (int j = 1; j <= m; ++j)
            ans[p[j]] = j;

        return ans;
    }
}

template<class GetterServers, class GetterDisksFromServers>
AssignmentProblemRandomNeighborhood<GetterServers, GetterDisksFromServers>::AssignmentProblemRandomNeighborhood(const TaskData& _data, const size_t _minNumberOfServers)
    : data(_data)
    , minNumberOfServers(_minNumberOfServers)
{
    assert(minNumberOfServers >= 2);
}

template<class GetterServers, class GetterDisksFromServers>
std::vector< std::vector<int> > AssignmentProblemRandomNeighborhood<GetterServers, GetterDisksFromServers>::calcCost
(
        const std::vector< std::pair< size_t, std::vector<size_t> > >& serversWithDisks,
        const ISolution& solution,
        const std::vector<size_t>& disks
) const
{
    size_t numberOfRows = serversWithDisks.size();
    size_t numberOfColumns = disks.size();

    // the requirement to implement the algorithm with e-max
    assert(numberOfRows <= numberOfColumns);

    std::vector< std::vector<int> > matrixCosts;
    matrixCosts.reserve((numberOfRows + 1) * (numberOfColumns + 1));    // numbering from one, zero - fictive

    // add fictive row
    matrixCosts.push_back(std::vector<int>(numberOfColumns + 1, 0));

    //double previousObjectiveValue = solution.tryOnEraseDisks(disks);
    for (size_t i = 0; i < serversWithDisks.size(); ++i)
    {
        std::vector<int> row;
        row.push_back(0);   // numbering from one, 0 element is fictive

        for (auto diskId : disks)
        {
            row.push_back(solution.tryOnInsertDisk(serversWithDisks[i].first, diskId, disks));
        }

        matrixCosts.emplace_back(std::move(row));
    }

    return matrixCosts;
}

template<class GetterServers, class GetterDisksFromServers>
std::vector<std::unique_ptr<IMove> > AssignmentProblemRandomNeighborhood<GetterServers, GetterDisksFromServers>::getMoves(const ISolution& solution) const
{
    size_t numberOfServers = 0;

    {
        assert(minNumberOfServers < data.numberOfDisks);
        std::uniform_int_distribution<int> distribution(minNumberOfServers, data.numberOfServers);
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        numberOfServers = distribution(generator);
    }

    auto servers = getServers(numberOfServers, data.numberOfServers);
    auto serverWithDisks = getDisks(solution, servers);
    std::vector<size_t> disks;
    for (auto serverDisk : serverWithDisks)
    {
        disks.insert(disks.end(), serverDisk.second.cbegin(), serverDisk.second.cend());
    }

    std::vector<int> answer = assignmentProblem(calcCost(serverWithDisks, solution, disks));

    std::vector<IMove::AtomMove> atomMoves;
    std::vector<size_t> distribution = solution.getDistribution();
    for (size_t i = 1; i < answer.size(); ++i)
    {
        size_t diskId = disks[answer[i] - 1];
        size_t server = serverWithDisks[i - 1].first;

        assert(server < data.numberOfServers);
        assert(diskId < data.numberOfDisks);

        // cut off degenerate moves
        if (server == distribution[diskId])
        {
            continue;
        }

        atomMoves.emplace_back(server, distribution[diskId], diskId);
    }

    std::vector< std::unique_ptr<IMove> > result;

    if (atomMoves.empty())
    {
        return result;
    }

    std::unique_ptr<IMove> move(new CompoundMove(std::move(atomMoves)));
    if (solution.moveIsCorrect(*move))
    {
        result.emplace_back(std::move(move));
    }

    return result;
}
