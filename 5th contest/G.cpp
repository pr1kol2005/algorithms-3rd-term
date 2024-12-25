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
const char kBeginChar = 'x';

struct Puzzle {
  BoardState board;
  int x;              // x coordinate of empty cell
  int y;              // y coordinate of empty cell
  int64_t dist;       // sum of Manhattan distances and Linear conflicts
  int64_t path_size;  // number of made moves
  std::vector<uint64_t> path;  // encoded sequence of moves

  Puzzle(BoardState b, int xx, int yy, int64_t d, int64_t p,
         const std::vector<uint64_t>& pth)
      : board(b), x(xx), y(yy), dist(d), path_size(p), path(pth) {}

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
    int max_value = -1;
    for (int j = 0; j < kBoardSize; ++j) {
      int value = board[i][j];
      if (value != 0 && (value - 1) / kBoardSize == i) {
        if (value > max_value) {
          max_value = value;
        } else {
          dist += 2;
        }
      }
    }
  }

  for (int i = 0; i < kBoardSize; ++i) {
    int max_value = -1;
    for (int j = 0; j < kBoardSize; ++j) {
      int value = board[j][i];
      if (value != 0 && (value - 1) % kBoardSize == i) {
        if (value > max_value) {
          max_value = value;
        } else {
          dist += 2;
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
  int inversions = 0;
  for (size_t i = 0; i < board_flat.size(); ++i) {
    for (size_t j = i + 1; j < board_flat.size(); ++j) {
      if (board_flat[i] > board_flat[j]) {
        ++inversions;
      }
    }
  }
  return (inversions + zero_cell_row) % 2 == 0;
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

void AddMove(std::vector<uint64_t>& path, int move_bits, int current_path_size) {
  if (path.empty() || current_path_size % 32 == 0) {
    path.push_back(0);
  }
  path.back() = (path.back() << 2) | static_cast<uint64_t>(move_bits);
}

std::string DecodePath(const std::vector<uint64_t>& path) {
  std::string decoded;
  for (uint64_t block : path) {
    for (int i = 62; i >= 0; i -= 2) {
      int move = (block >> i) & 0b11;
      decoded += kDirectionsLetters[move];
    }
  }
  return decoded;
}

std::string TrimPath(const std::string& path, int path_size) {
  int prefix_length = (path_size / 32) * 32;
  int suffix_length = path_size % 32;

  return path.substr(0, prefix_length) +
         path.substr(path.size() - suffix_length);
}

void SolvePuzzle(const Matrix<int>& start) {
  std::vector<Puzzle> container;
  container.reserve(10000000);

  PriorityQueue<Puzzle> pq({}, std::move(container));
  UnorderedSet visited;
  visited.reserve(10000000);

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
  pq.emplace(start_state, start_x, start_y, CalculateHeuristics(start_state), 0,
             std::vector<uint64_t>{});

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();

    if (current.board == kGoal) {
      std::cout << current.path_size << '\n';
      std::cout << TrimPath(DecodePath(current.path), current.path_size) << '\n';
      return;
    }

    if (!visited.insert(current.board).second) {
      continue;
    }
    visited.insert(current.board);

    for (int i = 0; i < kDirectionsNumber; ++i) {
      int new_x = current.x + kDirections[i].first;
      int new_y = current.y + kDirections[i].second;

      if (new_x < 0 || new_x > kMaxIndex || new_y < 0 || new_y > kMaxIndex) {
        continue;
      }

      BoardState new_state =
          SwapInBitmask(current.board, current.x, current.y, new_x, new_y);
      if (visited.contains(new_state)) {
        continue;
      }

      std::vector<uint64_t> new_path = current.path;
      AddMove(new_path, i, current.path_size);
      pq.emplace(new_state, new_x, new_y, CalculateHeuristics(new_state),
                 current.path_size + 1, new_path);
    }
  }

  std::cout << -1 << '\n';
}

int main() {
  Matrix<int> board = InputBoard();
  if (IsSolvable(board)) {
    SolvePuzzle(board);
  } else {
    std::cout << -1 << '\n';
  }
  return 0;
}
