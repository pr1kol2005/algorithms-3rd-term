#include <iostream>
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

  friend Matrix Multi(Matrix a, Matrix b);

  friend Matrix BinPow(Matrix a, uint64_t p);
};

Matrix Multi(Matrix a, Matrix b) {
  Matrix c(a.n_, b.m_, {a.n_, std::vector<uint64_t>(b.m_, 0)});
  for (int i = 0; i < a.n_; i++) {
    for (int j = 0; j < b.m_; j++) {
      for (int k = 0; k < a.m_; k++) {
        c.buffer_[i][j] += (a.buffer_[i][k] * b.buffer_[k][j]) % kModulo;
      }
    }
  }
  return c;
}

Matrix BinPow(Matrix a, uint64_t p) {
  Matrix b(a.n_, a.n_, {a.n_, std::vector<uint64_t>(a.n_, 0)});
  for (int i = 0; i < b.n_; i++) {
    b.buffer_[i][i] = 1;
  }

  while (p > 0) {
    if (p & 1) {
      b = Multi(b, a);
    }
    a = Multi(a, a);
    p >>= 1;
  }

  return b;
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