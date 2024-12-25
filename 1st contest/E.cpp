#include <stdint.h>

#include <iostream>
#include <numeric>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

void SolveProblem(std::vector<int>& houses, int n, int m) {
  Matrix dp(n + 1, std::vector<int>(m + 1, INT32_MAX));
  Matrix prev(n + 1, std::vector<int>(m + 1, 0));

  //  dp[i][j] -- optimal sum in prefix of  i for j points and last is point

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      if (i == j) {
        dp[i][j] = 0;
        prev[i][j] = i - 1;
        break;
      }
      if (j == 1) {
        dp[i][j] = 0;
        for (int m = 1; m < i; m++) {
          dp[i][j] += houses[i - 1] - houses[m - 1];
        }
        continue;
      }
      for (int k = j - 1; k < i; k++) {
        int sum = 0;
        for (int l = k + 1; l < i; l++) {
          sum += std::min(houses[l - 1] - houses[k - 1],
                          houses[i - 1] - houses[l - 1]);
        }
        if (dp[k][j - 1] + sum < dp[i][j]) {
          dp[i][j] = dp[k][j - 1] + sum;
          prev[i][j] = k;
        }
      }
    }
  }

  int result = INT32_MAX;
  int current_house = 0;
  std::vector<int> point_of_influence(m);

  for (int i = m; i <= n; i++) {
    for (int j = i + 1; j <= n; j++) {
      dp[i][m] += houses[j - 1] - houses[i - 1];
    }
    if (dp[i][m] < result) {
      result = dp[i][m];
      current_house = i;
    }
  }

  std::cout << result << '\n';

  for (int i = 0; i < m; i++) {
    point_of_influence[m - i - 1] = houses[current_house - 1];
    current_house = prev[current_house][m - i];
  }

  for (auto it = point_of_influence.begin(); it < point_of_influence.end(); it++) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;
}

int main() {
  int n = 0;
  int m = 0;

  std::cin >> n;
  std::cin >> m;

  std::vector<int> houses(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> houses[i];
  }

  SolveProblem(houses, n, m);
}