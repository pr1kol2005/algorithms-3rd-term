#include <cstdint>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Line {
  int64_t k;
  int64_t b;

  Line() = default;
  Line(int64_t k_, int64_t b_) : k(k_), b(b_) {}

  int64_t GetY(int64_t x) const {
    if (x == INT64_MIN) {
      return INT64_MIN;
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
      points.push_back(INT64_MIN);
    } else {
      points.push_back(hull.back().Cross(line));
    }
    hull.push_back(line);
  }

  int64_t GetMinY(int64_t x) const {
    int64_t pos =
        std::lower_bound(points.begin(), points.end(), x) - points.begin() - 1;
    // if (pos < 0) {
    //   pos = 0;
    // }
    return hull[pos].GetY(x);
  }
};

int64_t SolveProblem(const std::vector<int64_t>& rocks, int64_t c) {
  std::vector<int64_t> dp(rocks.size(), INT64_MAX);
  int64_t result = 0;
  LowerConvexHull trick;

  dp[0] = 0;

  for (int64_t i = 1; i < rocks.size(); ++i) {
    int64_t k = -2 * rocks[i - 1];
    int64_t b = dp[i - 1] + rocks[i - 1] * rocks[i - 1];

    // std::cout << k << ' ' << b << '\n';

    trick.AddLine({k, b});

    int64_t curr = c + rocks[i] * rocks[i];
    curr += trick.GetMinY(rocks[i]);

    dp[i] = std::min(dp[i], curr);
    // std::cout << trick.GetMinY(rocks[i]) << ' ';
  }
  // std::cout << '\n';

  result = dp[rocks.size() - 1];

  // for (auto& x : trick.hull) {
  //   std::cout << x.k << ' ' << x.b << '|';
  // }
  // std::cout << '\n';

  // for (auto& x : trick.points) {
  //   std::cout << x << ' ';
  // }
  // std::cout << '\n';

  // for (int64_t i = 0; i < dp.size(); ++i) {
  //   std::cout << dp[i] << ' ';
  // }
  // std::cout << '\n';

  return result;
}

int main() {
  int64_t n = 0;
  int64_t c = 0;

  std::cin >> n;
  std::cin >> c;

  std::vector<int64_t> rocks(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> rocks[i];
  }

  // LowerConvexHull h;

  // h.hull = {{1, 0}, {0, 3}, {-1, 9}};
  // h.points = {-999, 3, 6};

  // std::cout << h.GetMinY(0) << '\n';

  std::cout << SolveProblem(rocks, c) << '\n';

  return 0;
}