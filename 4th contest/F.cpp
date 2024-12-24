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

class Graph {
  Matrix<Edge> adj_list;
  size_t vertice_num;
  size_t edge_num;

 public:
  Graph() = default;

  Graph(size_t vertice_num, size_t edge_num = 0)
      : adj_list(vertice_num + 1),
        vertice_num(vertice_num),
        edge_num(edge_num) {}

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

  Matrix<Edge>& GetAdjacencyList() { return adj_list; }

  const Matrix<Edge>& GetAdjacencyList() const { return adj_list; }

  size_t GetVerticeNum() const { return vertice_num; }

  size_t GetEdgeNum() const { return edge_num; }
};

class DSU {
  std::vector<int> parent;
  std::vector<int> rank;
  std::vector<int> LCA_class;

 public:
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

  std::vector<int>& GetLCAClass() { return LCA_class; }
};

class LCA_Finder {
  RequestsMap result;
  DSU dsu;
  std::vector<int> color;
  const Graph& graph;
  Matrix<int>& request_list;
  std::vector<int>& dist;

 public:
  LCA_Finder(const Graph& graph, Matrix<int>& request_list,
             std::vector<int>& dist)
      : graph(graph),
        request_list(request_list),
        dsu(graph.GetVerticeNum()),
        result(),
        color(graph.GetVerticeNum() + 1, 0),
        dist(dist) {}

  void Find_LCA_DFS(int v) {
    color[v] = 1;
    dsu.GetLCAClass()[v] = v;
    for (const auto& edge : graph.GetAdjacencyList()[v]) {
      if (color[edge.to] == 0) {
        Find_LCA_DFS(edge.to);
        dsu.UniteLCA(v, edge.to, v);
      }
    }

    for (int u : request_list[v]) {
      if (color[u] != 0) {
        int w = dsu.GetLCA(u);
        result.emplace(Request(v, u), dist[w]);
        result.emplace(Request(u, v), dist[w]);
      }
    }

    color[v] = 2;
  }

  RequestsMap& GetResult() { return result; }

  const RequestsMap& GetResult() const { return result; }
};

class BridgeFinder {
  int dfs_timer;
  const Graph& graph;
  std::vector<int> color;
  std::vector<int> time_in;
  std::vector<int> time_up;
  std::vector<bool> is_bridge;

 public:
  BridgeFinder(const Graph& graph)
      : graph(graph),
        color(graph.GetVerticeNum() + 1, 0),
        time_in(graph.GetVerticeNum() + 1, -1),
        time_up(graph.GetVerticeNum() + 1, -1),
        is_bridge(graph.GetEdgeNum() + 1, false),
        dfs_timer(0) {}

  void FindBridgesDFS(int v, int current_edge = -1) {
    color[v] = 1;
    time_in[v] = time_up[v] = dfs_timer++;

    for (const auto& edge : graph.GetAdjacencyList()[v]) {
      if (edge.id != current_edge) {
        if (color[edge.to] == 1) {
          time_up[v] = std::min(time_up[v], time_in[edge.to]);
        } else if (color[edge.to] == 0) {
          FindBridgesDFS(edge.to, edge.id);
          time_up[v] = std::min(time_up[v], time_up[edge.to]);
          if (time_up[edge.to] > time_in[v]) {
            is_bridge[edge.id] = true;
          }
        }
      }
    }

    color[v] = 2;
  }

  void FindBridges() {
    for (size_t u = 1; u <= graph.GetVerticeNum(); ++u) {
      if (color[u] == 0) {
        FindBridgesDFS(u);
      }
    }
  }

  std::vector<bool>& GetIsBridge() { return is_bridge; }

  const std::vector<bool>& GetIsBridge() const { return is_bridge; }
};

class ComponentFinder {
  int component_count;
  const Graph& graph;
  std::vector<bool>& is_bridge;
  std::vector<int> color;
  std::vector<int> component;

 public:
  ComponentFinder(const Graph& graph, std::vector<bool>& is_bridge)
      : graph(graph),
        color(graph.GetVerticeNum() + 1),
        component(graph.GetVerticeNum() + 1),
        is_bridge(is_bridge),
        component_count(0) {}

  void MarkingComponentDFS(int v) {
    color[v] = 1;
    component[v] = component_count;

    for (const auto& edge : graph.GetAdjacencyList()[v]) {
      if (color[edge.to] == 0 && !is_bridge[edge.id]) {
        MarkingComponentDFS(edge.to);
      }
    }

    color[v] = 2;
  }

  std::vector<int> FindStronglyConnectedComponents() {
    for (size_t u = 1; u <= graph.GetVerticeNum(); u++) {
      if (color[u] == 0) {
        component_count++;
        MarkingComponentDFS(u);
      }
    }

    return component;
  }

  Graph MakeCondensate() {
    FindStronglyConnectedComponents();

    Graph condensate(component_count);
    for (int u = 1; u <= graph.GetVerticeNum(); u++) {
      for (const auto& edge : graph.GetAdjacencyList()[u]) {
        if (component[u] != component[edge.to]) {
          condensate.AddEdge(component[u], component[edge.to], 1, edge.id);
        }
      }
    }
    return condensate;
  }

  std::vector<int>& GetComponent() { return component; }

  const std::vector<int>& GetComponent() const { return component; }
};

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

std::vector<int> CalculateDistancesFromVertice(const Graph& graph, int s) {
  std::vector<int> dist(graph.GetAdjacencyList().size(), kInf);
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

    for (const auto& edge : graph.GetAdjacencyList()[u]) {
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

void ProcessRequests(const Graph& init_graph, int finish_vertice) {
  size_t n = init_graph.GetVerticeNum();
  size_t m = init_graph.GetEdgeNum();
  BridgeFinder bridge_finder(init_graph);

  bridge_finder.FindBridges();

  std::vector<bool> is_bridge = bridge_finder.GetIsBridge();

  ComponentFinder component_finder(init_graph, is_bridge);

  Graph condensate = component_finder.MakeCondensate();

  std::vector<int> component = component_finder.GetComponent();

  size_t q = 0;
  std::cin >> q;
  std::vector<Request> output_order;
  Matrix<int> request_list = MakeRequestsList(n, q, output_order, component);

  std::vector<int> dist =
      CalculateDistancesFromVertice(condensate, component[finish_vertice]);

  LCA_Finder lca_finder(condensate, request_list, dist);

  lca_finder.Find_LCA_DFS(component[finish_vertice]);

  RequestsMap result = lca_finder.GetResult();

  for (const auto& request : output_order) {
    std::cout << result[request] << '\n';
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  int finish_vertice = 0;
  std::cin >> finish_vertice;

  Graph init_graph(n, m);
  init_graph.ReadAdjacencyList(n, m);

  ProcessRequests(init_graph, finish_vertice);

  return 0;
}