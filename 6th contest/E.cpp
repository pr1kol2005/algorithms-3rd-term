#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include <numeric>

struct Edge {
  int64_t index;
  int64_t from;
  int64_t to;
  int64_t capacity;
  int64_t flow;

  Edge(int64_t i, int64_t u, int64_t v, int64_t cap, int64_t fl) : index(i), from(u), to(v), capacity(cap), flow(fl) {}
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using EdgeVector = std::vector<Edge>;

using PairVector = std::vector<std::pair<int64_t, int64_t>>;

const int64_t kInf = std::numeric_limits<int64_t>::max();

void AddEdge(int64_t from, int64_t to, int64_t capacity, EdgeVector& edges, Matrix<int64_t>& adj_list) {
  Edge edge(edges.size(), from, to, capacity, 0);
  adj_list[from].push_back(edge.index);
  edges.push_back(edge);

  Edge reverse_edge(edges.size(), to, from, 0, 0);
  adj_list[to].push_back(reverse_edge.index);
  edges.push_back(reverse_edge);
}

Matrix<int64_t> MakeAdjacencyList(int64_t n, int64_t k, EdgeVector& edges, const PairVector& pairs) {
  Matrix<int64_t> adj_list(n + 1);
  for (int64_t i = 0; i < k; ++i) {
    int64_t a = pairs[i].first;
    int64_t b = pairs[i].second;
    AddEdge(a, b, kInf, edges, adj_list);
  }
  return adj_list;
}

bool BFS(int64_t s, int64_t t, EdgeVector& edges, Matrix<int64_t>& adj_list, std::vector<int64_t>& level) {
  std::fill(level.begin(), level.end(), -1);
  std::queue<int64_t> q;

  level[s] = 0;
  q.push(s);

  while (!q.empty()) {
    int64_t u = q.front();
    q.pop();

    for (int64_t i = 0; i < adj_list[u].size(); ++i) {
      int64_t edge_id = adj_list[u][i];
      int64_t v = edges[edge_id].to;
      if (level[v] < 0 && edges[edge_id].capacity - edges[edge_id].flow > 0) {
        level[v] = level[u] + 1;
        q.push(v);
      }
    }
  }

  return !(level[t] < 0);
}

int64_t DFS(int64_t u, int64_t t, int64_t flow, EdgeVector& edges, Matrix<int64_t>& adj_list,
        std::vector<int64_t>& level, std::vector<int64_t>& start) {
  if (u == t) {
    return flow;
  }

  if (flow == 0) {
    return 0;
  }

  while (start[u] < adj_list[u].size()) {
    int64_t edge_id = adj_list[u][start[u]];
    int64_t v = edges[edge_id].to;
    if (level[v] == level[u] + 1) {  // removed &&
      int64_t curr_flow = std::min(flow, edges[edge_id].capacity - edges[edge_id].flow);
      int64_t temp_flow = DFS(v, t, curr_flow, edges, adj_list, level, start);

      if (temp_flow > 0) {
        edges[edge_id].flow += temp_flow;
        edges[edge_id ^ 1].flow -= temp_flow;
        return temp_flow;
      }
    }
    ++start[u];
  }

  return 0;
}

int64_t Dinitz(int64_t s, int64_t t, EdgeVector& edges, Matrix<int64_t>& adj_list, int64_t n) {
  std::vector<int64_t> level(n + 1);
  int64_t max_flow = 0;

  while (BFS(s, t, edges, adj_list, level)) {
    std::vector<int64_t> start(n + 1, 0);
    while (int64_t flow = DFS(s, t, kInf, edges, adj_list, level, start)) {
      max_flow += flow;
    }
  }

  return max_flow;
}

bool IsPossibleToTransfer(const EdgeVector& edges, int64_t total_count, int64_t t) {
  int64_t result = 0;

  for (std::size_t i = 0; i < edges.size(); ++i) {
    if (i % 2 == 0 && edges[i].to == t) {
      result += edges[i].flow;
    }
  }

  return (result == total_count);
}

int main() {
  int64_t n = 0;
  int64_t k = 0;
  std::cin >> n >> k;

  std::vector<int64_t> init_capacity(n + 1);
  for (int64_t i = 1; i <= n; ++i) {
    std::cin >> init_capacity[i];
  }

  int64_t total_count = std::accumulate(init_capacity.begin(), init_capacity.end(), 0);

  int64_t min_capacity = *std::min_element(init_capacity.begin(), init_capacity.end());

  int64_t max_capacity = *std::max_element(init_capacity.begin(), init_capacity.end());

  PairVector pairs;
  for (int64_t i = 0; i < k; ++i) {
    int64_t a = 0;
    int64_t b = 0;
    std::cin >> a >> b;
    pairs.emplace_back(a, b);
  }

  int64_t mid = 0;
  int64_t left = min_capacity;
  int64_t right = max_capacity;

  while (left <= right) {
    mid = (left + right) / 2;

    EdgeVector edges;

    Matrix<int64_t> adj_list = MakeAdjacencyList(n, k, edges, pairs);

    int s = adj_list.size();
    int t = adj_list.size() + 1;

    adj_list.resize(adj_list.size() + 2);
    for (int i = 1; i <= n; ++i) {
      AddEdge(s, i, init_capacity[i], edges, adj_list);
    }
    for (int i = 1; i <= n; ++i) {
      AddEdge(i, t, mid, edges, adj_list);
    }

    Dinitz(s, t, edges, adj_list, adj_list.size());
    if (!IsPossibleToTransfer(edges, total_count, t)) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  std::cout << left << '\n';

  return 0;
}
