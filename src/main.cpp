#include "matrix.hpp"
#include <iostream>
#include <string>
constexpr int SIZE = 9;
int main() {
  Matrix<int, -1, 2> matrix;
  std::cout << "Size before filled " << matrix.size() << std::endl;
  for (int i = 0; i <= SIZE; ++i) {
    matrix[i][i] = i;
  }
  for (int i = 0; i <= SIZE; ++i) {
    matrix[i][SIZE - i] = SIZE - i;
  }

  for (int i = 1; i < SIZE; ++i) {
    for (int j = 1; j < SIZE; ++j) {
      std::cout << matrix[i][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "Size: " << matrix.size() << std::endl;

  for (auto [i1, i2, value] : matrix) {
    std::cout << i1 << ' ' << i2 << " : " << value << std::endl;
  }
}