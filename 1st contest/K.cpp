#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

void FindLCIS(std::vector<int>& first, std::vector<int>& second) {
  Matrix dp(first.size() + 1, std::vector<int>(second.size() + 1, 0));
  int result = 0;

  for (size_t i = 1; i <= first.size(); i++) {
    int best = 0;
    for (size_t j = 1; j <= second.size(); j++) {
      dp[i][j] = dp[i - 1][j];
      if (first[i - 1] == second[j - 1] && dp[i - 1][j] < best + 1) {
        dp[i][j] = best + 1;
      }
      if (first[i - 1] > second[j - 1] && dp[i - 1][j] > best) {
        best = dp[i - 1][j];
      }
    }
  }

  for (auto i : dp[first.size()]) {
    result = std::max(i, result);
  }
  std::cout << result << '\n';
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n;
  std::cin >> m;

  std::vector<int> first(n, 0);
  std::vector<int> second(m, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> first[i];
  }

  for (int i = 0; i < m; i++) {
    std::cin >> second[i];
  }

  FindLCIS(first, second);
}