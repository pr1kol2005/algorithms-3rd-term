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

  ModuloInt operator*(const ModuloInt& rhs) {
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
  size_t n_;
  size_t m_;
  TwoDimVector<T> buffer_;

 public:
  Matrix(int n, int m, const TwoDimVector<T>& data)
      : n_(n), m_(m), buffer_(data) {}

  std::vector<T>& operator[](size_t i) { return buffer_[i]; }

  Matrix operator*(Matrix& rhs) {
    Matrix c(n_, rhs.m_, {n_, std::vector<T>(rhs.m_, 0)});
    for (int i = 0; i < n_; i++) {
      for (int j = 0; j < rhs.m_; j++) {
        for (int k = 0; k < m_; k++) {
          c.buffer_[i][j] += (buffer_[i][k] * rhs.buffer_[k][j]);
        }
      }
    }
    return c;
  }

  Matrix BinPow(uint64_t p) {
    Matrix a(n_, m_, buffer_);
    Matrix result(n_, n_, {n_, std::vector<T>(n_, 0)});
    for (int i = 0; i < result.n_; i++) {
      result.buffer_[i][i] = 1;
    }

    while (p > 0) {
      if (p % 2 == 1) {
        result = result * a;
      }
      a = a * a;
      p /= 2;
    }

    return result;
  }
};

uint64_t ComputeFibonacci(uint64_t n) {
  Matrix<ModuloInt<kModulo>> initial_row(5, 1, {{8}, {4}, {2}, {1}, {1}});

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

  std::cout << ComputeFibonacci(n) << '\n';
}