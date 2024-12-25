#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using PairMatrix = Matrix<std::pair<int, int>>;

int dfs_timer = 0;

PairMatrix MakeAdjacencyList(int vertice_num, int edge_num) {
  PairMatrix adj_list(vertice_num + 1);
  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    adj_list[u].emplace_back(v, i + 1);
    adj_list[v].emplace_back(u, i + 1);
  }
  return adj_list;
}

void MarkingComponentDFS(int v, PairMatrix& graph, std::vector<int>& color,
                         std::vector<int>& component, int component_count,
                         std::vector<bool>& is_bridge) {
  color[v] = 1;
  component[v] = component_count;

  for (const auto& u : graph[v]) {
    if (color[u.first] == 0 && !is_bridge[u.second]) {
      MarkingComponentDFS(u.first, graph, color, component, component_count,
                          is_bridge);
    }
  }

  color[v] = 2;
}

std::vector<int> FindStronglyConnectedComponents(int n, PairMatrix& graph,
                                                 std::vector<bool>& is_bridge,
                                                 int& component_count) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> component(n + 1, 0);

  for (int i = 1; i <= n; i++) {
    if (color[i] == 0) {
      component_count++;
      MarkingComponentDFS(i, graph, color, component, component_count,
                          is_bridge);
    }
  }

  // std::cout << component_count << '\n';
  // for (int i = 1; i <= n; i++) {
  //   std::cout << component[i] << " ";
  // }
  // std::cout << '\n';

  return component;
}

void DFS(int v, PairMatrix& adj_list, std::vector<int>& color,
         std::vector<int>& time_in, std::vector<int>& time_up,
         std::vector<bool>& is_bridge, int current_edge = -1) {
  color[v] = 1;
  time_in[v] = time_up[v] = dfs_timer++;

  for (const auto& u : adj_list[v]) {
    if (u.second != current_edge) {
      if (color[u.first] == 1) {
        time_up[v] = std::min(time_up[v], time_in[u.first]);
      } else if (color[u.first] == 0) {
        DFS(u.first, adj_list, color, time_in, time_up, is_bridge, u.second);
        time_up[v] = std::min(time_up[v], time_up[u.first]);
        if (time_up[u.first] > time_in[v]) {
          is_bridge[u.second] = true;
        }
      }
    }
  }

  color[v] = 2;
}

std::vector<int> MakeCondensate(int vertex_number, std::vector<int>& component,
                                PairMatrix& adj_list, int component_count) {
  std::vector<int> condensate(component_count + 1);
  for (int u = 1; u <= vertex_number; u++) {
    for (const auto& edge : adj_list[u]) {
      if (component[u] != component[edge.first]) {
        condensate[component[u]]++;
      }
    }
  }
  return condensate;
}

void FindBridges(int n, int m, PairMatrix& adj_list) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> time_in(n + 1, -1);
  std::vector<int> time_up(n + 1, -1);
  std::vector<bool> is_bridge(m + 1, false);
  std::vector<int> bridges;
  int component_count = 0;

  for (int i = 1; i <= n; ++i) {
    if (color[i] == 0) {
      DFS(i, adj_list, color, time_in, time_up, is_bridge);
    }
  }

  for (int i = 1; i <= m; i++) {
    if (is_bridge[i]) {
      bridges.push_back(i);
    }
  }

  // std::sort(bridges.begin(), bridges.end());

  // std::cout << bridges.size() << '\n';
  // for (int u : bridges) {
  //   std::cout << u << '\n';
  // }

  std::vector<int> component =
      FindStronglyConnectedComponents(n, adj_list, is_bridge, component_count);

  std::vector<int> condensate =
      MakeCondensate(n, component, adj_list, component_count);

  for (auto i : condensate) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  int result = 0;
  for (const auto& i : condensate) {
    if (i == 1) {
      result++;
    }
  }

  std::cout << (result + 1) / 2 << '\n';
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  PairMatrix adj_list = MakeAdjacencyList(n, m);

  FindBridges(n, m, adj_list);

  return 0;
}
