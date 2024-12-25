#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

Matrix<int> MakeAdjastencyList(int vertice_num, int edge_num) {
  Matrix<int> adjastency_list(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;

    adjastency_list[u].push_back(v);
  }
  return adjastency_list;
}

void FindCycleDFS(int v, Matrix<int>& adj_list, std::vector<int>& color,
                  std::vector<int>& parent, int& cycle_start, int& cycle_end) {
  color[v] = 1;
  for (int u : adj_list[v]) {
    if (color[u] == 0) {
      parent[u] = v;
      FindCycleDFS(u, adj_list, color, parent, cycle_start, cycle_end);
      if (cycle_end != -1) {
        return;
      }
    } else if (color[u] == 1) {
      cycle_end = v;
      cycle_start = u;
      return;
    }
  }
  color[v] = 2;
}

void FindCycle(Matrix<int>& adj_list) {
  std::vector<int> color(adj_list.size(), 0);
  std::vector<int> parent(adj_list.size(), -1);
  int cycle_start = -1;
  int cycle_end = -1;

  for (int v = 1; v < adj_list.size(); v++) {
    if (color[v] == 0 && cycle_end == -1) {
      FindCycleDFS(v, adj_list, color, parent, cycle_start, cycle_end);
    }
  }

  if (cycle_end == -1) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";
    std::vector<int> cycle;
    cycle.push_back(cycle_start);
    for (int v = cycle_end; v != cycle_start; v = parent[v]) {
      cycle.push_back(v);
    }
    cycle.push_back(cycle_start);
    std::reverse(cycle.begin(), cycle.end());
    for (int i = 0; i < cycle.size() - 1; i++) {
      std::cout << cycle[i] << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  Matrix<int> adj_list = MakeAdjastencyList(n, m);

  FindCycle(adj_list);
}
