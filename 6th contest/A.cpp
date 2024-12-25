#include <iostream>
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

struct Edge {
  int index;
  int from;
  int to;
  int capacity;
  int flow;

  Edge(int i, int u, int v, int cap, int fl) : index(i), from(u), to(v), capacity(cap), flow(fl) {}
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using EdgeVector = std::vector<Edge>;

const int kInf = std::numeric_limits<int>::max();

void AddEdge(int from, int to, int capacity, EdgeVector& edges, Matrix<int>& adj_list) {
  Edge edge(edges.size(), from, to, capacity, 0);
  adj_list[from].push_back(edge.index);
  edges.push_back(edge);

  Edge reverse_edge(edges.size(), to, from, 0, 0);
  adj_list[to].push_back(reverse_edge.index);
  edges.push_back(reverse_edge);
}

Matrix<int> MakeAdjacencyList(int n, int m, EdgeVector& edges) {
  Matrix<int> adj_list(n + 1);
  for (int i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    int с = 0;
    std::cin >> a >> b >> с;
    Edge edge(2 * i, a, b, с, 0);
    adj_list[a].push_back(edge.index);
    edges.push_back(edge);

    Edge reverse_edge(2 * i + 1, b, a, 0, 0);
    adj_list[b].push_back(reverse_edge.index);
    edges.push_back(reverse_edge);
  }
  return adj_list;
}

bool BFS(int s, int t, EdgeVector& edges, Matrix<int>& adj_list, std::vector<int>& level) {
  std::fill(level.begin(), level.end(), -1);
  std::queue<int> q;

  level[s] = 0;
  q.push(s);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int i = 0; i < adj_list[u].size(); ++i) {
      int edge_id = adj_list[u][i];
      int v = edges[edge_id].to;
      if (level[v] < 0 && edges[edge_id].capacity - edges[edge_id].flow > 0) {
        level[v] = level[u] + 1;
        q.push(v);
      }
    }
  }

  return !(level[t] < 0);
}

int DFS(int u, int t, int flow, EdgeVector& edges, Matrix<int>& adj_list,
        std::vector<int>& level, std::vector<int>& start) {
  if (u == t) {
    return flow;
  }

  if (flow == 0) {
    return 0;
  }

  while (start[u] < adj_list[u].size()) {
    int edge_id = adj_list[u][start[u]];
    int v = edges[edge_id].to;
    if (level[v] == level[u] + 1) {  // removed &&
      int curr_flow = std::min(flow, edges[edge_id].capacity - edges[edge_id].flow);
      int temp_flow = DFS(v, t, curr_flow, edges, adj_list, level, start);

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

int Dinitz(int s, int t, EdgeVector& edges, Matrix<int>& adj_list, int n) {
  std::vector<int> level(n + 1);
  int max_flow = 0;

  while (BFS(s, t, edges, adj_list, level)) {
    std::vector<int> start(n + 1, 0);
    while (int flow = DFS(s, t, kInf, edges, adj_list, level, start)) {
      max_flow += flow;
    }
  }

  return max_flow;
}

const int max_part_size = 250;

Matrix<int> ReadMatching(int n, int m, EdgeVector& edges) {
  int matching_size = 2 * max_part_size + 1;
  Matrix<int> adj_list(2 * max_part_size + 1);

  for (int i = 1; i <= n; ++i) {
    int from_a_to = 0;
    std::cin >> from_a_to;
    while (from_a_to != 0) {
      AddEdge(i, from_a_to + max_part_size, 1, edges, adj_list);
      std::cin >> from_a_to;
    }
  }

  return adj_list;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  EdgeVector edges;

  Matrix<int> adj_list = ReadMatching(n, m, edges);

  // for (int i = 1; i <= n; ++i) {
  //   for (auto edge_id : adj_list[i]) {
  //     std::cout << edges[edge_id].from << ' ' << edges[edge_id].to << '\n';
  //   }
  // }

  int s = adj_list.size();
  int t = adj_list.size() + 1;

  adj_list.resize(adj_list.size() + 2);
  for (int i = 1; i <= n; ++i) {
    AddEdge(s, i, 1, edges, adj_list);
  }
  for (int i = 1; i <= m; ++i) {
    AddEdge(i + max_part_size, t, 1, edges, adj_list);
  }

  std::cout << Dinitz(s, t, edges, adj_list, adj_list.size()) << '\n';

  for (std::size_t i = 0; i < edges.size(); ++i) {
    if (edges[i].capacity == 1 && edges[i].from <= 2 * max_part_size && edges[i].to <= 2 * max_part_size && edges[i].flow == 1) {
      std::cout << edges[i].from << ' ' << edges[i].to - max_part_size << '\n';
    }
  }

  return 0;
}