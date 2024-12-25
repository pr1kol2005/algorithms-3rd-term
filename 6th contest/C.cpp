#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using PairMatrix = Matrix<std::pair<int, int>>;

const int kInf = std::numeric_limits<int>::max();

PairMatrix MakeAdjacencyList(int n, int m) {
  PairMatrix adj_list(n + 1);
  for (int i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    int с = 0;
    std::cin >> a >> b >> с;
    adj_list[a].emplace_back(b, с);
  }
  return adj_list;
}

bool BFS(Matrix<int>& residual_graph, int start, int t, std::vector<int>& parent) {
  int n = residual_graph.size();
  std::vector<bool> visited(n, false);
  std::queue<int> q;

  q.push(start);
  visited[start] = true;
  parent[start] = -1;

  while (!q.empty()) {
    int current = q.front();
    q.pop();

    for (int next = 0; next < n; ++next) {
      if (!visited[next] && residual_graph[current][next] > 0) {
        if (next == t) {
          parent[next] = current;
          return true;
        }
        q.push(next);
        parent[next] = current;
        visited[next] = true;
      }
    }
  }
  return false;
}

int FordFulkerson(PairMatrix& adj_list, int start, int t) {
  int n = static_cast<int>(adj_list.size());
  Matrix<int> residual_graph(n, std::vector<int>(n, 0));

  for (int u = 1; u < n; ++u) {
    for (const auto& edge : adj_list[u]) {
      int v = edge.first;
      int capacity = edge.second;
      residual_graph[u][v] = capacity;
    }
  }

  std::vector<int> parent(n);
  int max_flow = 0;

  while (BFS(residual_graph, start, t, parent)) {
    int path_flow = kInf;

    for (int v = t; v != start; v = parent[v]) {
      int u = parent[v];
      path_flow = std::min(path_flow, residual_graph[u][v]);
    }

    for (int v = t; v != start; v = parent[v]) {
      int u = parent[v];
      residual_graph[u][v] -= path_flow;
      residual_graph[v][u] += path_flow;
    }

    max_flow += path_flow;
  }

  return max_flow;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  auto adj_list = MakeAdjacencyList(n, m);

  std::cout << FordFulkerson(adj_list, 1, n) << '\n';

  return 0;
}
