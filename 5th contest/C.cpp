#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

const int kInf = std::numeric_limits<int>::max();

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using TupleMatrix = Matrix<std::tuple<int, int, int>>;
using PairMatrix = Matrix<std::pair<int, int>>;
using PairVector = std::vector<std::pair<int, int>>;
using PriorityQueue =
    std::priority_queue<std::tuple<int, int, int>,
                        std::vector<std::tuple<int, int, int>>,
                        std::greater<std::tuple<int, int, int>>>;

void MakeAdjacencyList(TupleMatrix& adj_list, std::size_t m) {
  for (std::size_t i = 0; i < m; i++) {
    int u = 0;
    int v = 0;
    int p = 0;
    int t = 0;
    std::cin >> u >> v >> p >> t;

    adj_list[u].emplace_back(v, p, t);
    adj_list[v].emplace_back(u, p, t);
  }
}

Matrix<int> ModifiedDijkstrasAlgorithm(const TupleMatrix& adj_list,
                                       PairMatrix& prev, int s, int t_max) {
  Matrix<int> dist(adj_list.size(), std::vector<int>(t_max + 1, kInf));
  dist[s][0] = 0;
  PriorityQueue pq;
  pq.emplace(0, s, 0);

  while (!pq.empty()) {
    auto [weight, u, time] = pq.top();
    pq.pop();

    if (weight > dist[u][time]) {
      continue;
    }

    for (const auto& edge : adj_list[u]) {
      auto [v, new_weight, new_time] = edge;
      new_weight += weight;
      new_time += time;
      if (new_weight < dist[v][new_time] && new_time <= t_max) {
        dist[v][new_time] = new_weight;
        prev[v][new_time] = {u, time};
        pq.emplace(new_weight, v, new_time);
      }
    }
  }

  return dist;
}

void PrintResult(const Matrix<int>& dist, const PairMatrix& prev, int t_max) {
  int min_cost = kInf;
  int best_time = -1;
  for (int t = 0; t <= t_max; ++t) {
    if (dist.back()[t] < min_cost) {
      min_cost = dist.back()[t];
      best_time = t;
    }
  }

  if (min_cost == kInf) {
    std::cout << "-1\n";
    return;
  }

  std::vector<int> path;
  int curr_v = dist.size() - 1;
  int curr_t = best_time;
  while (curr_v != -1) {
    path.push_back(curr_v);
    auto [prev_v, prev_t] = prev[curr_v][curr_t];
    curr_v = prev_v;
    curr_t = prev_t;
  }

  std::reverse(path.begin(), path.end());

  std::cout << min_cost << '\n';
  std::cout << path.size() << '\n';
  for (int v : path) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::size_t n = 0;
  std::size_t m = 0;
  int t = 0;
  std::cin >> n >> m >> t;

  TupleMatrix adj_list(n + 1);

  MakeAdjacencyList(adj_list, m);

  PairMatrix prev(adj_list.size(), PairVector(t + 1, {-1, -1}));

  Matrix<int> dist = ModifiedDijkstrasAlgorithm(adj_list, prev, 1, t);

  PrintResult(dist, prev, t);

  return 0;
}
