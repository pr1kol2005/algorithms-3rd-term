#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

int dfs_timer = 0;

Matrix<int> MakeAdjacencyList(int vertex_number, int edge_number) {
  Matrix<int> adj_list(vertex_number + 1);
  for (int i = 0; i < edge_number; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    adj_list[u].push_back(v);
    adj_list[v].push_back(u);
  }
  return adj_list;
}

void DFS(int v, Matrix<int>& adj_list, std::vector<int>& color,
         std::vector<int>& time_in, std::vector<int>& time_up,
         std::vector<bool>& is_articulation, int parent = -1) {
  color[v] = 1;
  time_in[v] = time_up[v] = dfs_timer++;
  int children_count = 0;

  for (int u : adj_list[v]) {
    if (color[u] == 1) {
      time_up[v] = std::min(time_up[v], time_in[u]);
    } else if (color[u] == 0) {
      children_count++;
      DFS(u, adj_list, color, time_in, time_up, is_articulation, v);
      time_up[v] = std::min(time_up[v], time_up[u]);
      if (time_up[u] >= time_in[v] && parent != -1) {
        is_articulation[v] = true;
      }
    }
  }

  if (parent == -1 && children_count > 1) {
    is_articulation[v] = true;
  }

  color[v] = 2;
}

void FindArticulationPoints(int n, Matrix<int>& adj_list) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> time_in(n + 1, -1);
  std::vector<int> time_up(n + 1, -1);
  std::vector<bool> is_articulation(n + 1, false);
  std::vector<int> articulation_points;

  for (int i = 1; i <= n; ++i) {
    if (color[i] == 0) {
      DFS(i, adj_list, color, time_in, time_up, is_articulation);
    }
  }

  for (int i = 1; i <= n; i++) {
    if (is_articulation[i]) {
      articulation_points.push_back(i);
    }
  }

  std::cout << articulation_points.size() << '\n';
  for (int u : articulation_points) {
    std::cout << u << '\n';
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  Matrix<int> adj_list = MakeAdjacencyList(n, m);

  FindArticulationPoints(n, adj_list);
}
