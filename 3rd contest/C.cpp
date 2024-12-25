#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

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

void FindComponentsDFS(int v, Matrix<int>& adj_list, std::vector<int>& color,
                       std::vector<int>& component) {
  color[v] = 1;
  component.push_back(v);
  for (int u : adj_list[v]) {
    if (color[u] == 0) {
      FindComponentsDFS(u, adj_list, color, component);
    }
  }

  color[v] = 2;
}

void FindComponents(Matrix<int>& adj_list) {
  std::vector<int> color(adj_list.size(), 0);
  Matrix<int> components;

  for (int i = 1; i < adj_list.size(); ++i) {
    if (color[i] == 0) {
      std::vector<int> component;
      FindComponentsDFS(i, adj_list, color, component);
      components.push_back(component);
    }
  }

  std::cout << components.size() << "\n";
  for (const auto& component : components) {
    std::cout << component.size() << "\n";
    for (const auto& student : component) {
      std::cout << student << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  Matrix<int> adj_list = MakeAdjastencyList(n, m);

  FindComponents(adj_list);

  return 0;
}
