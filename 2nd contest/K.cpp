#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Line {
  int64_t k;
  int64_t b;

  Line() = default;
  Line(int64_t k_, int64_t b_) : k(k_), b(b_) {}

  int64_t GetY(int64_t x) const {
    if (x == std::numeric_limits<int64_t>::min()) {
      return std::numeric_limits<int64_t>::min();
    }
    return k * x + b;
  }

  int64_t Cross(const Line& other) const {
    int64_t x = (other.b - b) / (k - other.k);
    if (x < 0) {
      x--;
    }
    return x;
  }
};

struct LowerConvexHull {
  std::vector<Line> hull;
  std::vector<int64_t> points;

  LowerConvexHull() = default;

  void AddLine(const Line& line) {
    while (hull.size() > 0 &&
           hull.back().GetY(points.back()) > line.GetY(points.back())) {
      hull.pop_back();
      points.pop_back();
    }
    if (hull.empty()) {
      points.push_back(std::numeric_limits<int64_t>::min());
    } else {
      points.push_back(hull.back().Cross(line));
    }
    hull.push_back(line);
  }

  int64_t GetMinY(int64_t x) const {
    if (hull.empty()) {
      return 0;
    }
    int64_t pos =
        std::lower_bound(points.begin(), points.end(), x) - points.begin() - 1;
    return hull[pos].GetY(x);
  }
};

int64_t SolveProblem(int64_t n, int64_t k) {
  Matrix<int64_t> dp(
      n + 1, std::vector<int64_t>(k + 1, std::numeric_limits<int64_t>::max()));
  int64_t result = 0;

  // dp[i][j] == optimal sum to cover i points with j segments
  // dp[0][j] = 0
  // dp[i != 0][0] = INF

  for (int64_t j = 0; j <= k; j++) {
    dp[0][j] = 0;
  }

  // for (int64_t j = 1; j <= k; j++) {
  //   for (int64_t i = 1; i <= n; i++) {
  //     for (int64_t p = 0; p < i; p++) {
  //       if (dp[p][j - 1] == std::numeric_limits<int64_t>::max()) {
  //         continue;
  //       }
  //       dp[i][j] = std::min(dp[i][j], dp[p][j - 1] + (i - p - 1) * (i - p -
  //       1));
  //     }
  //   }
  // }

  for (int64_t j = 1; j <= k; j++) {
    LowerConvexHull trick;
    for (int64_t i = 1; i <= n; i++) {
      int64_t k = -2 * i;
      int64_t b = 0;
      if (dp[i - 1][j - 1] != std::numeric_limits<int64_t>::max()) {
        b = dp[i - 1][j - 1] + i * i;
        trick.AddLine({k, b});
      }
      // std::cout << k << " | " << b << " | " << trick.GetMinY(i) << " | "  <<
      // i * i << " | " << '\n';
      dp[i][j] = trick.GetMinY(i) + (i) * (i);
    }
  }

  // for (int64_t i = 0; i <= n; i++) {
  //   if (dp[i][1] < 0) {
  //     std::cout << i << ' ' << dp[i - 1][1] << '\n';
  //     break;
  //   }
  //   for (int64_t j = 0; j <= k; j++) {
  //     std::cout << dp[i][j] << ' ';
  //   }
  //   std::cout << '\n';
  // }

  result = dp[n][k];

  if (result == std::numeric_limits<int64_t>::max()) {
    return 0;
  }
  return result;
}

int main() {
  int64_t n = 0;
  int64_t k = 0;

  std::cin >> n;
  std::cin >> k;

  std::cout << SolveProblem(n, k) << '\n';

  return 0;
}