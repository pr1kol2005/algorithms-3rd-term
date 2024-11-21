#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

const int kMaxPathLength = 101;

Matrix<int> MakeAdjacencyMatrix(size_t n) {
  Matrix<int> adj_matrix(n, std::vector<int>(n));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      size_t value = 0;
      std::cin >> value;
      adj_matrix[i][j] = value == 0 ? kMaxPathLength : value;
    }
  }
  return adj_matrix;
}

Matrix<int> FloydWarshall(int n, Matrix<int> adj_matrix) {
  Matrix<int> dist = adj_matrix;

  for (size_t k = 0; k < n; ++k) {
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (dist[i][k] < kMaxPathLength && dist[k][j] < kMaxPathLength) {
          dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  return dist;
}

int main() {
  size_t n = 0;
  std::cin >> n;

  Matrix<int> adj_matrix = MakeAdjacencyMatrix(n);
  Matrix<int> shortest_paths = FloydWarshall(n, adj_matrix);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      std::cout << (shortest_paths[i][j] == kMaxPathLength ? 0 : 1) << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
