#include <iostream>
#include <limits>
#include <vector>

using PairMatrix = std::vector<std::vector<std::pair<int, int>>>;

const int kInf = 30000;

PairMatrix MakeAdjacencyList(int n, int m) {
  PairMatrix adj_list(n + 1);
  for (size_t i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    int w = 0;
    std::cin >> a >> b >> w;
    adj_list[a].emplace_back(b, w);
  }
  return adj_list;
}

void BellmanFord(int n, std::vector<std::vector<std::pair<int, int>>>& adj_list,
                 std::vector<int>& d) {
  d[1] = 0;
  for (int i = 0; i < n; ++i) {
    for (int u = 1; u <= n; u++) {
      for (auto& edge : adj_list[u]) {
        if (d[u] < kInf && d[u] + edge.second < d[edge.first]) {
          d[edge.first] = d[u] + edge.second;
        }
      }
    }
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  PairMatrix adj_list = MakeAdjacencyList(n, m);

  std::vector<int> d(n + 1, kInf);
  BellmanFord(n, adj_list, d);

  for (int i = 1; i <= n; i++) {
    std::cout << d[i] << ' ';
  }
  std::cout << '\n';

  return 0;
}
