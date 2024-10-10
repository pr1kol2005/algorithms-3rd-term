#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using SetVector = std::vector<std::set<T>>;

template <typename T>
using StackSet = std::stack<std::set<T>>;

template <typename T>
using SetIt = std::set<T>::iterator;

void InputComplement(Matrix<int>& adj_matrix_complement) {
  for (size_t i = 0; i < adj_matrix_complement.size(); ++i) {
    for (size_t j = 0; j < adj_matrix_complement.size(); ++j) {
      char x;
      std::cin >> x;
      adj_matrix_complement[i][j] = (i == j ? 0 : !(x - '0'));
    }
  }
}

void OutputMatrix(const Matrix<int>& adj_matrix_complement) {
  for (size_t i = 0; i < adj_matrix_complement.size(); ++i) {
    for (size_t j = 0; j < adj_matrix_complement.size(); ++j) {
      std::cout << adj_matrix_complement[i][j] << ' ';
    }
    std::cout << '\n';
  }
}

SetVector<int> BronKerbosh(const Matrix<int>& adj_matrix_complement) {
  std::set<int> max_clique_set;
  std::set<int> possible_max_clique_members;
  std::set<int> not_clique_members;
  SetVector<int> max_cliques;

  for (int u = 0; u < adj_matrix_complement.size(); u++) {
    possible_max_clique_members.insert(u);
  }

  int u = 0;
  std::stack<int> recursive_vertex_stack;
  StackSet<int> recursive_set_stack;
  SetIt<int> current_vertex_it;

  while ((possible_max_clique_members.size() != 0) ||
         (max_clique_set.size() != 0)) {
    if (possible_max_clique_members.size() != 0) {
      current_vertex_it = possible_max_clique_members.begin();
      u = *current_vertex_it;

      recursive_set_stack.push(max_clique_set);
      recursive_set_stack.push(possible_max_clique_members);
      recursive_set_stack.push(not_clique_members);
      recursive_vertex_stack.push(u);

      max_clique_set.insert(u);

      for (int v = 0; v < adj_matrix_complement.size(); v++) {
        if (adj_matrix_complement[u][v]) {
          current_vertex_it = possible_max_clique_members.find(v);
          if (current_vertex_it != possible_max_clique_members.end()) {
            possible_max_clique_members.erase(current_vertex_it);
          }
          current_vertex_it = not_clique_members.find(v);
          if (current_vertex_it != not_clique_members.end()) {
            not_clique_members.erase(current_vertex_it);
          }
        }
      }

      current_vertex_it = possible_max_clique_members.find(u);
      if (current_vertex_it != possible_max_clique_members.end()) {
        possible_max_clique_members.erase(current_vertex_it);
      }
    } else {
      if (not_clique_members.size() == 0) {
        max_cliques.push_back(max_clique_set);
      }

      u = recursive_vertex_stack.top();
      recursive_vertex_stack.pop();
      not_clique_members = recursive_set_stack.top();
      recursive_set_stack.pop();
      possible_max_clique_members = recursive_set_stack.top();
      recursive_set_stack.pop();
      max_clique_set = recursive_set_stack.top();
      recursive_set_stack.pop();

      current_vertex_it = possible_max_clique_members.find(u);
      if (current_vertex_it != possible_max_clique_members.end()) {
        possible_max_clique_members.erase(current_vertex_it);
      }

      not_clique_members.insert(u);
    }
  }
  return max_cliques;
}

int CountCliques(const SetVector<int>& max_cliques) {
  int result = 0;

  for (auto it1 = max_cliques.begin(); it1 < max_cliques.end(); it1++) {
    result += (1 << it1->size()) - 1;
    // for (auto& vertex : *it1) {
    //   std::cout << vertex + 1 << ' ';
    // }
    // std::cout << '\n';
    // std::cout << (1 << it1->size()) - 1 << "|\n";
    for (auto it2 = it1 + 1; it2 < max_cliques.end(); it2++) {
      std::vector<int> intersection;
      std::set_intersection(it1->begin(), it1->end(), it2->begin(),
                                it2->end(), std::back_inserter(intersection));
      result -= ((1 << intersection.size()) - 1);
      //std::cout << (1 << intersection.size()) - 1 << "z\n";
    }
  }

  return result + 1;
}

int main() {
  int n;

  std::cin >> n;

  Matrix<int> adj_matrix_complement(n, std::vector<int>(n));

  InputComplement(adj_matrix_complement);

  // OutputMatrix(adj_matrix_complement);

  std::cout << CountCliques(BronKerbosh(adj_matrix_complement)) << '\n';

  return 0;
}