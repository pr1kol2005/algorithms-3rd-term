#include <iostream>
#include <limits>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void InputData(Matrix<int>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      char input;
      std::cin >> input;
      if (input == 'N') {
        adj_matrix[i][j] = 0;
      } else {
        adj_matrix[i][j] = 1;
      }
    }
  }
}

void OutputData(Matrix<int>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      std::cout << adj_matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
}

bool Contains(size_t mask, int vertex) { return mask & (1 << vertex); }

void CountMaxPairs(const Matrix<int>& adj_matrix) {
  int n = adj_matrix.size();
  int max_mask = (1 << n) - 1;
  std::vector<int> dp(1 << n, 0);

  // dp[mask] max number of connected computers

  for (int mask = 0; mask <= max_mask; mask++) {
    for (int u = 0; u < n; u++) {
      if (Contains(mask, u)) {
        for (int v = u + 1; v < n; v++) {
          if (Contains(mask, v)) {
            if (adj_matrix[u][v]) {
              dp[mask] = std::max(dp[mask], dp[mask ^ (1 << u) ^ (1 << v)] + 2);
            }
          }
        }
      }
    }
  }

  int max_pairs = 0;

  for (int mask = 0; mask <= max_mask; ++mask) {
    max_pairs = std::max(max_pairs, dp[mask]);
  }

  std::cout << max_pairs << '\n';
}

int main() {
  int n;

  std::cin >> n;

  Matrix<int> adj_matrix(n, std::vector<int>(n));

  InputData(adj_matrix);

  //OutputData(adj_matrix);

  CountMaxPairs(adj_matrix);

  return 0;
}