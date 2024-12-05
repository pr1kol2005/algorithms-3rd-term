#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

const std::size_t kMaxPathLength = 101;

Matrix<std::size_t> MakeAdjacencyMatrix(std::size_t n) {
  Matrix<std::size_t> adj_matrix(n, std::vector<std::size_t>(n));
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      std::size_t value = 0;
      std::cin >> value;
      adj_matrix[i][j] = value == 0 ? kMaxPathLength : value;
    }
  }
  return adj_matrix;
}

Matrix<std::size_t> BellmanFord(std::size_t n,
                                const Matrix<std::size_t>& adj_matrix) {
  Matrix<std::size_t> dist = adj_matrix;

  for (std::size_t k = 0; k < n; ++k) {
    for (std::size_t i = 0; i < n; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        if (dist[i][k] < kMaxPathLength && dist[k][j] < kMaxPathLength) {
          dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  return dist;
}

void DisplayShortestPaths(const Matrix<std::size_t>& shortest_paths,
                          std::size_t n) {
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      std::cout << (shortest_paths[i][j] == kMaxPathLength ? 0 : 1) << ' ';
    }
    std::cout << '\n';
  }
}

int main() {
  std::size_t n = 0;
  std::cin >> n;

  Matrix<std::size_t> adj_matrix = MakeAdjacencyMatrix(n);
  Matrix<std::size_t> shortest_paths = BellmanFord(n, adj_matrix);

  DisplayShortestPaths(shortest_paths, n);

  return 0;
}
