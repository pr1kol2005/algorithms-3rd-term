#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <vector>

class GraphVisitor {
 public:
  virtual ~GraphVisitor() = default;
  virtual bool DiscoverVertice(int64_t) { return true; }
  virtual bool ExamineEdge(int64_t, int64_t, int64_t) { return true; }
  virtual void FinishVertice(int64_t) {}
};

template <typename GraphType, typename VisitorType>
bool BFS(GraphType& graph, int64_t source, int64_t target,
         VisitorType& visitor) {
  std::vector<bool> visited(graph.GetAdjList().size(), false);
  std::queue<int64_t> q;
  visited[source] = true;
  if (!visitor.DiscoverVertice(source)) {
    return false;
  }
  q.push(source);
  while (!q.empty()) {
    int64_t u = q.front();
    q.pop();
    for (auto edge_id : graph.GetAdjList()[u]) {
      auto& edge = graph.GetEdges()[edge_id];
      int64_t v = edge.to;
      if (!visited[v]) {
        if (visitor.ExamineEdge(u, v, edge_id)) {
          visited[v] = true;
          if (!visitor.DiscoverVertice(v)) {
            return false;
          }
          q.push(v);
        }
      }
    }
    visitor.FinishVertice(u);
  }
  return visited[target];
}

template <typename GraphType, typename VisitorType>
int64_t DFS(GraphType& graph, int64_t u, int64_t target, int64_t flow,
            VisitorType& visitor) {
  if (u == target) {
    return flow;
  }
  if (flow == 0) {
    return 0;
  }
  while (visitor.HasNextEdge(u)) {
    int64_t edge_id = visitor.GetNextEdge(u);
    auto& edge = graph.GetEdges()[edge_id];
    int64_t v = edge.to;
    if (visitor.ExamineEdge(u, v, edge_id)) {
      int64_t capacity_left = edge.capacity - edge.flow;
      int64_t curr_flow = std::min(flow, capacity_left);
      int64_t temp_flow = DFS(graph, v, target, curr_flow, visitor);
      if (temp_flow > 0) {
        edge.flow += temp_flow;
        int64_t reverse_edge_id = edge_id ^ 1;
        graph.GetEdges()[reverse_edge_id].flow -= temp_flow;
        return temp_flow;
      }
    }
  }
  return 0;
}

struct Edge {
  int64_t index;
  int64_t from;
  int64_t to;
  int64_t capacity;
  int64_t flow;
  Edge(int64_t i, int64_t u, int64_t v, int64_t cap, int64_t fl)
      : index(i), from(u), to(v), capacity(cap), flow(fl) {}
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;
template <typename T>
using PairVector = std::vector<std::pair<T, T>>;
using EdgeVector = std::vector<Edge>;

const int64_t kInf = std::numeric_limits<int64_t>::max();

class Graph {
  Matrix<int64_t> adj_list_;
  EdgeVector edges_;

 public:
  explicit Graph(int64_t n) : adj_list_(n + 1) {}
  void AddEdge(int64_t from, int64_t to, int64_t capacity) {
    Edge edge(edges_.size(), from, to, capacity, 0);
    adj_list_[from].push_back(edge.index);
    edges_.push_back(edge);
  }
  void AddEdgesFromPairs(const PairVector<int64_t>& pairs, int64_t capacity) {
    for (const auto& p : pairs) {
      AddEdge(p.first, p.second, capacity);
      AddEdge(p.second, p.first, 0);
    }
  }
  Matrix<int64_t>& GetAdjList() { return adj_list_; }
  const Matrix<int64_t>& GetAdjList() const { return adj_list_; }
  EdgeVector& GetEdges() { return edges_; }
  const EdgeVector& GetEdges() const { return edges_; }
};

class FlowNetwork {
  Graph graph;
  int64_t source_;
  int64_t target_;

 public:
  explicit FlowNetwork(int64_t n) : graph(n), source_(-1), target_(-1) {}
  Graph& GetGraph() { return graph; }
  const Graph& GetGraph() const { return graph; }
  void SetSourceTarget(int64_t s, int64_t t) {
    source_ = s;
    target_ = t;
  }
  int64_t GetSource() const { return source_; }
  int64_t GetTarget() const { return target_; }
};

class FlowNetworkBuilder {
 public:
  FlowNetworkBuilder() = default;

  FlowNetwork BuildBaseGraph(int64_t n, const PairVector<int64_t>& p) {
    FlowNetwork flow_network(n);
    flow_network.GetGraph().AddEdgesFromPairs(p, kInf);
    return flow_network;
  }

  FlowNetwork SetupSourceTarget(FlowNetwork flow_network,
                                const std::vector<int64_t>& init_capacity,
                                int64_t mid) {
    Graph& g = flow_network.GetGraph();
    int64_t source = g.GetAdjList().size();
    int64_t target = g.GetAdjList().size() + 1;
    g.GetAdjList().resize(g.GetAdjList().size() + 2);
    for (int64_t i = 1; i < (int64_t)init_capacity.size(); ++i) {
      g.AddEdge(source, i, init_capacity[i]);
      g.AddEdge(i, source, 0);
      g.AddEdge(i, target, mid);
      g.AddEdge(target, i, 0);
    }
    flow_network.SetSourceTarget(source, target);
    return flow_network;
  }
};

class DinitsBFSVisitor : public GraphVisitor {
  Graph& graph;
  std::vector<int64_t>& level_;

 public:
  DinitsBFSVisitor(Graph& g, std::vector<int64_t>& l) : graph(g), level_(l) {}

  bool ExamineEdge(int64_t u, int64_t v, int64_t edge_id) override {
    auto& edge = graph.GetEdges()[edge_id];
    if (level_[v] < 0 && edge.capacity - edge.flow > 0) {
      level_[v] = level_[u] + 1;
      return true;
    }
    return false;
  }
};

class DinitsDFSVisitor : public GraphVisitor {
  Graph& graph;
  std::vector<int64_t>& level_;
  std::vector<int64_t>& start_;

 public:
  DinitsDFSVisitor(Graph& g, std::vector<int64_t>& l,
                   std::vector<int64_t>& source)
      : graph(g), level_(l), start_(source) {}

  bool HasNextEdge(int64_t u) {
    return (start_[u] < (int64_t)graph.GetAdjList()[u].size());
  }

  int64_t GetNextEdge(int64_t u) { return graph.GetAdjList()[u][start_[u]++]; }

  bool ExamineEdge(int64_t u, int64_t v, int64_t edge_id) override {
    auto& edge = graph.GetEdges()[edge_id];
    if (level_[v] == level_[u] + 1 && edge.capacity - edge.flow > 0) {
      return true;
    }
    return false;
  }
};

int64_t FindMaxFlow(Graph& graph, int64_t source, int64_t target, int64_t n) {
  std::vector<int64_t> level(n + 1, -1);
  std::vector<int64_t> start(n + 1, 0);
  int64_t max_flow = 0;

  while (true) {
    std::fill(level.begin(), level.end(), -1);
    level[source] = 0;

    DinitsBFSVisitor bfs_visitor(graph, level);
    if (!BFS(graph, source, target, bfs_visitor) || level[target] < 0) {
      break;
    }

    std::fill(start.begin(), start.end(), 0);
    while (true) {
      DinitsDFSVisitor dfs_visitor(graph, level, start);
      int64_t pushed = DFS(graph, source, target, kInf, dfs_visitor);
      if (pushed == 0) {
        break;
      }
      max_flow += pushed;
    }
  }
  return max_flow;
}

bool IsPossibleToTransfer(const Graph& graph, int64_t total_count,
                          int64_t target) {
  int64_t result = 0;
  for (std::size_t i = 0; i < graph.GetEdges().size(); ++i) {
    if ((i % 2 == 0) && (graph.GetEdges()[i].to == target)) {
      result += graph.GetEdges()[i].flow;
    }
  }
  return (result == total_count);
}

template <typename T, typename Predicate>
T BinarySearch(T left, T right, Predicate pred) {
  T answer = right;
  while (left <= right) {
    T mid = (left + right) / 2;
    if (pred(mid)) {
      answer = mid;
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return answer;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int64_t n;
  int64_t k;
  std::cin >> n >> k;

  std::vector<int64_t> init_capacity(n + 1);
  for (int64_t i = 1; i <= n; ++i) {
    std::cin >> init_capacity[i];
  }

  int64_t total_count =
      std::accumulate(init_capacity.begin(), init_capacity.end(), 0);
  int64_t min_capacity =
      *std::min_element(init_capacity.begin(), init_capacity.end());
  int64_t max_capacity =
      *std::max_element(init_capacity.begin(), init_capacity.end());

  PairVector<int64_t> pairs;
  pairs.reserve(k);
  for (int64_t i = 0; i < k; ++i) {
    int64_t a, b;
    std::cin >> a >> b;
    pairs.emplace_back(a, b);
  }

  FlowNetworkBuilder builder;
  auto CanTransferAll = [&](int64_t mid) {
    FlowNetwork flow_network = builder.BuildBaseGraph(n, pairs);
    flow_network = builder.SetupSourceTarget(flow_network, init_capacity, mid);
    Graph& g = flow_network.GetGraph();
    int64_t source = flow_network.GetSource();
    int64_t target = flow_network.GetTarget();
    FindMaxFlow(g, source, target, g.GetAdjList().size() - 1);
    return IsPossibleToTransfer(g, total_count, target);
  };

  int64_t answer =
      BinarySearch<int64_t>(min_capacity, max_capacity, CanTransferAll);

  std::cout << answer << "\n";

  return 0;
}
