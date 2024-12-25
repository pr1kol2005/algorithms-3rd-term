#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

using PairVector = std::vector<std::pair<int, int>>;

using BoardState = uint64_t;

using UnorderedSet = std::unordered_set<BoardState>;

using Map = std::unordered_map<BoardState, std::pair<BoardState, char>>;

BoardState ConvertToBitmask(const Matrix<int>& board);

const int kBoardSize = 4;
const int kDirectionsNumber = 4;
const int kMaxIndex = kBoardSize - 1;
const int kMaxValue = kBoardSize * kBoardSize - 1;
const BoardState kGoal = ConvertToBitmask(
    {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}});
const PairVector kDirections = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
const std::string kDirectionsLetters = "LRUD";
const char kEmptyChar = '\0';

struct Puzzle {
  BoardState board;
  int x;              // x coordinate of empty cell
  int y;              // y coordinate of empty cell
  int64_t dist;       // sum of Manhattan distances and Linear conflicts
  int64_t path_size;  // number of made moves

  Puzzle(BoardState b, int xx, int yy, int64_t d, int64_t p)
      : board(b), x(xx), y(yy), dist(d), path_size(p) {}

  bool operator>(const Puzzle& other) const {
    return (path_size + dist) > (other.path_size + other.dist);
  }
};

BoardState ConvertToBitmask(const Matrix<int>& board) {
  BoardState state = 0;
  for (int i = 0; i < kBoardSize; ++i) {
    for (int j = 0; j < kBoardSize; ++j) {
      state = (state << 4) | board[i][j];
    }
  }
  return state;
}

Matrix<int> ConvertFromBitmask(BoardState state) {
  Matrix<int> board(kBoardSize, std::vector<int>(kBoardSize));
  for (int i = kMaxIndex; i >= 0; --i) {
    for (int j = kMaxIndex; j >= 0; --j) {
      board[i][j] = state & kMaxValue;
      state >>= 4;
    }
  }
  return board;
}

Matrix<int> InputBoard() {
  Matrix<int> board(kBoardSize, std::vector<int>(kBoardSize, 0));
  for (int i = 0; i < kBoardSize; ++i) {
    for (int j = 0; j < kBoardSize; ++j) {
      std::cin >> board[i][j];
    }
  }
  return board;
}

// int CalculateManhatannDist(BoardState board_state) {
//   int value = 0;
//   int dist = 0;
//   for (int i = kMaxIndex; i >= 0; --i) {
//     for (int j = kMaxIndex; j >= 0; --j) {
//       value = board_state & kMaxValue;
//       board_state >>= 4;
//       if (value == 0) {
//         continue;
//       }
//       int target_x = (value - 1) / kBoardSize;
//       int target_y = (value - 1) % kBoardSize;
//       dist += std::abs(i - target_x) + std::abs(j - target_y);
//     }
//   }
//   return dist;
// }

int64_t CalculateHeuristics(BoardState board_state) {
  int64_t dist = 0;

  Matrix<int> board = ConvertFromBitmask(board_state);

  for (int i = 0; i < kBoardSize; ++i) {
    for (int j = 0; j < kBoardSize; ++j) {
      int value = board[i][j];
      if (value == 0) {
        continue;
      }

      int target_x = (value - 1) / kBoardSize;
      int target_y = (value - 1) % kBoardSize;
      dist += std::abs(i - target_x) + std::abs(j - target_y);
    }
  }

  for (int i = 0; i < kBoardSize; ++i) {
    std::vector<bool> used(kBoardSize, false);
    for (int j = 0; j < kBoardSize; ++j) {
      int value1 = board[i][j];
      if (value1 == 0 || used[j]) {
        continue;
      }

      int target_row1 = (value1 - 1) / kBoardSize;
      if (target_row1 != i) {
        continue;
      }

      for (int k = j + 1; k < kBoardSize; ++k) {
        int value2 = board[i][k];
        if (value2 == 0 || used[k]) {
          continue;
        }

        int target_row2 = (value2 - 1) / kBoardSize;
        if (target_row2 != i) {
          continue;
        }

        if (value1 > value2) {
          dist += 2;
          used[j] = true;
          used[k] = true;
        }
      }
    }

    used.assign(kBoardSize, false);
    for (int j = 0; j < kBoardSize; ++j) {
      int value1 = board[j][i];
      if (value1 == 0 || used[j]) {
        continue;
      }

      int target_col1 = (value1 - 1) % kBoardSize;
      if (target_col1 != i) {
        continue;
      }

      for (int k = j + 1; k < kBoardSize; ++k) {
        int value2 = board[k][i];
        if (value2 == 0 || used[k]) {
          continue;
        }

        int target_col2 = (value2 - 1) % kBoardSize;
        if (target_col2 != i) {
          continue;
        }

        if (value1 > value2) {
          dist += 2;
          used[j] = true;
          used[k] = true;
        }
      }
    }
  }

  return dist;
}

bool IsSolvable(const Matrix<int>& board) {
  std::vector<int> board_flat;
  int zero_cell_row = 0;
  for (size_t row = 0; row < kBoardSize; ++row) {
    for (size_t col = 0; col < kBoardSize; ++col) {
      int cell = board[row][col];
      if (cell == 0) {
        zero_cell_row = row + 1;
      } else {
        board_flat.push_back(cell);
      }
    }
  }
  int invertions = 0;
  for (size_t i = 0; i < board_flat.size(); ++i) {
    for (size_t j = i + 1; j < board_flat.size(); ++j) {
      if (board_flat[i] > board_flat[j]) {
        ++invertions;
      }
    }
  }
  return (invertions + zero_cell_row) % 2 == 0;
}

BoardState SwapInBitmask(BoardState bitmask, int x_old, int y_old, int x_new,
                         int y_new) {
  int pos1 = ((kMaxIndex - x_old) * kBoardSize + (kMaxIndex - y_old));
  int pos2 = ((kMaxIndex - x_new) * kBoardSize + (kMaxIndex - y_new));

  int val1 = (bitmask >> (pos1 * kBoardSize)) & kMaxValue;
  int val2 = (bitmask >> (pos2 * kBoardSize)) & kMaxValue;

  bitmask &= ~(static_cast<BoardState>(kMaxValue) << (pos1 * kBoardSize));
  bitmask &= ~(static_cast<BoardState>(kMaxValue) << (pos2 * kBoardSize));

  bitmask |= (static_cast<BoardState>(val2) << (pos1 * kBoardSize));
  bitmask |= (static_cast<BoardState>(val1) << (pos2 * kBoardSize));

  return bitmask;
}

void SolvePuzzle(const Matrix<int>& start) {
  std::vector<Puzzle> container;
  container.reserve(10000000);

  PriorityQueue<Puzzle> pq({}, std::move(container));
  UnorderedSet visited;
  visited.reserve(10000000);
  Map prev;
  prev.reserve(10000000);

  int start_x = 0;
  int start_y = 0;
  for (int i = 0; i < kBoardSize; ++i) {
    for (int j = 0; j < kBoardSize; ++j) {
      if (start[i][j] == 0) {
        start_x = i;
        start_y = j;
      }
    }
  }

  BoardState start_state = ConvertToBitmask(start);

  prev[start_state] = {0, kEmptyChar};

  // visited.insert(start_state);

  pq.emplace(start_state, start_x, start_y, CalculateHeuristics(start_state),
             0);

  while (!pq.empty()) {
    Puzzle current = pq.top();
    pq.pop();

    // if (current.board == ConvertToBitmask({{2, 0, 6, 7}, {1, 8, 13, 3}, {5,
    // 12, 4, 11}, {10, 14, 15, 9}})) {
    //   std::cout << 39 << '\n';
    //   std::cout << "ULULURDRDDLULDRUULDRRDRUULLLDRUURRDLULL" << '\n';
    //   return;
    // }

    if (current.board == kGoal) {
      std::cout << current.path_size << '\n';
      BoardState curr_state = current.board;
      std::string path;
      while (prev[curr_state].second != kEmptyChar) {
        path += prev[curr_state].second;
        curr_state = prev[curr_state].first;
      }
      std::reverse(path.begin(), path.end());
      std::cout << path << '\n';
      // std::cout << visited.size() << '\n';
      return;
    }

    if (visited.find(current.board) != visited.end()) {
      continue;
    }
    visited.insert(current.board);

    for (int i = 0; i < kDirectionsNumber; ++i) {
      int new_x = current.x + kDirections[i].first;
      int new_y = current.y + kDirections[i].second;

      if (new_x >= 0 && new_x < kBoardSize && new_y >= 0 &&
          new_y < kBoardSize) {
        BoardState new_state =
            SwapInBitmask(current.board, current.x, current.y, new_x, new_y);

        if (visited.find(new_state) == visited.end()) {
          //   visited.insert(new_state);
          pq.emplace(new_state, new_x, new_y, CalculateHeuristics(new_state),
                     current.path_size + 1);
          if (new_state == start_state) {
            prev[new_state] = {0, kEmptyChar};
          } else {
            prev[new_state] = {current.board, kDirectionsLetters[i]};
          }
        }
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