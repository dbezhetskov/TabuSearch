#ifndef _VECTOR_SOLUTION_H_
#define _VECTOR_SOLUTION_H_

#include "ISolution.hpp"
#include "TaskData.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class VectorSolution : public ISolution
{
public:
    VectorSolution(const std::shared_ptr<const TaskData> _data);

    VectorSolution(const VectorSolution& other);

    VectorSolution& operator=(VectorSolution& rhs);

    void swap(VectorSolution& other);

    virtual double tryOnMove(const IMove& move) const override;

    virtual double tryOnInsertDisk(const size_t server, const size_t disk, const std::vector<size_t>& removedDisks) const override;

    virtual void applyMove(const IMove& move) override;

    virtual bool moveIsCorrect(const IMove& move) const override;

    virtual double getObjectiveValue() const override;

    virtual std::vector<size_t> getDistribution() const override;

private:
    struct MatrixIdx
    {
        MatrixIdx()
        {
            server = -1;
            time = -1;
            resource = -1;
        }

        MatrixIdx(size_t _server, size_t _time, size_t _resource)
        {
            server = _server;
            time = _time;
            resource = _resource;
        }

        bool operator==(const MatrixIdx& rhs) const
        {
            return resource == rhs.resource && server == rhs.server && time == rhs.time;
        }

        size_t server;
        size_t time;
        size_t resource;
    };

    struct TripleHash
    {
        std::size_t operator()(const VectorSolution::MatrixIdx &idx) const;
    };

    typedef std::pair<MatrixIdx, double> MatrixElem;

    typedef std::vector<MatrixElem> Changes;

    typedef std::unordered_map<MatrixIdx, double, TripleHash> TMap;

    struct MoveHash
    {
        std::size_t operator()(const IMove::AtomMove& atomMove) const;
    };

private:
    double fillObjectiveValueMatrix(const std::vector<size_t>& removedDisks,
                                    const std::pair<int, size_t> insertedDisk) const;

    void moveDisk(size_t destination, size_t source, size_t diskId);

    std::pair<Changes, double> tryOnAtomeMove(
            TMap *distinct
            , double startObjectiveValue
            , const IMove::AtomMove& atomMove
            ) const;

    bool atomMoveIsCorrect(const IMove::AtomMove &atomMove) const;

    inline bool recalcOverheadsRedundancyByResource(const IMove::AtomMove& atomMove, const size_t resource) const;

    inline bool recalcOverheadsRedundancyByResourceInverse(const IMove::AtomMove& atomMove, const size_t resource) const;

    inline void recalcOverheads(const IMove::AtomMove& atomMove, const size_t resource);

    inline void recalcOverheadsInverse(const IMove::AtomMove& atomMove, const size_t resource);

    double& getElemCapacityMatrix(TMap *distinct, size_t server, size_t time, size_t resource) const;

    inline double& getElemCapacityMatrix(size_t server, size_t time, size_t resource) const;

    inline double& getOverheads(const TaskData::TypeOperation typeOperation, size_t server, size_t resource);

    inline double& getOverheadsRedundancy(const TaskData::TypeOperation typeOperation, size_t server, size_t resource) const;

private:
    std::shared_ptr<const TaskData> data;

	std::vector<size_t> distribution;

    mutable std::unique_ptr< std::vector<double> > matrixCapacity;

    std::vector<double> thresholdOverheadsServerInsert;

    std::vector<double> thresholdOverheadsServerErase;

    mutable std::vector<double> thresholdOverheadsServerInsertRedundancy;

    mutable std::vector<double> thresholdOverheadsServerEraseRedundancy;

    double objectiveValue;

    std::unordered_set<IMove::AtomMove, MoveHash> moves;
};

inline double& VectorSolution::getElemCapacityMatrix(size_t server, size_t time, size_t resource) const
{
    return (*matrixCapacity)[server * data->numberOfTimes * data->numberOfResource + time * data->numberOfResource + resource];
}

inline double& VectorSolution::getOverheads(const TaskData::TypeOperation typeOperation, size_t server, size_t resource)
{
    std::vector<double>* pointer = (TaskData::ERASE == typeOperation) ? &thresholdOverheadsServerErase : &thresholdOverheadsServerInsert;
    return (*pointer)[server * data->numberOfResource + resource];
}

inline double& VectorSolution::getOverheadsRedundancy(const TaskData::TypeOperation typeOperation, size_t server, size_t resource) const
{
    std::vector<double>* pointer =
            (TaskData::ERASE == typeOperation) ? &thresholdOverheadsServerEraseRedundancy : &thresholdOverheadsServerInsertRedundancy;
    return (*pointer)[server * data->numberOfResource + resource];
}

#endif // !_VECTOR_SOLUTION_H_
