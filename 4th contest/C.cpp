#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using PairMatrix = std::vector<std::vector<std::pair<int, int>>>;
using PriorityQueue =
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>;

const int kInf = std::numeric_limits<int>::max();

PairMatrix MakeAdjacencyList(int n) {
  PairMatrix adj_list(n + 1);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      int w = 0;
      std::cin >> w;
      if (w != 0) {
        adj_list[i + 1].emplace_back(j + 1, w);
      }
    }
  }
  for (size_t i = 0; i < n; i++) {
    int w = 0;
    std::cin >> w;
    adj_list[0].emplace_back(i + 1, w);
    adj_list[i + 1].emplace_back(0, w);
  }
  return adj_list;
}

int Prim(PairMatrix& adj_list) {
  std::vector<int> min_edge(adj_list.size(), kInf);
  std::vector<bool> in_mst(adj_list.size(), false);
  PriorityQueue pq;

  min_edge[0] = 0;
  pq.emplace(0, 0);

  int result = 0;

  while (!pq.empty()) {
    int weight = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (in_mst[u]) {
      continue;
    }

    in_mst[u] = true;
    result += weight;

    for (const auto& edge : adj_list[u]) {
      int v = edge.first;
      int w = edge.second;

      if (!in_mst[v] && w < min_edge[v]) {
        min_edge[v] = w;
        pq.emplace(w, v);
      }
    }
  }

  return result;
}

int main() {
  int n = 0;
  std::cin >> n;

  PairMatrix adj_list = MakeAdjacencyList(n);

  std::cout << Prim(adj_list) << '\n';

  return 0;
}
