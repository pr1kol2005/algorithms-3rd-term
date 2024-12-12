#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

struct request_hash;
struct Edge;
struct Graph;
struct DSU;
struct LCA_Finder;
struct BridgeFinder;
struct ComponentFinder;
struct DijkstrasVerticeStatus;
struct Request;

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using DijkstrasPriorityQueue =
    std::priority_queue<DijkstrasVerticeStatus,
                        std::vector<DijkstrasVerticeStatus>,
                        std::greater<DijkstrasVerticeStatus>>;
using RequestsMap = std::unordered_map<Request, int, request_hash>;

const int kInf = std::numeric_limits<int>::max();

struct DijkstrasVerticeStatus {
  int dist;
  int vertice;

  DijkstrasVerticeStatus(int d, int v) : dist(d), vertice(v) {}

  bool operator>(const DijkstrasVerticeStatus& other) const {
    return dist > other.dist;
  }
};

struct Request {
  int blue_start;
  int orange_start;

  Request(int b, int o) : blue_start(b), orange_start(o) {}

  bool operator==(const Request& other) const {
    return blue_start == other.blue_start && orange_start == other.orange_start;
  }
};

struct request_hash {
  std::size_t operator()(const Request& req) const {
    auto h1 = std::hash<int>{}(req.blue_start);
    auto h2 = std::hash<int>{}(req.orange_start);
    return h1 ^ h2;
  }
};

struct Edge {
  int from;
  int to;
  int weight;
  int id;

  Edge(int u, int v, int w, int n) : from(u), to(v), weight(w), id(n) {}
};

struct Graph {
  Matrix<Edge> adj_list;

  Graph() = default;

  explicit Graph(int size) : adj_list(size + 1) {}

  void AddEdge(int from, int to, int weight, int id) {
    adj_list[from].emplace_back(from, to, weight, id);
  }

  void ReadAdjacencyList(int vertice_num, int edge_num) {
    adj_list.reserve(vertice_num + 1);
    for (int i = 0; i < edge_num; i++) {
      int u = 0;
      int v = 0;
      std::cin >> u >> v;
      AddEdge(u, v, 1, i + 1);
      AddEdge(v, u, 1, i + 1);
    }
  }
};

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

struct LCA_Finder {
  Matrix<int>& request_list;
  DSU& dsu;
  RequestsMap& result;
  std::vector<int>& color;
  std::vector<int>& dist;

  LCA_Finder(Matrix<int>& request_list, DSU& dsu, RequestsMap& result,
             std::vector<int>& color, std::vector<int>& dist)
      : request_list(request_list),
        dsu(dsu),
        result(result),
        color(color),
        dist(dist) {}
};

struct BridgeFinder {
  std::vector<int>& color;
  std::vector<int>& time_in;
  std::vector<int>& time_up;
  std::vector<bool>& is_bridge;
  int dfs_timer;

  BridgeFinder(std::vector<int>& color, std::vector<int>& time_in,
               std::vector<int>& time_up, std::vector<bool>& is_bridge)
      : color(color),
        time_in(time_in),
        time_up(time_up),
        is_bridge(is_bridge),
        dfs_timer(0) {}
};

struct ComponentFinder {
  std::vector<int>& color;
  std::vector<int>& component;
  std::vector<bool>& is_bridge;
  int& component_count;

  ComponentFinder(std::vector<int>& color, std::vector<int>& component,
                  std::vector<bool>& is_bridge, int& component_count)
      : color(color),
        component(component),
        is_bridge(is_bridge),
        component_count(component_count) {}
};

void Find_LCA_DFS(int v, Graph& graph, LCA_Finder& lca_finder) {
  lca_finder.color[v] = 1;
  lca_finder.dsu.LCA_class[v] = v;
  for (const auto& edge : graph.adj_list[v]) {
    if (lca_finder.color[edge.to] == 0) {
      Find_LCA_DFS(edge.to, graph, lca_finder);
      lca_finder.dsu.UniteLCA(v, edge.to, v);
    }
  }

  for (int u : lca_finder.request_list[v]) {
    if (lca_finder.color[u] != 0) {
      int w = lca_finder.dsu.GetLCA(u);
      lca_finder.result.emplace(Request(v, u), lca_finder.dist[w]);
      lca_finder.result.emplace(Request(u, v), lca_finder.dist[w]);
    }
  }

  lca_finder.color[v] = 2;
}

Matrix<int> MakeRequestsList(size_t vertice_num, size_t edge_num,
                             std::vector<Request>& output_order,
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

void DFS(int v, Graph& graph, BridgeFinder& bridge_finder,
         int current_edge = -1) {
  bridge_finder.color[v] = 1;
  bridge_finder.time_in[v] = bridge_finder.time_up[v] =
      bridge_finder.dfs_timer++;

  for (const auto& edge : graph.adj_list[v]) {
    if (edge.id != current_edge) {
      if (bridge_finder.color[edge.to] == 1) {
        bridge_finder.time_up[v] =
            std::min(bridge_finder.time_up[v], bridge_finder.time_in[edge.to]);
      } else if (bridge_finder.color[edge.to] == 0) {
        DFS(edge.to, graph, bridge_finder, edge.id);
        bridge_finder.time_up[v] =
            std::min(bridge_finder.time_up[v], bridge_finder.time_up[edge.to]);
        if (bridge_finder.time_up[edge.to] > bridge_finder.time_in[v]) {
          bridge_finder.is_bridge[edge.id] = true;
        }
      }
    }
  }

  bridge_finder.color[v] = 2;
}

void MarkingComponentDFS(int v, Graph& graph,
                         ComponentFinder& component_finder) {
  component_finder.color[v] = 1;
  component_finder.component[v] = component_finder.component_count;

  for (const auto& edge : graph.adj_list[v]) {
    if (component_finder.color[edge.to] == 0 &&
        !component_finder.is_bridge[edge.id]) {
      MarkingComponentDFS(edge.to, graph, component_finder);
    }
  }

  component_finder.color[v] = 2;
}

std::vector<int> FindStronglyConnectedComponents(int n, Graph& graph,
                                                 std::vector<bool>& is_bridge,
                                                 int& component_count) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> component(n + 1, 0);
  ComponentFinder component_finder(color, component, is_bridge,
                                   component_count);

  for (size_t u = 1; u <= n; u++) {
    if (color[u] == 0) {
      component_finder.component_count++;
      MarkingComponentDFS(u, graph, component_finder);
    }
  }

  return component;
}

Graph MakeCondensate(int vertice_num, std::vector<int>& component, Graph& graph,
                     int component_count) {
  Graph condensate(component_count);
  for (int u = 1; u <= vertice_num; u++) {
    for (const auto& edge : graph.adj_list[u]) {
      if (component[u] != component[edge.to]) {
        condensate.AddEdge(component[u], component[edge.to], 1, edge.id);
      }
    }
  }
  return condensate;
}

std::vector<int> DijkstrasAlgorithm(const Graph& graph, int s) {
  std::vector<int> dist(graph.adj_list.size(), kInf);
  dist[s] = 0;
  DijkstrasPriorityQueue pq;
  pq.emplace(0, s);

  while (!pq.empty()) {
    int d = pq.top().dist;
    int u = pq.top().vertice;
    pq.pop();

    if (d > dist[u]) {
      continue;
    }

    for (const auto& edge : graph.adj_list[u]) {
      int v = edge.to;
      int weight = edge.weight;
      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pq.emplace(dist[v], v);
      }
    }
  }

  return dist;
}

void ProcessRequests(int n, int m, Graph& init_graph, int f) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> time_in(n + 1, -1);
  std::vector<int> time_up(n + 1, -1);
  std::vector<bool> is_bridge(m + 1, false);
  int component_count = 0;
  BridgeFinder bridge_finder(color, time_in, time_up, is_bridge);

  for (size_t u = 1; u <= n; ++u) {
    if (color[u] == 0) {
      DFS(u, init_graph, bridge_finder);
    }
  }

  std::vector<int> component = FindStronglyConnectedComponents(
      n, init_graph, is_bridge, component_count);

  Graph condensate = MakeCondensate(n, component, init_graph, component_count);

  size_t q = 0;
  std::cin >> q;
  std::vector<Request> output_order;
  Matrix<int> request_list = MakeRequestsList(n, q, output_order, component);

  DSU dsu(n);
  std::vector<int> dist = DijkstrasAlgorithm(condensate, component[f]);

  std::fill(color.begin(), color.end(), 0);

  RequestsMap result;

  LCA_Finder lca_finder(request_list, dsu, result, color, dist);

  Find_LCA_DFS(component[f], condensate, lca_finder);

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
  Graph init_graph;
  init_graph.ReadAdjacencyList(n, m);

  ProcessRequests(n, m, init_graph, f);

  return 0;
}