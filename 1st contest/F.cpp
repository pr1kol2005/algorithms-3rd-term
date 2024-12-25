#include <cmath>
#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

int MinAttempts(int n, int k) {
  int needed_planes = std::min(static_cast<int>(log2(n)) + 1, k + 1);
  Matrix dp(n + 1, std::vector(needed_planes, 0));

  //  dp[i][j] = max checked floors, i -- attempts, j -- planes
  //  dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j] + 1

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j < dp[0].size(); j++) {
      dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j] + 1;
      //  std::cout << dp[i][j] << ' ' << i << ' ' << j << '\n';
    }
  }

  int result = -1;

  for (int i = 0; i <= n; i++) {
    if (n - 1 <= dp[i][needed_planes - 1]) {
      result = i;
      break;
    }
  }

  return result;
}

int main() {
  int n = 0;
  int k = 0;

  std::cin >> n;
  std::cin >> k;

  std::cout << MinAttempts(n, k) << '\n';
}