#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>

using PairMatrix = std::vector<std::vector<std::pair<int, int>>>;
using PriorityQueue =
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>;

const int kInf = 2009000999;

PairMatrix MakeAdjacencyList(int n, int m) {
  std::vector<std::vector<std::pair<int, int>>> adj_list(n + 1);
  for (int i = 0; i < m; i++) {
    int a = 0;
    int b = 0;
    int t = 0;
    std::cin >> a >> b >> t;
    adj_list[a + 1].emplace_back(b + 1, t);
    adj_list[b + 1].emplace_back(a + 1, t);
  }
  return adj_list;
}

std::vector<int> DijkstrasAlgorithm(const PairMatrix& adj_list, int s) {
  std::vector<int> dist(adj_list.size(), kInf);
  dist[s] = 0;
  PriorityQueue pq;
  pq.emplace(0, s);

  while (!pq.empty()) {
    int d = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u]) {
      continue;
    }

    for (const auto& edge : adj_list[u]) {
      int v = edge.first;
      int weight = edge.second;
      if (d + weight < dist[v]) {
        dist[v] = d + weight;
        pq.emplace(dist[v], v);
      }
    }
  }

  return dist;
}

int main() {
  int k = 0;
  std::cin >> k;
  for (size_t i = 0; i < k; i++) {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, int>>> adj_list = MakeAdjacencyList(n, m);

    int s = 0;
    std::cin >> s;

    std::vector<int> dist = DijkstrasAlgorithm(adj_list, s + 1);

    for (int u = 1; u <= n; u++) {
      std::cout << dist[u] << " ";
    }
    std::cout << "\n";
  }

  return 0;
}
