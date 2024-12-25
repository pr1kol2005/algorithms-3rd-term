#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct CircularIndex {
  int value;
  int size;

  CircularIndex(int v, int s) : value(v), size(s) {}

  CircularIndex& operator++() {
    value = (value + 1) % size;
    return *this;
  }

  CircularIndex& operator--() {
    value = (value + size - 1) % size;
    return *this;
  }

  CircularIndex operator+(int n) const {
    return CircularIndex((value + n) % size, size);
  }

  CircularIndex operator-(int n) const {
    return CircularIndex((value - n + size) % size, size);
  }
};

std::ostream& operator<<(std::ostream& out, const CircularIndex& index) {
  out << index.value;
  return out;
}

void SolveProblem(const std::vector<int64_t>& missions) {
  Matrix<int> dp(missions.size() + 1, std::vector(missions.size() + 1, 0));
  Matrix<std::pair<int, int>> prev(
      missions.size() + 1,
      std::vector<std::pair<int, int>>(missions.size() + 1, {-1, -1}));

  //  CircularIndex = i - 1
  //  missions[i - 1]
  //  dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j] + 1

  for (int i = 1; i <= missions.size(); i++) {
    dp[1][i] = 1;
  }

  for (int i = 2; i <= missions.size(); i++) {
    for (int j = 1; j <= missions.size(); j++) {
      CircularIndex index_prev(j - 1, missions.size());
      --index_prev;
      if (dp[i][j] < dp[i - 1][index_prev.value + 1]) {
        dp[i][j] = dp[i - 1][index_prev.value + 1];
        prev[i][j] = prev[i - 1][index_prev.value + 1];
      }

      CircularIndex index(j - 1, missions.size());
      for (int k = 1; k < i; k++) {
        --index;
        // std::cout << i << '|' << j << '|' << i - k << ' ' << index.value + 1 << '\n';
        if (missions[j - 1] % missions[index.value] == 0) {
          // std::cout << dp[i][j] << ' ' << dp[i - k][index.value + 1] + 1 << '\n';
          if (dp[i][j] <= dp[i - k][index.value + 1] + 1) {
            dp[i][j] = dp[i - k][index.value + 1] + 1;
            prev[i][j] = {i - k, index.value + 1};
          }
        }
      }
    }
  }

  for (int i = 0; i < dp.size(); i++) {
    for (int j = 0; j < dp[0].size(); j++) {
      std::cout << dp[i][j] << ' ';
    }
    std::cout << '\n';
  }

  std::vector<int> result;
  int max_missions = 0;
  int prev_i = -1;
  int prev_j = -1;

  for (int j = 1; j <= missions.size(); j++) {
    if (max_missions < dp[missions.size()][j]) {
      max_missions = dp[missions.size()][j];
      prev_i = missions.size();
      prev_j = j;
    }
  }

  std::cout << max_missions << '\n';

  for (int i = 0; i < prev.size(); i++) {
    for (int j = 0; j < prev[0].size(); j++) {
      std::cout << prev[i][j].first << ' ' << prev[i][j].second << '|';
    }
    std::cout << '\n';
  }

  while (prev_j != -1) {
    result.push_back(prev_j);
    // std::cout << prev_i << ' ' << prev_j << '\n';
    int last_prev_i = prev_i;
    prev_i = prev[prev_i][prev_j].first;
    prev_j = prev[last_prev_i][prev_j].second;
  }

  // std::cout << prev_i << ' ' << prev_j << '\n';

  std::reverse(result.begin(), result.end());
  for (int i = 0; i < result.size(); i++) {
    std::cout << result[i] << ' ';
  }
  std::cout << '\n';
}

int main() {
  int n = 0;

  std::cin >> n;

  std::vector<int64_t> missions(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> missions[i];
  }

  SolveProblem(missions);
}