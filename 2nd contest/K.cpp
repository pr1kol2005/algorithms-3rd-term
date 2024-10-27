#include <iostream>
#include <limits>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Line {
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

  int64_t k;
  int64_t b;
};

struct LowerConvexHull {
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

  std::vector<Line> hull;
  std::vector<int64_t> points;
};

int64_t MinSumOfLengthSquares(int n, int k) {
  Matrix<int64_t> optimal_sum(
      n + 1, std::vector<int64_t>(k + 1, std::numeric_limits<int64_t>::max()));

  // optimal_sum[i][j] == optimal sum to cover i points with j segments
  // optimal_sum[0][j] = 0
  // optimal_sum[i != 0][0] = INF

  for (size_t j = 0; j <= k; j++) {
    optimal_sum[0][j] = 0;
  }

  for (size_t j = 1; j <= k; j++) {
    LowerConvexHull trick;
    for (int64_t i = 1; i <= n; i++) {
      int64_t line_k = -2 * i;
      int64_t line_b = 0;
      if (optimal_sum[i - 1][j - 1] != std::numeric_limits<int64_t>::max()) {
        line_b = optimal_sum[i - 1][j - 1] + i * i;
        trick.AddLine({line_k, line_b});
      }
      optimal_sum[i][j] = trick.GetMinY(i) + (i) * (i);
    }
  }

  if (optimal_sum[n][k] == std::numeric_limits<int64_t>::max()) {
    return 0;
  }
  return optimal_sum[n][k];
}

int main() {
  int n = 0;
  int k = 0;

  std::cin >> n;
  std::cin >> k;

  std::cout << MinSumOfLengthSquares(n, k) << '\n';

  return 0;
}