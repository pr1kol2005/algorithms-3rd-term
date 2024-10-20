#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

using StrUnorderedSet = std::unordered_set<std::string>;

using PairVector = std::vector<std::pair<int, int>>;

struct Puzzle {
  Matrix<int> board;
  int x; // x coordinate of empty cell
  int y; // y coordinate of empty cell
  int dist; // sum of Manhattan distances
  std::string path;

  Puzzle(Matrix<int> b, int xx, int yy, int d, std::string p)
      : board(b), x(xx), y(yy), dist(d), path(p) {}

  bool operator>(const Puzzle& other) const {
    return (path.size() + dist) > (other.path.size() + other.dist);
  }
};

const Matrix<int> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
const PairVector directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
const std::string directions_letters = "RLDU";
const std::string empty_string = "";

Matrix<int> InputBoard() {
  Matrix<int> board(3, std::vector<int>(3, 0));
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cin >> board[i][j];
    }
  }
  return board;
}

int CalculateDist(const Matrix<int>& board) {
  int dist = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == 0) {
        continue;
      }
      int target_x = (board[i][j] - 1) / 3;
      int target_y = (board[i][j] - 1) % 3;
      dist += std::abs(i - target_x) + std::abs(j - target_y);
    }
  }
  return dist;
}

bool IsSolvable(const Matrix<int>& board) {
  std::vector<int> board_flat;
  for (const auto& row : board) {
    for (const auto& cell : row) {
      if (cell != 0) {
        board_flat.push_back(cell);
      }
    }
  }
  int sum_of_bigger_before = 0;
  for (size_t i = 0; i < board_flat.size(); ++i) {
    for (size_t j = i + 1; j < board_flat.size(); ++j) {
      if (board_flat[i] > board_flat[j]) {
        ++sum_of_bigger_before;
      }
    }
  }
  return sum_of_bigger_before % 2 == 0;
}

void SolvePuzzle(const Matrix<int>& start) {
  PriorityQueue<Puzzle> pq;
  StrUnorderedSet visited;

  int start_x = 0;
  int start_y = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (start[i][j] == 0) {
        start_x = i;
        start_y = j;
      }
    }
  }

  pq.emplace(start, start_x, start_y, CalculateDist(start), empty_string);

  while (!pq.empty()) {
    Puzzle current = pq.top();
    pq.pop();

    if (current.board == goal) {
      std::cout << current.path.size() << '\n';
      std::cout << current.path << '\n';
      return;
    }

    std::string current_state;
    for (const auto& row : current.board) {
      for (const auto& cell : row) {
        current_state += std::to_string(cell);
      }
    }

    if (visited.find(current_state) != visited.end()) {
      continue;
    }
    visited.insert(current_state);

    for (int i = 0; i < 4; ++i) {
      int new_x = current.x + directions[i].first;
      int new_y = current.y + directions[i].second;

      if (new_x >= 0 && new_x < 3 && new_y >= 0 && new_y < 3) {
        Matrix<int> new_board = current.board;
        std::swap(new_board[current.x][current.y], new_board[new_x][new_y]);
        pq.emplace(new_board, new_x, new_y, CalculateDist(new_board),
                   current.path + directions_letters[i]);
      }
    }
  }
}

int main() {
  Matrix<int> start = InputBoard();

  if (IsSolvable(start)) {
    SolvePuzzle(start);
  } else {
    std::cout << -1 << '\n';
  }
}