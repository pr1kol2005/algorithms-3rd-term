#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

const int kMaxCapacity = 201;

struct Graph {
  Matrix<int> capacity_matrix;

  Graph() = default;

  explicit Graph(int size) : capacity_matrix(size + 1, std::vector<int>(size + 1, 0)) {}

  void ReadGraph(size_t n, size_t m) {
    for (size_t i = 1; i <= n; ++i) {
      capacity_matrix[i][i] = kMaxCapacity;
    }

    for (size_t i = 0; i < m; ++i) {
      int u = 0;
      int v = 0;
      int w = 0;
      std::cin >> u >> v >> w;
      capacity_matrix[u][v] = std::max(capacity_matrix[u][v], w);
      capacity_matrix[v][u] = std::max(capacity_matrix[v][u], w);
    }
  }
};

void CalculateCapacities(size_t n, Graph& graph) {
  for (size_t k = 1; k <= n; ++k) {
    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 1; j <= n; ++j) {
        graph.capacity_matrix[i][j] = std::max(
            graph.capacity_matrix[i][j],
            std::min(graph.capacity_matrix[i][k], graph.capacity_matrix[k][j]));
      }
    }
  }
}

void PrintResults(size_t k, const Graph& graph) {
  for (size_t i = 0; i < k; ++i) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    std::cout << graph.capacity_matrix[u][v] << '\n';
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

  Graph graph(n);

  graph.ReadGraph(n, m);

  CalculateCapacities(n, graph);

  PrintResults(k, graph);

  return 0;
}
