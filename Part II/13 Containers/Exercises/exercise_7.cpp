#include <stdexcept>
#include <initializer_list>
#include <vector>
#include <gtest/gtest.h>

size_t calculate_cols(size_t len, size_t rows) {
    if (len % rows != 0) {
        throw std::logic_error("Len is not a multiple of rows.");
    }
    return len / rows;
}

template <typename T>
struct Matrix {
    Matrix(std::initializer_list<T> val)
        :   rows{static_cast<size_t>(*val.begin())},
            cols{ calculate_cols(val.size() - 1, rows)},
            data(cols, std::vector<T>{}) {
        auto itr = val.begin();
        itr++;
        for(size_t row{}; row<cols; row++){
            data[row].assign(itr, itr+cols);
            itr += cols;
        }
    }
    T& at(size_t row, size_t col) {
        if (row >= rows || col >= cols)
            throw std::out_of_range{ "Index invalid." };
        return data[row][col];
    }
    const size_t rows;
    const size_t cols;
private:
    std::vector<std::vector<T>> data;
};

TEST(MatrixTest, ThrowDimNotFit) {
    ASSERT_THROW((Matrix<int>{ 2,
        2, 3, 5,
        6, 2, 6,
        5, 6, 1
    }), std::logic_error);
}

TEST(MatrixTest, NoThrowDimFit) {
    ASSERT_NO_THROW((Matrix<int>{ 3,
                                   2, 3, 5,
                                   6, 2, 6,
                                   5, 6, 1
    }));
}

TEST(MatrixTest, ColsRows) {
    Matrix<int> mat{ 3,
      2, 3, 5, 7,
      6, 2, 6, 9,
      5, 6, 1, 4
    };
    ASSERT_TRUE(mat.cols == 4);
    ASSERT_TRUE(mat.rows == 3);
    ASSERT_TRUE(mat.at(2,3) == 4);
}

TEST(MatrixTest, ThrowOutOfIndex) {
    Matrix<int> mat{ 3,
                     2, 3, 5, 7,
                     6, 2, 6, 9,
                     5, 6, 1, 4
    };
    ASSERT_THROW(mat.at(3,3), std::out_of_range);
}