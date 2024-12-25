#include <algorithm>
#include <iostream>
#include <vector>

using PairMatrix = std::vector<std::vector<std::pair<int, int>>>;

PairMatrix MakeAdjacencyList(int n) {
  PairMatrix adj_list(n);
  for (int u = 0; u < n; u++) {
    for (int last = 0; last < n; last++) {
      int w = 0;
      std::cin >> w;
      if (w != 100000) {
        adj_list[u].emplace_back(last, w);
      }
    }
  }
  return adj_list;
}

bool BellmanFord(int n, PairMatrix& adj_list, std::vector<int>& answer) {
  std::vector<int> dist(n, 0);
  std::vector<int> prev(n, -1);

  int last = -1;
  for (int i = 0; i < n; ++i) {
    last = -1;
    for (int u = 0; u < n; u++) {
      for (const auto& edge : adj_list[u]) {
        if (dist[u] + edge.second < dist[edge.first]) {
          dist[edge.first] = dist[u] + edge.second;
          prev[edge.first] = u;
          last = edge.first;
        }
      }
    }
  }

  if (last == -1) {
    return true;
  }

  for (int i = 0; i < n; i++) {
    last = prev[last];
  }
  for (int v = last;; v = prev[v]) {
    answer.push_back(v + 1);
    if (v == last && answer.size() > 1) {
      break;
    }
  }
  std::reverse(answer.begin(), answer.end());

  return answer.empty();
}

int main() {
  int n = 0;
  std::cin >> n;

  PairMatrix adj_list = MakeAdjacencyList(n);

  std::vector<int> answer;

  if (!BellmanFord(n, adj_list, answer)) {
    std::cout << "YES\n" << answer.size() << '\n';
    for (int u : answer) {
      std::cout << u << ' ';
    }
    std::cout << '\n';
  } else {
    std::cout << "NO\n";
  }
}
