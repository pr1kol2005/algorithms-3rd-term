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
    int64_t from = 0;
    int64_t to = 0;
    int64_t length = 0;
    std::cin >> from >> to >> length;
    adj_list[from].emplace_back(to, length);
    adj_list[to].emplace_back(from, length);
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

std::vector<int64_t> DijkstrasAlgorithmMultipleSources(
    const PairMatrix& adj_list, std::vector<int64_t> multiple_sources) {
  std::vector<int64_t> dist(adj_list.size(), kInf);
  PriorityQueue pq;
  for (int64_t s : multiple_sources) {
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

int64_t MinTimeToGetToCure(int64_t crewmate_start, int64_t cure_location,
                           const PairMatrix& adj_list,
                           const std::vector<int64_t>& virus_sources) {
  std::vector<int64_t> infection_time =
      DijkstrasAlgorithmMultipleSources(adj_list, virus_sources);

  std::vector<int64_t> crewmate_time =
      DijkstrasAlgorithmMultipleSources(adj_list, {crewmate_start});

  if ((crewmate_time[cure_location] >= infection_time[cure_location] &&
       infection_time[cure_location] != kInf) ||
      crewmate_time[cure_location] == kInf) {
    return -1;
  }
  return crewmate_time[cure_location];
}

int main() {
  int64_t n = 0;
  int64_t m = 0;
  int64_t k = 0;
  std::cin >> n >> m >> k;

  std::vector<int64_t> virus_sources = InputVirusSources(k);

  PairMatrix adj_list = MakeAdjacencyList(n, m);

  int64_t crewmate_start = 0;
  int64_t cure_location = 0;
  std::cin >> crewmate_start >> cure_location;

  std::cout << MinTimeToGetToCure(crewmate_start, cure_location, adj_list,
                                  virus_sources)
            << '\n';

  return 0;
}
