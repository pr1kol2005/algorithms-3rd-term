#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

Matrix<int> MakeAdjastencyList(int vertice_num, int edge_num) {
  Matrix<int> adj_list(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;

    adj_list[u].push_back(v);
  }
  return adj_list;
}

bool TopSortDFS(int v, Matrix<int>& adj_list, std::vector<int>& color,
                std::vector<int>& result) {
  color[v] = 1;

  for (int u : adj_list[v]) {
    if (color[u] == 1) {
      return true;
    }
    if (color[u] == 0) {
      if (TopSortDFS(u, adj_list, color, result)) {
        return true;
      }
    }
  }

  color[v] = 2;
  result.push_back(v);
  return false;
}

void TopSort(Matrix<int>& adj_list) {
  std::vector<int> color(adj_list.size(), 0);
  std::vector<int> result;

  bool has_cycle = false;
  for (int i = 1; i < adj_list.size(); ++i) {
    if (color[i] == 0) {
      if (TopSortDFS(i, adj_list, color, result)) {
        std::cout << -1 << '\n';
        return;
      }
    }
  }

  std::reverse(result.begin(), result.end());

  for (const auto& v : result) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  Matrix<int> adj_list = MakeAdjastencyList(n, m);

  TopSort(adj_list);
}
