#include "matrix.hpp"
#include "gtest/gtest.h"


TEST(Matrix1d, Reference) {
  Matrix<float, 0.25f, 1> matrix;
  ASSERT_EQ(matrix.size(), 0);
  for (int i = 2; i < 9; ++i) {
    matrix[i] = static_cast<float>(i * i);
  }

  EXPECT_EQ(matrix.size(), 7);

  for (int i = 2; i < 9; ++i) {
    EXPECT_EQ(static_cast<float>(matrix[i]), static_cast<float>(i * i));
  }

  EXPECT_EQ(static_cast<float>(matrix[1]), 0.25f);
  EXPECT_EQ(static_cast<float>(matrix[9]), 0.25f);

  matrix[5] = 0.25f;

  EXPECT_EQ(matrix.size(), 6);
  EXPECT_EQ(static_cast<float>(matrix[5]), 0.25f);
}

TEST(Matrix1d, RangeFor) {
  Matrix<int, -10, 1> matrix;
  for (int i = 2; i < 9; ++i) {
    matrix[i] = i * i;
  }

  int counter = 0;
  for (auto [i, v] : matrix) {
    EXPECT_EQ((i * i), v);
    ++counter;
  }
  EXPECT_EQ(counter, 7);
}

TEST(Matrix2d, Reference) {
  Matrix<float, 0.25f, 2> matrix;
  ASSERT_EQ(matrix.size(), 0);
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2] = static_cast<float>(i * i);
  }

  EXPECT_EQ(matrix.size(), 7);

  for (int i = 2; i < 9; ++i) {
    EXPECT_EQ(static_cast<float>(matrix[i][i * 2]), static_cast<float>(i * i));
  }

  EXPECT_EQ(static_cast<float>(matrix[1][2]), 0.25f);
  EXPECT_EQ(static_cast<float>(matrix[9][18]), 0.25f);

  matrix[5][10] = 0.25f;

  EXPECT_EQ(matrix.size(), 6);
  EXPECT_EQ(static_cast<float>(matrix[5][10]), 0.25f);
}

TEST(Matrix2d, RangeFor) {
  Matrix<int, -10, 2> matrix;
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2] = i * i;
  }

  int counter = 0;
  for (auto [i1, i2, v] : matrix) {
    EXPECT_EQ((i1 * i1), v);
    EXPECT_EQ((i1 * 2), i2);
    ++counter;
  }
  EXPECT_EQ(counter, 7);
}

TEST(Matrix3d, Reference) {
  Matrix<float, 0.25f, 3> matrix;
  ASSERT_EQ(matrix.size(), 0);
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2][i + 10] = static_cast<float>(i * i);
  }

  EXPECT_EQ(matrix.size(), 7);

  for (int i = 2; i < 9; ++i) {
    EXPECT_EQ(static_cast<float>(matrix[i][i * 2][i + 10]),
              static_cast<float>(i * i));
  }

  EXPECT_EQ(static_cast<float>(matrix[1][2][11]), 0.25f);
  EXPECT_EQ(static_cast<float>(matrix[9][18][19]), 0.25f);

  matrix[5][10][15] = 0.25f;

  EXPECT_EQ(matrix.size(), 6);
  EXPECT_EQ(static_cast<float>(matrix[5][10][15]), 0.25f);
}

TEST(Matrix3d, RangeFor) {
  Matrix<int, -10, 3> matrix;
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2][i + 10] = i * i;
  }

  int counter = 0;
  for (auto [i1, i2, i3, v] : matrix) {
    EXPECT_EQ((i1 * i1), v);
    EXPECT_EQ((i1 * 2), i2);
    EXPECT_EQ((i1 + 10), i3);
    ++counter;
  }
  EXPECT_EQ(counter, 7);
}

TEST(Matrix2d, Level) {
  Matrix<int, -10, 2> matrix;
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2] = i * i;
  }

  auto l = matrix[5];
  EXPECT_EQ(static_cast<int>(l[3]), -10);
  EXPECT_EQ(static_cast<int>(l[10]), 25);
}

TEST(Matrix3d, Level) {
  Matrix<int, -10, 3> matrix;
  for (int i = 2; i < 9; ++i) {
    matrix[i][i * 2][i + 10] = i * i;
  }

  auto l = matrix[5][10];
  EXPECT_EQ(static_cast<int>(l[14]), -10);
  EXPECT_EQ(static_cast<int>(l[15]), 25);

  auto dl = matrix[3];
  EXPECT_EQ(static_cast<int>(dl[7][13]), -10);
  EXPECT_EQ(static_cast<int>(dl[6][13]), 9);
}

TEST(Matrix2d, Expression) {
  Matrix<int, 0, 2> matrix;
  ((matrix[100][100] = 314) = 0) = 217;
  ASSERT_EQ(matrix.size(), 1);
  EXPECT_EQ(static_cast<int>(matrix[100][100]), 217);
}