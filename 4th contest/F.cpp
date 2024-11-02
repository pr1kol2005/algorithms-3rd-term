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

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using PairVector = std::vector<std::pair<int, int>>;
using PriorityQueue = std::priority_queue<std::pair<int, int>, PairVector,
                                          std::greater<std::pair<int, int>>>;
using Map = std::unordered_map<std::pair<int, int>, int, pair_hash>;
using PairMatrix = Matrix<std::pair<int, int>>;

const int kInf = std::numeric_limits<int>::max();

int dfs_timer = 0;

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

void FindLCADFS(int v, Matrix<int>& adj_list, Matrix<int>& request_list,
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
      result.emplace(std::make_pair(v, u), dist[w]);
      result.emplace(std::make_pair(u, v), dist[w]);
    }
  }

  color[v] = 2;
}

Matrix<int> MakeRequestsList(size_t vertice_num, size_t edge_num,
                             PairVector& output_order,
                             std::vector<int>& component) {
  Matrix<int> adj_list(vertice_num + 1);
  for (size_t i = 0; i < edge_num; ++i) {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    a = component[a];
    b = component[b];
    adj_list[a].emplace_back(b);
    adj_list[b].emplace_back(a);
    output_order.emplace_back(a, b);
  }
  return adj_list;
}

PairMatrix MakeAdjacencyList(int vertice_num, int edge_num) {
  PairMatrix adj_list(vertice_num + 1);
  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    adj_list[u].emplace_back(v, i + 1);
    adj_list[v].emplace_back(u, i + 1);
  }
  return adj_list;
}

void DFS(int v, PairMatrix& adj_list, std::vector<int>& color,
         std::vector<int>& time_in, std::vector<int>& time_up,
         std::vector<bool>& is_bridge, int current_edge = -1) {
  color[v] = 1;
  time_in[v] = time_up[v] = dfs_timer++;

  for (const auto& u : adj_list[v]) {
    if (u.second != current_edge) {
      if (color[u.first] == 1) {
        time_up[v] = std::min(time_up[v], time_in[u.first]);
      } else if (color[u.first] == 0) {
        DFS(u.first, adj_list, color, time_in, time_up, is_bridge, u.second);
        time_up[v] = std::min(time_up[v], time_up[u.first]);
        if (time_up[u.first] > time_in[v]) {
          is_bridge[u.second] = true;
        }
      }
    }
  }

  color[v] = 2;
}

void MarkingComponentDFS(int v, PairMatrix& graph, std::vector<int>& color,
                         std::vector<int>& component, int component_count,
                         std::vector<bool>& is_bridge) {
  color[v] = 1;
  component[v] = component_count;

  for (const auto& u : graph[v]) {
    if (color[u.first] == 0 && !is_bridge[u.second]) {
      MarkingComponentDFS(u.first, graph, color, component, component_count,
                          is_bridge);
    }
  }

  color[v] = 2;
}

std::vector<int> FindStronglyConnectedComponents(int n, PairMatrix& graph,
                                                 std::vector<bool>& is_bridge,
                                                 int& component_count) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> component(n + 1, 0);

  for (size_t i = 1; i <= n; i++) {
    if (color[i] == 0) {
      component_count++;
      MarkingComponentDFS(i, graph, color, component, component_count,
                          is_bridge);
    }
  }

  return component;
}

Matrix<int> MakeCondensate(int vertice_num, std::vector<int>& component,
                           PairMatrix& adj_list, int component_count) {
  Matrix<int> condensate(component_count + 1);
  for (int u = 1; u <= vertice_num; u++) {
    for (const auto& v : adj_list[u]) {
      if (component[u] != component[v.first]) {
        condensate[component[u]].push_back(component[v.first]);
      }
    }
  }
  return condensate;
}

std::vector<int> DijkstrasAlgorithm(const Matrix<int>& adj_list, int s) {
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

void ProcessRequests(int n, int m, PairMatrix& adj_list, int f) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> time_in(n + 1, -1);
  std::vector<int> time_up(n + 1, -1);
  std::vector<bool> is_bridge(m + 1, false);
  int component_count = 0;

  for (size_t i = 1; i <= n; ++i) {
    if (color[i] == 0) {
      DFS(i, adj_list, color, time_in, time_up, is_bridge);
    }
  }

  std::vector<int> component =
      FindStronglyConnectedComponents(n, adj_list, is_bridge, component_count);

  Matrix<int> condensate =
      MakeCondensate(n, component, adj_list, component_count);

  size_t q = 0;
  std::cin >> q;
  PairVector output_order;
  Matrix<int> request_list = MakeRequestsList(n, q, output_order, component);

  DSU dsu(n);
  std::vector<int> dist = DijkstrasAlgorithm(condensate, component[f]);

  std::fill(color.begin(), color.end(), 0);

  Map result;

  FindLCADFS(component[f], condensate, request_list, color, dsu, dist, result);

  for (const auto& request : output_order) {
    std::cout << result[request] << '\n';
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  int f = 0;
  std::cin >> f;
  PairMatrix adj_list = MakeAdjacencyList(n, m);

  ProcessRequests(n, m, adj_list, f);

  return 0;
}