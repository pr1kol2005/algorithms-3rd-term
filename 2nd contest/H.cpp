#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void InputMatrix(Matrix<int>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    std::string row;
    std::cin >> row;
    for (size_t j = 0; j < row.size(); ++j) {
      adj_matrix[i][j] = row[j] - '0';
    }
  }
}

int64_t CountCliques(const Matrix<int>& adj_matrix) {
  return 0;
}

int main() {
  int n = 0;

  std::cin >> n;

  Matrix<int> adj_matrix(n, std::vector<int>(n, 0));

  InputMatrix(adj_matrix);

  std::cout << CountCliques(adj_matrix) << '\n'; 
}