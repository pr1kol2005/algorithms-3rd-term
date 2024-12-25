#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using PairMatrix = std::vector<std::vector<std::pair<int, int>>>;
using PriorityQueue =
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>;

const int kInf = std::numeric_limits<int>::max();

PairMatrix MakeAdjacencyList(int vertice_num, int edge_num) {
  PairMatrix adjacency_list(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    int w = 0;
    std::cin >> u >> v >> w;
    adjacency_list[u].emplace_back(v, w);
    adjacency_list[v].emplace_back(u, w);
  }
  return adjacency_list;
}

int Prim(const PairMatrix& adj_list) {
  std::vector<int> min_edge(adj_list.size(), kInf);
  std::vector<bool> in_mst(adj_list.size(), false);
  PriorityQueue pq;

  min_edge[1] = 0;
  pq.emplace(0, 1);

  int resut = 0;

  while (!pq.empty()) {
    int weight = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (in_mst[u]) {
      continue;
    }

    in_mst[u] = true;
    resut += weight;

    for (const auto& edge : adj_list[u]) {
      int v = edge.first;
      int w = edge.second;

      if (!in_mst[v] && w < min_edge[v]) {
        min_edge[v] = w;
        pq.emplace(w, v);
      }
    }
  }

  return resut;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  PairMatrix adj_list = MakeAdjacencyList(n, m);

  std::cout << Prim(adj_list) << '\n';

  return 0;
}
