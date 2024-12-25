#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using Cube = Matrix<std::vector<T>>;

using StrVector = std::vector<std::string>;
using PairVector = std::vector<std::pair<int, int>>;

const int kModulo = 1000000007;

template <int M = kModulo>
struct ModuloInt {
  int value;

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

  ModuloInt operator+(const ModuloInt& rhs) const {
    ModuloInt result = *this;
    result += rhs;
    return result;
  }
};

void InputCity(StrVector& city, StrVector& city_turned) {
  for (size_t i = 0; i < city.size(); ++i) {
    std::cin >> city[i];
  }

  for (size_t i = 0; i < city_turned.size(); ++i) {
    for (size_t j = 0; j < city.size(); ++j) {
      city_turned[i].push_back(city[j][i]);
    }
  }
}

void OtputCity(const StrVector& city) {
  for (size_t i = 0; i < city.size(); ++i) {
    std::cout << city[i];
    std::cout << '\n';
  }
}

std::string CreateTemplat(const StrVector& city, int column, int fracture) {
  int profile_len = city[0].size() + 1;
  std::string templat;
  for (int i = 0; i < profile_len; ++i) {
    if (i + fracture < profile_len) {
      templat += city[column - 1][i + fracture - 1];
    } else {
      templat += city[column][i + fracture - profile_len];
    }
  }
  return templat;
}

std::pair<int, int> CreateZerosOnesTemplate(const std::string& templat) {
  // column и fracture нумируются с единицы

  int zeros = 0;
  int ones = 0;

  for (int i = 0; i < int(templat.size()); ++i) {
    if (templat[i] == '.') {
      continue;
    }
    if (templat[i] == '-') {
      zeros += (1 << i);
    }
    if (templat[i] == '+') {
      ones += (1 << i);
    }
  }

  return {zeros, ones};
}

bool MaskFitsTemplate(int mask, const std::pair<int, int>& templates) {
  // column и fracture нумируются с единицы

  int zeros = templates.first;
  int ones = templates.second;

  if (((~mask) & zeros) == zeros && (mask & ones) == ones) {
    return true;
  }
  return false;
}

int NextPiece(int mask, int max_mask) {
  int len = int(log2(max_mask));
  int first = (mask >> 0) & 1;
  int second = (mask >> 1) & 1;
  int last = (mask >> len) & 1;
  if (first + second + last == 1) {
    return 1;
  }
  if (first + second + last == 2) {
    return 0;
  }
  return -1;
}

int CountVariants(int n, int m, const StrVector& city) {
  int max_mask = (1 << (m + 1)) - 1;
  int first_row_max_mask = (1 << m) - 1;
  int profile_len = m + 1;
  Cube<ModuloInt<>> dp(
      n + 1,
      Matrix<ModuloInt<>>(m + 1, std::vector<ModuloInt<>>(max_mask + 1, 0)));
  Matrix<std::pair<int, int>> templates(
      n + 1, std::vector<std::pair<int, int>>(m + 1, {0, 0}));

  // dp[i][j][mask] == varaints of block distribution in first i rows before
  // fracture j with mask

  // city обходится снизу вверх

  for (int i = 1; i < n; ++i) {
    for (int j = 1; j <= m; ++j) {
      templates[i][j] = CreateZerosOnesTemplate(CreateTemplat(city, i, j));
    }
  }

  templates[0][m] = CreateZerosOnesTemplate(city[0]);

  // for (int i = 0; i <= n; ++i) {
  //   for (int j = 1; j <= m; ++j) {
  //     std::cout << '{' << templates[i][j].first << " | " <<
  //     templates[i][j].second << '}' << ' ';
  //   }
  //   std::cout << '\n';
  // }

  for (int mask = 0; mask <= first_row_max_mask; ++mask) {
    if (MaskFitsTemplate(mask, templates[0][m])) {
      dp[0][m][mask] = 1;
    }
  }

  int answer = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 1; j <= m; ++j) {
      if (i == 0 && j != m) {
        continue;
      }
      if (i == n - 1 && j == m) {
        continue;
      }

      int next_mask = 0;

      if (i == 0 && j == m) {
        for (int start_mask = 0; start_mask <= first_row_max_mask;
             ++start_mask) {
          if (MaskFitsTemplate(start_mask, templates[i][j])) {
            for (int k = 0; k <= 1; k++) {
              next_mask = (k << m) + start_mask;
              if (MaskFitsTemplate(next_mask, templates[1][1])) {
                dp[1][1][next_mask] += dp[i][j][start_mask];
              }
            }
          }
        }
        continue;
      }
      for (int prev_mask = 0; prev_mask <= max_mask; ++prev_mask) {
        if (j == m) {
          for (int k = 0; k <= 1; k++) {
            next_mask = (k << m) + (prev_mask >> 1);
            if (MaskFitsTemplate(next_mask, templates[i + 1][1])) {
              dp[i + 1][1][next_mask] += dp[i][j][prev_mask];
            }
          }
        } else {
          if (MaskFitsTemplate(prev_mask, templates[i][j]) &&
              NextPiece(prev_mask, max_mask) != -1) {
            next_mask =
                (NextPiece(prev_mask, max_mask) << m) + (prev_mask >> 1);
            // std::cout << next_mask << " | " << templates[i][j + 1].first << "
            // " << templates[i][j + 1].second << '\n'; std::cout << i << " " <<
            // j << " " << prev_mask << " " <<  next_mask << '\n';
            if (MaskFitsTemplate(next_mask, templates[i][j + 1])) {
              dp[i][j + 1][next_mask] += dp[i][j][prev_mask];
            }
          }
        }
      }
    }
  }

  ModuloInt<> result = 0;

  // for (int j = 1; j <= m; ++j) {
  //   for (int mask = 0; mask <= max_mask; ++mask) {
  //     std::cout << dp[2][j][mask].value << ' ';
  //   }
  //   std::cout << '\n';
  // }

  for (int mask = 0; mask <= max_mask; ++mask) {
    result += dp[n - 1][m][mask];
  }

  return result.value;
}

int main() {
  int n = 0;
  int m = 0;

  std::cin >> n;
  std::cin >> m;

  StrVector city(n);
  StrVector city_turned(m);

  InputCity(city, city_turned);

  if (n < m) {
    std::swap(n, m);
    std::swap(city, city_turned);
  }

  // OtputCity(city);

  std::cout << CountVariants(n, m, city) << '\n';

  // int max_mask = (1 << (m + 1)) - 1;

  // std::cout << NextPiece(1, max_mask) << '\n';

  return 0;
}
