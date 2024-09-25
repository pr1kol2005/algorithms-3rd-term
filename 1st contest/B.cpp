#include <stdint.h>

#include <iostream>
#include <vector>

template <typename T>
using TwoDimVector = std::vector<std::vector<T>>;

const int kModulo = 1000003;

template <int M>
struct ModuloInt {
  uint64_t value;

  ModuloInt(int n) : value(n) {}

  ModuloInt operator*(const ModuloInt& rhs) const {
    ModuloInt result = 0;
    result = (value * rhs.value) % M;
    return result;
  }

  ModuloInt& operator+=(const ModuloInt& rhs) {
    value = (value + rhs.value) % M;
    return *this;
  }
};

template <typename T>
class Matrix {
 public:
  Matrix(int n, int m) : n_(n), m_(m) {
    buffer_ = TwoDimVector<T>(n, std::vector<T>(m, 0));
  }

  Matrix(int n, int m, const std::vector<T>& data) : Matrix(n, m) {
    for (int i = 0; i < data.size(); i++) {
      buffer_[i][i] = data[i];
    }
  }

  Matrix(int n, int m, const TwoDimVector<T>& data)
      : n_(n), m_(m), buffer_(data) {}

  std::vector<T>& operator[](size_t i) { return buffer_[i]; }

  Matrix operator*(const Matrix& rhs) const {
    Matrix result(n_, rhs.m_);
    for (int i = 0; i < n_; i++) {
      for (int j = 0; j < rhs.m_; j++) {
        for (int k = 0; k < m_; k++) {
          result.buffer_[i][j] += buffer_[i][k] * rhs.buffer_[k][j];
        }
      }
    }
    return result;
  }

  Matrix BinPow(uint64_t power) const {
    Matrix init(n_, m_, buffer_);
    Matrix result(n_, n_, std::vector<T>(n_, 1));

    while (power > 0) {
      if (power % 2 == 1) {
        result = result * init;
      }
      init = init * init;
      power /= 2;
    }

    return result;
  }

 private:
  size_t n_;
  size_t m_;
  TwoDimVector<T> buffer_;
};

uint64_t CountWays(uint64_t n) {
  TwoDimVector<ModuloInt<kModulo>> initial_row_value = {
      {8}, {4}, {2}, {1}, {1}};

  Matrix<ModuloInt<kModulo>> initial_row(5, 1, initial_row_value);

  Matrix<ModuloInt<kModulo>> transition_matrix(5, 5,
                                               {{1, 1, 1, 1, 1},
                                                {1, 0, 0, 0, 0},
                                                {0, 1, 0, 0, 0},
                                                {0, 0, 1, 0, 0},
                                                {0, 0, 0, 1, 0}});

  Matrix<ModuloInt<kModulo>> transition_matrix_n =
      transition_matrix.BinPow(n - 1);

  Matrix<ModuloInt<kModulo>> result = transition_matrix_n * initial_row;

  return result[4][0].value;
}

int main() {
  uint64_t n = 0;

  std::cin >> n;

  std::cout << CountWays(n) << '\n';
}