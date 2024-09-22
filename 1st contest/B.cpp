#include <iostream>
#include <stdint.h>
#include <vector>

template <typename T>
using TwoDimVector = std::vector<std::vector<T>>;

const int kModulo = 1000003;

class Matrix {
  size_t n_;
  size_t m_;
  TwoDimVector<uint64_t> buffer_;

 public:
  Matrix(int n, int m, TwoDimVector<uint64_t> data)
      : n_(n), m_(m), buffer_(data) {}

  TwoDimVector<uint64_t> Data() { return buffer_; }

  friend Matrix Multi(Matrix lhs, Matrix rhs);

  friend Matrix BinPow(Matrix a, uint64_t p);
};

Matrix Multi(Matrix lhs, Matrix rhs) {
  Matrix c(lhs.n_, rhs.m_, {lhs.n_, std::vector<uint64_t>(rhs.m_, 0)});
  for (int i = 0; i < lhs.n_; i++) {
    for (int j = 0; j < rhs.m_; j++) {
      for (int k = 0; k < lhs.m_; k++) {
        c.buffer_[i][j] += (lhs.buffer_[i][k] * rhs.buffer_[k][j]) % kModulo;
      }
    }
  }
  return c;
}

Matrix BinPow(Matrix a, uint64_t p) {
  Matrix result(a.n_, a.n_, {a.n_, std::vector<uint64_t>(a.n_, 0)});
  for (int i = 0; i < result.n_; i++) {
    result.buffer_[i][i] = 1;
  }

  while (p > 0) {
    if (p % 2 == 1) {
      result = Multi(result, a);
    }
    a = Multi(a, a);
    p /= 2;
  }

  return result;
}

void SolveProblem(uint64_t n) {
  Matrix a(5, 1, {{8}, {4}, {2}, {1}, {1}});

  Matrix t(5, 5,
           {{1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 1, 0}});

  Matrix t_n = BinPow(t, n - 1);

  Matrix a_n = Multi(t_n, a);

  std::cout << a_n.Data()[4][0] % kModulo << '\n';
}

int main() {
  uint64_t n = 0;

  std::cin >> n;

  SolveProblem(n);
}