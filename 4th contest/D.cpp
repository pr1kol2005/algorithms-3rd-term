#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

const int kMaxCapacity = 201;

void FillMatrix(size_t n, size_t m, Matrix<int>& capacity) {
  for (size_t i = 1; i <= n; ++i) {
    capacity[i][i] = kMaxCapacity;
  }

  for (size_t i = 0; i < m; ++i) {
    int u = 0;
    int v = 0;
    int w = 0;
    std::cin >> u >> v >> w;
    capacity[u][v] = std::max(capacity[u][v], w);
    capacity[v][u] = std::max(capacity[v][u], w);
  }
}

void CalculateCapacities(size_t n, Matrix<int>& capacity) {
  for (size_t k = 1; k <= n; ++k) {
    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 1; j <= n; ++j) {
        capacity[i][j] =
            std::max(capacity[i][j], std::min(capacity[i][k], capacity[k][j]));
      }
    }
  }
}

void PrintResults(size_t k, const Matrix<int>& capacity) {
  for (size_t i = 0; i < k; ++i) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    std::cout << capacity[u][v] << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t n = 0;
  size_t m = 0;
  size_t k = 0;
  std::cin >> n >> m >> k;

  Matrix<int> capacity(n + 1, std::vector<int>(n + 1, 0));

  FillMatrix(n, m, capacity);

  CalculateCapacities(n, capacity);

  PrintResults(k, capacity);

  return 0;
}
