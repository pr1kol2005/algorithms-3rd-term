#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

struct pair_hash {
  std::size_t operator()(const std::pair<int, int>& p) const {
    auto h1 = std::hash<int>{}(p.first);
    auto h2 = std::hash<int>{}(p.second);
    return h1 ^ h2;
  }
};

using Matrix = std::vector<std::vector<int>>;
using PriorityQueue =
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>;
using Map = std::unordered_map<std::pair<int, int>, int, pair_hash>;
using PairVector = std::vector<std::pair<int, int>>;

const int kInf = std::numeric_limits<int>::max();

struct DSU {
  std::vector<int> parent;
  std::vector<int> rank;
  std::vector<int> LCA_class;

  explicit DSU(int size)
      : parent(size + 1), rank(size + 1, 1), LCA_class(size + 1) {
    for (int i = 1; i <= size; ++i) {
      parent[i] = i;
    }
  }

  int FindSet(int v) {
    if (v == parent[v]) {
      return parent[v];
    }
    return parent[v] = FindSet(parent[v]);
  }

  void Unite(int u, int v) {
    u = FindSet(u);
    v = FindSet(v);

    if (u != v) {
      if (rank[u] > rank[v]) {
        parent[v] = u;
      } else if (rank[u] < rank[v]) {
        parent[u] = v;
      } else {
        parent[v] = u;
        ++rank[u];
      }
    }
  }

  int& GetLCA(int u) { return LCA_class[FindSet(u)]; }

  void UniteLCA(int u, int v, int new_lca) {
    Unite(u, v);
    GetLCA(u) = new_lca;
  }
};

void FindLCADFS(int v, Matrix& adj_list, Matrix& request_list,
                std::vector<int>& color, DSU& dsu, std::vector<int>& dist,
                Map& result) {
  color[v] = 1;
  dsu.LCA_class[v] = v;
  for (int u : adj_list[v]) {
    if (color[u] == 0) {
      FindLCADFS(u, adj_list, request_list, color, dsu, dist, result);
      dsu.UniteLCA(v, u, v);
    }
  }

  for (int u : request_list[v]) {
    if (color[u] != 0) {
      int w = dsu.GetLCA(u);
      // std::cout << v << ' ' << u << ' ' << w << '\n';
      // std::cout << dist[u] + dist[v] - 2 * dist[w] << '\n';
      result.emplace(std::make_pair(v, u), dist[u] + dist[v] - 2 * dist[w]);
      result.emplace(std::make_pair(u, v), dist[u] + dist[v] - 2 * dist[w]);
    }
  }

  color[v] = 2;
}

Matrix MakeAdjacencyList(size_t vertice_num, size_t edge_num) {
  Matrix adj_list(vertice_num + 1);
  for (size_t i = 0; i < edge_num; ++i) {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    adj_list[a].emplace_back(b);
    adj_list[b].emplace_back(a);
  }
  return adj_list;
}

Matrix MakeRequestsList(size_t vertice_num, size_t edge_num,
                        PairVector& output_order) {
  Matrix adj_list(vertice_num + 1);
  for (size_t i = 0; i < edge_num; ++i) {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    adj_list[a].emplace_back(b);
    adj_list[b].emplace_back(a);
    output_order.emplace_back(a, b);
  }
  return adj_list;
}

std::vector<int> DijkstrasAlgorithm(const Matrix& adj_list, int s) {
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
      int v = edge;
      int weight = 1;
      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pq.emplace(dist[v], v);
      }
    }
  }

  return dist;
}

int main() {
  size_t n = 0;
  size_t m = 0;
  std::cin >> n >> m;
  size_t f = 0;
  std::cin >> f;
  Matrix adj_list = MakeAdjacencyList(n, m);

  size_t q = 0;
  std::cin >> q;
  std::vector<std::pair<int, int>> output_order;
  Matrix request_list = MakeRequestsList(n, q, output_order);

  DSU dsu(n);
  std::vector<int> dist = DijkstrasAlgorithm(adj_list, 1);

  std::vector<int> color(n + 1, 0);

  Map result;

  FindLCADFS(1, adj_list, request_list, color, dsu, dist, result);

  for (const auto& request : output_order) {
    std::cout << result[request] << '\n';
  }

  return 0;
}
