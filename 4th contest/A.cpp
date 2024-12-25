#include <algorithm>
#include <iostream>
#include <vector>

struct DSU {
  std::vector<int> parent;
  std::vector<int> rank;
  std::vector<int> rows_count;
  int max_size;

  explicit DSU(int size, const std::vector<int>& tables)
      : parent(size + 1), rank(size + 1, 1), rows_count(size + 1) {
    for (int i = 1; i <= size; ++i) {
      parent[i] = i;
      rows_count[i] = tables[i];
    }
    max_size = *std::max_element(rows_count.begin(), rows_count.end());
  }

  int FindSet(int v) {
    if (v == parent[v]) {
      return parent[v];
    }
    return parent[v] = FindSet(parent[v]);
  }

  void Unite(int u, int v) {
    u = FindSet(u);
    v = FindSet(v);

    if (u != v) {
      if (rank[u] < rank[v]) {
        std::swap(u, v);
      }
      parent[v] = u;
      ++rank[u];
      rows_count[u] += rows_count[v];
      rows_count[v] = 0;
      max_size = std::max(max_size, rows_count[u]);
    }
  }
};

void UniteTables(int n, int m, const std::vector<int>& tables) {
  DSU dsu(n, tables);

  for (size_t i = 0; i < m; ++i) {
    int destination = 0;
    int source = 0;
    std::cin >> destination >> source;
    dsu.Unite(destination, source);
    std::cout << dsu.max_size << '\n';
  }

}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<int> tables(n + 1);
  for (size_t i = 1; i <= n; ++i) {
    std::cin >> tables[i];
  }

  UniteTables(n, m, tables);

  return 0;
}
