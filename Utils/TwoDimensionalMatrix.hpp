#ifndef TWODIMENSIONALMATRIX_HPP
#define TWODIMENSIONALMATRIX_HPP

#include <vector>
#include <iostream>
#include <cassert>

template<class T>
class TwoDimensionalMatrix
{
public:
    TwoDimensionalMatrix(const size_t _numberOfRow, const size_t _numberOfColumn)
        : data(_numberOfRow * _numberOfColumn)
        , numberOfRow(_numberOfRow)
        , numberOfColumn(_numberOfColumn)
    {
    }

    TwoDimensionalMatrix()
        : numberOfRow(0)
        , numberOfColumn(0)
    {}

    void init(const size_t _numberOfRow, const size_t _numberOfColumn)
    {
        numberOfRow = _numberOfRow;
        numberOfColumn = _numberOfColumn;

        data = std::vector<T>(numberOfRow * numberOfColumn);
    }

    T get(const size_t indexOfRow, const size_t indexOfColumn) const
    {
        assert(indexOfRow < numberOfRow && indexOfColumn < numberOfColumn);
        return data[numberOfColumn * indexOfRow + indexOfColumn];
    }

    T& get(const size_t indexOfRow, const size_t indexOfColumn)
    {
        assert(indexOfRow < numberOfRow && indexOfColumn < numberOfColumn);
        return data[numberOfColumn * indexOfRow + indexOfColumn];
    }

    TwoDimensionalMatrix<T>& operator+=(const TwoDimensionalMatrix<T>& right)
    {
        assert(numberOfRow == right.numberOfRow);
        assert(numberOfColumn == right.numberOfColumn);

        for (size_t i = 0; i < numberOfRow; ++i)
        {
            for (size_t j = 0; j < numberOfColumn; ++j)
            {
                this->get(i, j) += right.get(i, j);
            }
        }
        return *this;
    }


private:
    friend std::istream& operator>>(std::istream& stream, TwoDimensionalMatrix<T>& twoDimensionalMatrix)
    {
        for (size_t i = 0; i < twoDimensionalMatrix.numberOfRow; ++i)
        {
            for (size_t j = 0; j < twoDimensionalMatrix.numberOfColumn; ++j)
            {
                T elementValue = T();
                stream >> elementValue;
                twoDimensionalMatrix.get(i, j) = elementValue;
            }
        }

        return stream;
    }

private:
    std::vector<T> data;

    size_t numberOfRow;

    size_t numberOfColumn;
};

#endif // TWODIMENSIONALMATRIX_HPP
