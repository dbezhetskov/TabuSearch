#ifndef THREEDIMENSIONALMATRIX_HPP
#define THREEDIMENSIONALMATRIX_HPP

#include <vector>
#include <iostream>

template<class T>
class ThreeDimensionalMatrix
{
public:
    ThreeDimensionalMatrix(const size_t _numberOfLayers, const size_t _numberOfRow, const size_t _numberOfColumn)
        : data(_numberOfLayers * _numberOfRow * _numberOfColumn)
        , numberOfLayers(_numberOfLayers)
        , numberOfRow(_numberOfRow)
        , numberOfColumn(_numberOfColumn)
    {}

    ThreeDimensionalMatrix()
        : numberOfLayers(0)
        , numberOfRow(0)
        , numberOfColumn(0)
    {}

    void init(const size_t _numberOfLayers, const size_t _numberOfRow, const size_t _numberOfColumn)
    {
        numberOfLayers = _numberOfLayers;
        numberOfRow = _numberOfRow;
        numberOfColumn = _numberOfColumn;

        data = std::vector<T>(numberOfLayers * numberOfRow * numberOfColumn);
    }

    T get(const size_t indexOfLayer, const size_t indexOfRow, const size_t indexOfColumn) const
    {
        return data[indexOfLayer * numberOfRow * numberOfColumn + indexOfRow * numberOfColumn + indexOfColumn];
    }

    T& get(const size_t indexOfLayer, const size_t indexOfRow, const size_t indexOfColumn)
    {
        return data[indexOfLayer * numberOfRow * numberOfColumn + indexOfRow * numberOfColumn + indexOfColumn];
    }

private:
    friend std::istream& operator>>(std::istream& stream, ThreeDimensionalMatrix<T>& threeDimensionalMatrix)
    {
        for (size_t i = 0; i < threeDimensionalMatrix.numberOfLayers; ++i)
        {
            for (size_t j = 0; j < threeDimensionalMatrix.numberOfRow; ++j)
            {
                for (size_t k = 0; k < threeDimensionalMatrix.numberOfColumn; ++k)
                {
                    T elementValue = T();
                    stream >> elementValue;
                    threeDimensionalMatrix.get(i, j, k) = elementValue;
                }
            }
        }

        return stream;
    }

private:
    std::vector<T> data;

    size_t numberOfLayers;

    size_t numberOfRow;

    size_t numberOfColumn;
};

#endif // THREEDIMENSIONALMATRIX_HPP
