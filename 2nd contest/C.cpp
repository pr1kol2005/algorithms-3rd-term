#include <iostream>
#include <limits>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void InputData(Matrix<int>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      std::cin >> adj_matrix[i][j];
    }
  }
}

bool Contains(size_t mask, int vertex) { return mask & (1 << vertex); }

void FindHamiltonianPath(const Matrix<int>& adj_matrix) {
  int n = adj_matrix.size();
  Matrix<int> dp(n,
                 std::vector<int>((1 << n), std::numeric_limits<int>::max()));
  size_t max_mask = (1 << n) - 1;
  Matrix<int> prev(n, std::vector<int>((1 << n), -1));
  // dp[v][mask] min cost path ending in v (0, n - 1) and including all vertices
  // from mask mask == 1 << n -- nothing is visited

  for (int v = 0; v < n; ++v) {
    dp[v][(1 << v)] = 0;
  }

  for (int mask = 1; mask <= max_mask; ++mask) {
    for (int u = 0; u < n; ++u) {
      if (Contains(mask, u)) {
        for (int v = 0; v < n; ++v) {
          if (!Contains(mask, v) && u != v) {
            if (dp[u][mask] + adj_matrix[u][v] < dp[v][mask | (1 << v)]) {
              dp[v][mask | (1 << v)] = dp[u][mask] + adj_matrix[u][v];
              prev[v][mask | (1 << v)] = u;
            }
          }
        }
      }
    }
  }

  int min_cost = std::numeric_limits<int>::max();
  std::vector<int> path;
  int last = 0;
  int last_mask = max_mask;

  for (int v = 0; v < n; ++v) {
    if (dp[v][max_mask] < min_cost) {
      min_cost = dp[v][max_mask];
      last = v;
    }
  }

  while (last_mask > 0) {
    // std::cout << last + 1 << ' ' << last_mask << '\n';
    path.push_back(last + 1);
    int temp = last;
    last = prev[last][last_mask];
    last_mask = last_mask & ~(1 << temp);
  }

  // std::cout << prev[last][last_mask] << '\n';

  // for (int i = 0; i < prev.size(); i++) {
  //   for (int j = 0; j < prev[i].size(); j++) {
  //     std::cout << prev[i][j] << ' ';
  //   }
  //   std::cout << '\n';
  // }

  std::cout << min_cost << '\n';

  if (path.size() == 0) {
    std::cout << 1 << '\n';
  } else {
    for (auto it = path.rbegin(); it != path.rend(); it++) {
      std::cout << *it << ' ';
    }
    std::cout << '\n';
  }
}

int main() {
  int n;

  std::cin >> n;

  Matrix<int> adj_matrix(n, std::vector<int>(n));

  InputData(adj_matrix);

  FindHamiltonianPath(adj_matrix);

  return 0;
}