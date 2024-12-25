#include <iostream>
#include <vector>
#include <queue>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

 // нумерация вершин с единицы, граф невзвешенный и неориентирвоанный
Matrix<int> MakeAdjastencyList(int vertice_num, int edge_num) {
  Matrix<int> adj_list(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;

    adj_list[u].push_back(v);
    adj_list[v].push_back(u);
  }
  return adj_list;
}

// нумерация вершин с единицы, граф взвешенный и ориентированный
// для 0-k bfs
Matrix<int> MakeAdjacencyListWithWeights(int vertice_num, int edge_num) {
  Matrix<int> adj_list;
  int next_index = vertice_num + 1;

  adj_list.resize(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int a = 0;
    int b = 0;
    int t = 0;
    std::cin >> a >> b >> t;

    int current = a;
    for (int j = 1; j < t; j++) {
      int new_vertex = next_index++;
      adj_list.resize(next_index);
      adj_list[current].push_back(new_vertex);
      current = new_vertex;
    }
    adj_list[current].push_back(b);
  }

  return adj_list;
}

std::pair<std::vector<int>, std::vector<int>> BFS(const Matrix<int>& graph, int start) {
  const int inf = 1e9;
  std::queue<int> q;
  std::vector<int> dist(graph.size(), inf);
  std::vector<int> parent(graph.size(), -1);
  
  dist[start] = 0;
  q.push(start);

  while (!q.empty()) {
    int current = q.front();
    q.pop();

    for (int neighbor : graph[current]) {
      if (dist[neighbor] == inf) {
        dist[neighbor] = dist[current] + 1;
        parent[neighbor] = current;
        q.push(neighbor);
      }
    }
  }
  return {dist, parent};
}

// 0-1 BFS
std::pair<std::vector<int>, std::vector<int>> BFS(const std::vector<std::vector<std::pair<int, int>>>& graph,
                                                  int start) {
  const int inf = 1e9;
  std::deque<int> q;
  std::vector<int> dist(graph.size(), inf);
  std::vector<int> parent(graph.size(), -1);

  dist[start] = 0;
  q.push_back(start);

  while (!q.empty()) {
    int current = q.front();
    q.pop_front();

    for (auto& neighbor : graph[current]) {
      if (dist[current] + neighbor.second < dist[neighbor.first]) {
        dist[neighbor.first] = dist[current] + neighbor.second;
        parent[neighbor.first] = current;
        if (neighbor.second == 0) {
          q.push_front(neighbor.first);
        } else if (neighbor.second == 1) {
          q.push_back(neighbor.first);
        }
      }
    }
  }
  return {dist, parent};
}

int dfs_time = 0;

// 0 - не посещена - WHITE, 1 - в процессе - GRAY, 2 - обработана - BLACK
void DFS(int v, Matrix<int>& adj_list, std::vector<int>& color, std::vector<int>& parent,
         std::vector<int>& time_in, std::vector<int>& time_out) {
  color[v] = 1;
  time_in[v] = ++dfs_time;

  for (int u : adj_list[v]) {
    if (color[u] == 0) {
      parent[u] = v;
      DFS(u, adj_list, color, parent, time_in, time_out);
    }
  }

  color[v] = 2;
  time_out[v] = ++dfs_time;
}

//проверка на ацикличность
bool FindCycleDFS(int v, Matrix<int>& adj_list, std::vector<int>& color) {
  color[v] = 1;

  for (int u : adj_list[v]) {
    if (color[u] == 1) {
      return true;
    }
    if (color[u] == 0) {
      if (FindCycleDFS(u, adj_list, color)) {
        return true;
      }
    }
  }

  color[v] = 2;
  return false;
}

void FindCycle(Matrix<int>& adj_list) {
  std::vector<int> color(adj_list.size(), 0);
  std::vector<int> parent(adj_list.size(), -1);
  
  for (int v = 1; v < adj_list.size(); v++) {
    if (color.at(v) == 0) {
      if (FindCycleDFS(v, adj_list, color)) {
        std::cout << "YES\n";
        return;
      }
    }
  }
  std::cout << "NO\n";
}