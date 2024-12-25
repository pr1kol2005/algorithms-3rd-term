#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int64_t>>;

void CountSets(int n) {
  Matrix dp(n + 1, std::vector<int64_t>(n + 1, 0));
  int64_t result = 0;

  //  a >= 2*b -- not interesting
  //  dp[i][j] -- number of peaceful sets of power i with max element j

  for (size_t i = 1; i <= n; i++) {
    for (size_t j = 1; j <= n; j++) {
      if (i == j) {
        dp[i][j] = 1;
      }
      if (dp[i][j] != 0) {
        for (size_t k = 2 * j; k <= n - i; k++) {
          dp[i + k][k] += dp[i][j];
        }
      }
    }
  }

  for (auto& i : dp[n]) {
    result += i;
  }

  std::cout << result << '\n';
}

int main() {
  int n = 0;

  std::cin >> n;

  CountSets(n);
}