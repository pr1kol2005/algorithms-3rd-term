#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

template <typename T>
using TwoDimVector = std::vector<std::vector<T>>;

struct UBigInt {
  const int kBase = 10;
  const int kDivivsionOperand = 2;

  std::vector<char> digits;

  UBigInt() = default;

  UBigInt(const std::string& str) {
    for (auto it = str.rbegin(); it != str.rend(); it++) {
      digits.push_back(*it - '0');
    }
  }

  UBigInt(int64_t num) : UBigInt(std::to_string(num)) {}

  UBigInt(const UBigInt& other) : digits(other.digits) {}

  UBigInt& operator=(const UBigInt& other) {
    if (this != &other) {
      digits = other.digits;
    }
    return *this;
  }

  friend std::istream& operator>>(std::istream& in, UBigInt& num) {
    std::string str;
    in >> str;
    num = UBigInt(str);
    return in;
  }

  friend std::ostream& operator<<(std::ostream& out, const UBigInt& num) {
    for (auto it = num.digits.rbegin(); it != num.digits.rend(); it++) {
      out << static_cast<char>(*it + '0');
    }
    return out;
  }

  void AddLastDigit(int64_t num) {
    std::reverse(digits.begin(), digits.end());
    if (digits[0] == 0) {
      digits.pop_back();
    }
    digits.push_back(num);
    std::reverse(digits.begin(), digits.end());
  }

  UBigInt& operator-=(const UBigInt& rhs) {
    int64_t carry = 0;
    for (size_t i = 0; i < digits.size(); i++) {
      int64_t curr = 0;
      if (i < digits.size()) {
        curr += digits[i];
      }
      if (i < rhs.digits.size()) {
        curr -= rhs.digits[i];
      }
      curr -= carry;
      if (curr < 0) {
        curr += kBase;
        carry = 1;
      } else {
        carry = 0;
      }
      digits[i] = (curr % kBase);
    }
    while (digits.size() > 1 && digits.back() == 0) {
      digits.pop_back();
    }
    return *this;
  }

  UBigInt& operator/=(const UBigInt&) {
    if (*this == kDivivsionOperand - 1) {
      *this = 0;
      return *this;
    }
    if (*this == kDivivsionOperand - 1) {
      *this = 1;
      return *this;
    }

    int64_t curr = 0;
    UBigInt temp = 0;
    std::vector<char> result;
    int64_t i = digits.size() - 1;

    if (digits[i] < kDivivsionOperand) {
      temp.AddLastDigit(digits[i]);
      --i;
    }

    for (; i >= 0; --i) {
      temp.AddLastDigit(digits[i]);
      for (curr = kBase - 1; temp < curr * kDivivsionOperand; --curr) {
      }
      temp -= (curr * kDivivsionOperand);
      result.push_back(curr);
    }
    digits = result;
    std::reverse(digits.begin(), digits.end());
    return *this;
  }

  int64_t operator%(const UBigInt&) const {
    int64_t last_digit = digits.front();
    return last_digit % kDivivsionOperand;
  }

  UBigInt operator-(const UBigInt&) const {
    UBigInt result = *this;
    result -= 1;
    return result;
  }

  bool operator<(const UBigInt& rhs) const {
    if (digits.size() != rhs.digits.size()) {
      return digits.size() < rhs.digits.size();
    }
    for (size_t i = 0; i < digits.size(); i++) {
      if (digits[i] != rhs.digits[i]) {
        return digits[i] < rhs.digits[i];
      }
    }
    return false;
  }

  bool operator==(const UBigInt& rhs) const {
    if (digits.size() != rhs.digits.size()) {
      return false;
    }
    for (size_t i = 0; i < digits.size(); i++) {
      if (digits[i] != rhs.digits[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const UBigInt& rhs) const { return !(*this == rhs); }
};

struct ModuloInt {
  int64_t value;
  static int64_t modulo;

  ModuloInt(int64_t n) : value(n) {}

  ModuloInt operator*(const ModuloInt& rhs) const {
    ModuloInt result = 0;
    result = (value * rhs.value) % modulo;
    return result;
  }

  ModuloInt& operator+=(const ModuloInt& rhs) {
    value = (value + rhs.value) % modulo;
    return *this;
  }
};

int64_t ModuloInt::modulo = std::numeric_limits<int64_t>::max();

template <typename T>
struct Matrix {
  Matrix(size_t n, size_t m) : n_(n), m_(m) {
    buffer_ = TwoDimVector<T>(n, std::vector<T>(m, 0));
  }

  Matrix(size_t n, size_t m, const std::vector<T>& data) : Matrix(n, m) {
    for (size_t i = 0; i < data.size(); i++) {
      buffer_[i][i] = data[i];
    }
  }

  Matrix(size_t n, size_t m, const TwoDimVector<T>& data)
      : n_(n), m_(m), buffer_(data) {}

  std::vector<T>& operator[](size_t i) { return buffer_[i]; }

  Matrix operator*(const Matrix& rhs) const {
    Matrix result(n_, rhs.m_);
    for (size_t i = 0; i < n_; i++) {
      for (size_t j = 0; j < rhs.m_; j++) {
        for (size_t k = 0; k < m_; k++) {
          result.buffer_[i][j] += buffer_[i][k] * rhs.buffer_[k][j];
        }
      }
    }
    return result;
  }

  Matrix BinPow(UBigInt power) const {
    Matrix init(n_, m_, buffer_);
    Matrix result(n_, n_, std::vector<T>(n_, 1));

    while (power != 0) {
      if (power % 2 == 1) {
        result = result * init;
      }
      init = init * init;
      power /= 2;
    }

    return result;
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.n_; i++) {
      for (size_t j = 0; j < matrix.m_; j++) {
        os << matrix.buffer_[i][j] << ' ';
      }
      os << '\n';
    }
    return os;
  }

  size_t n_;
  size_t m_;
  TwoDimVector<T> buffer_;
};

int64_t No2x2Blocks(const std::vector<int64_t>& mask,
                const std::vector<int64_t>& prev_mask) {
  int64_t prev_1 = -1;
  int64_t prev_2 = -1;
  for (size_t i = 0; i < mask.size(); ++i) {
    int64_t curr_1 = mask[i];
    int64_t curr_2 = prev_mask[i];
    if (prev_1 == prev_2 && curr_1 == curr_2 && curr_1 == prev_1) {
      return 0;
    }
    prev_1 = curr_1;
    prev_2 = curr_2;
  }
  return 1;
}

Matrix<ModuloInt> MakeInitMatrixForProblem(const Matrix<int64_t>& lhs,
                                           const Matrix<int64_t>& rhs) {
  Matrix<ModuloInt> result(lhs.n_, rhs.m_);
  for (size_t i = 0; i < lhs.n_; i++) {
    for (size_t j = 0; j < rhs.m_; j++) {
      std::vector<int64_t> prev_mask(lhs.m_, 0);
      for (size_t k = 0; k < lhs.m_; k++) {
        prev_mask[k] = rhs.buffer_[k][j];
      }
      result.buffer_[i][j] = No2x2Blocks(lhs.buffer_[i], prev_mask);
    }
  }
  return result;
}

int64_t CountVariants(UBigInt n, size_t m) {
  Matrix<int64_t> masks_vertical(1 << m, m);
  Matrix<int64_t> masks_horizontal(m, 1 << m);

  for (uint32_t i = 0; i < masks_vertical.n_; ++i) {
    for (uint32_t j = 0; j < masks_vertical.m_; ++j) {
      masks_vertical.buffer_[i][j] = ((i >> j) & 1);
    }
  }

  for (uint32_t i = 0; i < masks_horizontal.n_; ++i) {
    for (uint32_t j = 0; j < masks_horizontal.m_; ++j) {
      masks_horizontal.buffer_[i][j] = ((j >> i) & 1);
    }
  }

  Matrix<ModuloInt> init_matrix =
      MakeInitMatrixForProblem(masks_vertical, masks_horizontal);

  Matrix<ModuloInt> result_matrix = init_matrix.BinPow(n - 1);

  ModuloInt answer = 0;

  for (size_t i = 0; i < init_matrix.n_; ++i) {
    for (size_t j = 0; j < init_matrix.m_; ++j) {
      answer += result_matrix.buffer_[i][j];
    }
  }

  return answer.value;
}

int main() {
  UBigInt n;
  size_t m = 0;

  std::cin >> n;
  std::cin >> m;
  std::cin >> ModuloInt::modulo;

  std::cout << CountVariants(n, m) << '\n';

  return 0;
}
