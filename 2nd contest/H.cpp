#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void InputMatrix(Matrix<bool>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      char x;
      std::cin >> x;
      bool value = (i == j ? 1 : x - '0');
      adj_matrix[i][j] = value;
    }
  }
}

std::vector<int64_t> CreateAdjList(const Matrix<bool>& adj_matrix) {
  std::vector<int64_t> adj_list(adj_matrix.size(), 0);
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (auto it = adj_matrix[i].rbegin(); it != adj_matrix[i].rend(); ++it) {
      adj_list[i] = (adj_list[i] << 1) + *it;
    }
  }
  return adj_list;
}

void DivideGraph(const Matrix<bool>& adj_matrix, Matrix<bool>& adj_matrix_1,
                 Matrix<bool>& adj_matrix_2) {
  int64_t division = (adj_matrix.size() + 1) / 2;
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      if (i < division && j < division) {
        adj_matrix_1[i][j] = adj_matrix[i][j];
      }
      if (i >= division && j >= division) {
        adj_matrix_2[i][j] = adj_matrix[i][j];
      }
      if (i == j) {
        adj_matrix_1[i][j] = 1;
        adj_matrix_2[i][j] = 1;
      }
    }
  }
}

void OutputMatrix(const Matrix<bool>& adj_matrix) {
  for (size_t i = 0; i < adj_matrix.size(); ++i) {
    for (size_t j = 0; j < adj_matrix.size(); ++j) {
      std::cout << adj_matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
}

bool Contains(int64_t mask, int64_t vertex) { return mask & (1ll << vertex); }

void SetInitParametersForCliqueSearch(const Matrix<bool>& adj_matrix,
                                      int64_t part, int64_t& max_mask,
                                      size_t& offset) {
  if (part == 0) {
    max_mask = (1ll << adj_matrix.size()) - 1;
    offset = 0;
  } else {
    max_mask = (1ll << ((adj_matrix.size() + 1) / 2)) - 1;
  }
  if (part == 1) {
    offset = 0;
  } else if (part == 2) {
    offset = (adj_matrix.size() + 1) / 2;
  }
}

std::vector<bool> GetCliques(const Matrix<bool>& adj_matrix,
                             const std::vector<int64_t>& adj_list,
                             int64_t part) {
  int64_t max_mask = 0;
  size_t offset = 0;
  SetInitParametersForCliqueSearch(adj_matrix, part, max_mask, offset);
  std::vector<bool> is_clique(max_mask + 1, false);

  is_clique[0] = true;
  int last = -1;

  for (int64_t mask = 1; mask <= max_mask; ++mask) {
    if (mask == (1 << (last + 1))) {
      last += 1;
    }

    mask <<= offset;
    last += offset;

    int64_t prev_mask = mask ^ (1ll << last);

    if ((adj_list[last] & mask) == mask) {
      is_clique[mask >> offset] = is_clique[prev_mask >> offset];
    }

    last -= offset;
    mask >>= offset;
  }

  return is_clique;
}

std::vector<int64_t> HowManySubmasksAreCliques(
    const Matrix<bool>& adj_matrix, const std::vector<int64_t>& adj_list) {
  int64_t max_mask = (1ll << (adj_matrix.size() / 2)) - 1;
  size_t offset = (adj_matrix.size() + 1) / 2;
  std::vector<int64_t> subgraphs_that_cliques_count(max_mask + 1, 0);
  int last = -1;

  subgraphs_that_cliques_count[0] = 1;

  for (int64_t mask = 1; mask <= max_mask; ++mask) {
    if (mask == (1 << (last + 1))) {
      last += 1;
    }
    mask <<= offset;
    last += offset;

    int64_t prev_mask = mask ^ (1ll << last);
    int64_t prev_mask_2 = (mask & adj_list[last]) ^ (1ll << last);

    subgraphs_that_cliques_count[mask >> offset] =
        subgraphs_that_cliques_count[prev_mask >> offset] +
        subgraphs_that_cliques_count[prev_mask_2 >> offset];

    last -= offset;
    mask >>= offset;
  }

  return subgraphs_that_cliques_count;
}

std::vector<uint32_t> MaxConnectedMaskInSecond(
    const Matrix<bool>& adj_matrix, const std::vector<int64_t>& adj_list,
    const std::vector<int64_t>& adj_list_all) {
  uint32_t max_mask = (1ll << ((adj_matrix.size() + 1) / 2)) - 1;
  std::vector<uint32_t> max_connected_from_second(max_mask + 1, 0);
  int last = -1;
  size_t offset = (adj_matrix.size() + 1) / 2;

  max_connected_from_second[0] = (1ll << (adj_matrix.size() / 2)) - 1;

  for (uint32_t mask = 1; mask <= max_mask; ++mask) {
    if (mask == (1 << (last + 1))) {
      last += 1;
    }

    uint32_t prev_mask = mask ^ (1ll << last);

    max_connected_from_second[mask] = max_connected_from_second[prev_mask] &
                                      uint32_t(adj_list_all[last] >> offset);
  }

  return max_connected_from_second;
}

int64_t CountCliques(const Matrix<bool>& adj_matrix) {
  int64_t n = adj_matrix.size();
  std::vector<int64_t> adj_list = CreateAdjList(adj_matrix);

  Matrix<bool> adj_matrix_1(n, std::vector<bool>(n));
  Matrix<bool> adj_matrix_2(n, std::vector<bool>(n));
  DivideGraph(adj_matrix, adj_matrix_1, adj_matrix_2);

  std::vector<int64_t> adj_list_1 = CreateAdjList(adj_matrix_1);
  std::vector<int64_t> adj_list_2 = CreateAdjList(adj_matrix_2);

  std::vector<bool> is_clique_1 = GetCliques(adj_matrix_1, adj_list_1, 1);

  std::vector<int64_t> subcliques_count_2 =
      HowManySubmasksAreCliques(adj_matrix_2, adj_list_2);

  std::vector<uint32_t> adj_cliques_1 =
      MaxConnectedMaskInSecond(adj_matrix_1, adj_list_1, adj_list);

  int64_t result = 0;
  uint32_t max_mask = (1ll << ((n + 1) / 2)) - 1;

  for (uint32_t mask = 0; mask <= max_mask; ++mask) {
    if (is_clique_1[mask]) {
      result += subcliques_count_2[adj_cliques_1[mask]];
    }
  }

  return result;
}

int main() {
  size_t n;

  std::cin >> n;

  Matrix<bool> adj_matrix(n, std::vector<bool>(n));

  InputMatrix(adj_matrix);

  std::cout << CountCliques(adj_matrix) << '\n';
}