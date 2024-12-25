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


Matrix<bool> ReadField(int m, int n) {
  Matrix<bool> field(m, std::vector<bool>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      char cell;
      std::cin >> cell;
      field[i][j] = cell == '.' ? false : true;
    }
  }
  return field;
}

Matrix<int> MakeAdjacencyList(const Matrix<bool>& field, int& counter, EdgeVector& edges) {
  Matrix<int> adj_list(field.size() * field[0].size() + 1);
  for (int i = 0; i < field.size(); ++i) {
    for (int j = 0; j < field[i].size(); ++j) {
      if (field[i][j]) {
        ++counter;
        std:size_t adj_list_index = i * field[0].size() + j + 1;
        std::size_t adj_list_index_right = adj_list_index + 1;
        std::size_t adj_list_index_down = adj_list_index + field[0].size();
        // std::cout << i << ' ' << j << '\n';
        // std::cout << adj_list_index << ' ' << j + 1 << ' ' << i + 1 << ' ' << field[0].size() << ' ' << field.size() << '\n';
        if (j + 1 < field[0].size()) {
          if (field[i][j + 1]) {
            if ((i + j + 1) % 2 != 0) {
              AddEdge(adj_list_index_right, adj_list_index, 1, edges, adj_list);
            } else {
              AddEdge(adj_list_index, adj_list_index_right, 1, edges, adj_list);
            }
          }
        } 
        if (i + 1 < field.size()) {
          if (field[i + 1][j]) {
            if ((i + j + 1) % 2 != 0) {
              AddEdge(adj_list_index_down, adj_list_index, 1, edges, adj_list);
            } else {
              AddEdge(adj_list_index, adj_list_index_down, 1, edges, adj_list);
            }
          }
        }
      }
    }
  }
  return adj_list;
}

int main() {
  int m = 0;
  int n = 0;
  int a = 0;
  int b = 0;
  std::cin >> m >> n >> a >> b;

  int counter = 0;

  Matrix<bool> field = ReadField(m, n);

  EdgeVector edges;

  Matrix<int> adj_list = MakeAdjacencyList(field, counter, edges);

  // for (int i = 0; i < field.size(); ++i) {
  //   for (int j = 0; j < field[i].size(); ++j) {
  //     std::cout << field[i][j] << ' ';
  //   }
  //   std::cout << '\n';
  // }

  // for (int i = 1; i < adj_list.size(); ++i) {
  //   for (int j = 0; j < adj_list[i].size(); ++j) {
  //     const auto& edge = adj_list[i][j];
  //     std::cout << i << ' ' << edge.first << ' ' << edge.second << '\n';
  //   }
  // }

  int s = adj_list.size();
  int t = adj_list.size() + 1;

  adj_list.resize(adj_list.size() + 2);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std:size_t adj_list_index = i * n + j + 1;
      if ((i + j + 1) % 2 == 0 && field[i][j]) {
        AddEdge(s, adj_list_index, 1, edges, adj_list);
      } else if (field[i][j]) {
        AddEdge(adj_list_index, t, 1, edges, adj_list);  
      }
    }
  }

  int count_2x1 = Dinitz(s, t, edges, adj_list, adj_list.size());
  int count_1x1 = counter - count_2x1 * 2;

  std::cout << std::min(count_2x1 * a + count_1x1 * b, counter * b) << '\n';

  // for (std::size_t i = 0; i < edges.size(); ++i) {
  //   if (edges[i].capacity == 1) {
  //     std::cout << edges[i].from << ' ' << edges[i].to << '\n';
  //   }
  // }

  return 0;
}