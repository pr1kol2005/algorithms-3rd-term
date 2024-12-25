#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

void FindLIS(std::vector<int>& sequence, int n) {
  std::vector<int> dp(n + 1, std::numeric_limits<int>::max());
  std::vector<size_t> pos(n + 1);
  std::vector<int> prev(n + 1);
  size_t length = 0;
  size_t ub = 0;  // upper_bound of dp

  std::reverse(sequence.begin(), sequence.end());
  dp[0] = std::numeric_limits<int>::min();
  pos[0] = -1;

  for (int i = 0; i < n; i++) {
    ub = std::upper_bound(dp.begin(), dp.end(), sequence[i]) - dp.begin();
    if (dp[ub - 1] <= sequence[i] && sequence[i] <= dp[ub]) {
      dp[ub] = sequence[i];
      pos[ub] = i;
      prev[i] = pos[ub - 1];
      length = std::max(length, ub);
    }
  }

  std::vector<int> answer;
  size_t last = pos[length];  // last element in inverted LIS

  while (last != -1) {
    answer.push_back(sequence.size() - last);
    last = prev[last];
  }

  std::cout << length << '\n';

  for (auto& i : answer) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

int main() {
  int n = 0;
  std::cin >> n;
  std::vector<int> sequence(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> sequence[i];
  }

  FindLIS(sequence, n);
}