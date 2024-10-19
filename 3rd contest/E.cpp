#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct SCC {
  int count;
  std::vector<int> component;

  SCC(int count_, std::vector<int> component_)
      : count(count_), component(component_) {}

  void Output() const {
    std::cout << count << '\n';
    for (int i = 1; i < component.size(); i++) {
      std::cout << component[i] << ' ';
    }
    std::cout << '\n';
  }
};

void MakeAdjastencyList(int vertice_num, int edge_num, Matrix<int>& adj_list,
                        Matrix<int>& transposed_adj_list) {
  adj_list.resize(vertice_num + 1);
  transposed_adj_list.resize(vertice_num + 1);

  for (int i = 0; i < edge_num; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    adj_list[u].push_back(v);
    transposed_adj_list[v].push_back(u);
  }
}

void SortingDFS(int v, Matrix<int>& adj_list, std::vector<int>& color,
                std::stack<int>& sorted_vertices) {
  color[v] = 1;

  for (int u : adj_list[v]) {
    if (color[u] == 0) {
      SortingDFS(u, adj_list, color, sorted_vertices);
    }
  }

  color[v] = 2;
  sorted_vertices.push(v);
}

void MarkingComponentDFS(int v, Matrix<int>& transposed_adj_list,
                         std::vector<int>& color, std::vector<int>& component,
                         int component_count) {
  color[v] = 1;
  component[v] = component_count;

  for (int u : transposed_adj_list[v]) {
    if (color[u] == 0) {
      MarkingComponentDFS(u, transposed_adj_list, color, component,
                          component_count);
    }
  }

  color[v] = 2;
}

SCC FindStronglyConnectedComponents(int n, Matrix<int>& adj_list,
                                    Matrix<int>& transposed_adj_list) {
  std::vector<int> color(n + 1, 0);
  std::vector<int> component(n + 1, 0);
  std::stack<int> sorted_vertices;
  int component_count = 0;

  for (int i = 1; i <= n; i++) {
    if (color[i] == 0) {
      SortingDFS(i, adj_list, color, sorted_vertices);
    }
  }

  color.assign(color.size(), 0);

  while (!sorted_vertices.empty()) {
    int v = sorted_vertices.top();
    sorted_vertices.pop();
    if (color[v] == 0) {
      component_count++;
      MarkingComponentDFS(v, transposed_adj_list, color, component,
                          component_count);
    }
  }

  SCC answer(component_count, component);

  return answer;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  Matrix<int> adj_list;
  Matrix<int> transposed_adj_list;
  MakeAdjastencyList(n, m, adj_list, transposed_adj_list);

  SCC answer =
      FindStronglyConnectedComponents(n, adj_list, transposed_adj_list);

  answer.Output();
}
