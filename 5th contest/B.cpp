#include <iostream>
#include <limits>
#include <queue>
#include <vector>

const int64_t kInf = std::numeric_limits<int64_t>::max();

using PairMatrix = std::vector<std::vector<std::pair<int64_t, int64_t>>>;
using PriorityQueue =
    std::priority_queue<std::pair<int64_t, int64_t>,
                        std::vector<std::pair<int64_t, int64_t>>,
                        std::greater<std::pair<int64_t, int64_t>>>;

PairMatrix MakeAdjacencyList(int64_t n, int64_t m) {
  std::vector<std::vector<std::pair<int64_t, int64_t>>> adj_list(n + 1);
  for (int64_t i = 0; i < m; i++) {
    int64_t a = 0;
    int64_t b = 0;
    int64_t l = 0;
    std::cin >> a >> b >> l;
    adj_list[a].emplace_back(b, l);
    adj_list[b].emplace_back(a, l);
  }
  return adj_list;
}

std::vector<int64_t> InputVirusSources(int64_t k) {
  std::vector<int64_t> virus_sources(k);
  for (int64_t i = 0; i < k; i++) {
    std::cin >> virus_sources[i];
  }
  return virus_sources;
}

std::vector<int64_t> DijkstrasAlgorithmWithInfection(
    const PairMatrix& adj_list, std::vector<int64_t> virus_sources) {
  std::vector<int64_t> dist(adj_list.size(), kInf);
  PriorityQueue pq;
  for (int64_t s : virus_sources) {
    dist[s] = 0;
    pq.emplace(0, s);
  }

  while (!pq.empty()) {
    int64_t curr_dist = pq.top().first;
    int64_t u = pq.top().second;
    pq.pop();

    if (curr_dist > dist[u]) {
      continue;
    }

    for (const auto& [v, weight] : adj_list[u]) {
      int64_t new_weight = weight + curr_dist;
      if (new_weight < dist[v]) {
        dist[v] = new_weight;
        pq.emplace(new_weight, v);
      }
    }
  }

  return dist;
}

std::vector<int64_t> DijkstrasAlgorithm(const PairMatrix& adj_list, int64_t s) {
  std::vector<int64_t> dist(adj_list.size(), kInf);
  dist[s] = 0;
  PriorityQueue pq;
  pq.emplace(0, s);

  while (!pq.empty()) {
    int64_t curr_dist = pq.top().first;
    int64_t u = pq.top().second;
    pq.pop();

    if (curr_dist > dist[u]) {
      continue;
    }

    for (const auto& [v, weight] : adj_list[u]) {
      int64_t new_weight = weight + curr_dist;
      if (new_weight < dist[v]) {
        dist[v] = new_weight;
        pq.emplace(new_weight, v);
      }
    }
  }

  return dist;
}

int main() {
  int64_t n = 0;
  int64_t m = 0;
  int64_t k = 0;
  std::cin >> n >> m >> k;

  std::vector<int64_t> virus_sources = InputVirusSources(k);

  PairMatrix adj_list = MakeAdjacencyList(n, m);

  int64_t s = 0;
  int64_t t = 0;
  std::cin >> s >> t;

  std::vector<int64_t> infection_time =
      DijkstrasAlgorithmWithInfection(adj_list, virus_sources);

  std::vector<int64_t> crewmate_time = DijkstrasAlgorithm(adj_list, s);

  if ((crewmate_time[t] >= infection_time[t] && infection_time[t] != kInf) ||
      crewmate_time[t] == kInf) {
    std::cout << "-1\n";
  } else {
    std::cout << crewmate_time[t] << '\n';
  }

  return 0;
}
