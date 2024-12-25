#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

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

const int64_t kInf = std::numeric_limits<int64_t>::max();

void AddEdge(int64_t from, int64_t to, int64_t capacity, EdgeVector& edges, Matrix<int64_t>& adj_list) {
  Edge edge(edges.size(), from, to, capacity, 0);
  adj_list[from].push_back(edge.index);
  edges.push_back(edge);

  Edge reverse_edge(edges.size(), to, from, 0, 0);
  adj_list[to].push_back(reverse_edge.index);
  edges.push_back(reverse_edge);
}

Matrix<int64_t> MakeAdjacencyList(int64_t n, int64_t m, EdgeVector& edges) {
  Matrix<int64_t> adj_list(n + 1);
  for (int64_t i = 0; i < m; ++i) {
    int64_t a = 0;
    int64_t b = 0;
    int64_t с = 0;
    std::cin >> a >> b >> с;
    AddEdge(a, b, с, edges, adj_list);
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

int main() {
  int64_t n = 0;
  int64_t m = 0;
  std::cin >> n >> m;

  EdgeVector edges;

  Matrix<int64_t> adj_list = MakeAdjacencyList(n, m, edges);

  std::cout << Dinitz(1, n, edges, adj_list, n) << '\n';

  for (std::size_t i = 0; i < edges.size(); ++i) {
    if (i % 2 == 0) {
      std::cout << edges[i].flow << '\n';
    }
  }

  return 0;
}
