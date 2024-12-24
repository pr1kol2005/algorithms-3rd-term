#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

const int kMaxCapacity = 201;

class Graph {
  Matrix<int> capacity_matrix_;
  size_t vertice_count_;

 public:
  Graph() = default;

  explicit Graph(size_t size)
      : capacity_matrix_(size + 1, std::vector<int>(size + 1, 0)),
        vertice_count_(size) {}

  int GetCapacity(size_t u, size_t v) const { return capacity_matrix_[u][v]; }

  void ReadGraph(size_t n, size_t m) {
    for (size_t i = 1; i <= n; ++i) {
      capacity_matrix_[i][i] = kMaxCapacity;
    }

    for (size_t i = 0; i < m; ++i) {
      int u = 0;
      int v = 0;
      int w = 0;
      std::cin >> u >> v >> w;
      capacity_matrix_[u][v] = std::max(capacity_matrix_[u][v], w);
      capacity_matrix_[v][u] = std::max(capacity_matrix_[v][u], w);
    }
  }

  void CalculateCapacities() {
    for (size_t k = 1; k <= vertice_count_; ++k) {
      for (size_t i = 1; i <= vertice_count_; ++i) {
        for (size_t j = 1; j <= vertice_count_; ++j) {
          capacity_matrix_[i][j] = std::max(
              capacity_matrix_[i][j],
              std::min(capacity_matrix_[i][k], capacity_matrix_[k][j]));
        }
      }
    }
  }
};

void RespondToRequests(size_t k, const Graph& graph) {
  for (size_t i = 0; i < k; ++i) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    std::cout << graph.GetCapacity(u, v) << '\n';
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

  graph.CalculateCapacities();

  RespondToRequests(k, graph);

  return 0;
}
