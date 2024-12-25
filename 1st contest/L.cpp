#include <algorithm>
#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

void PrintAnswer(Matrix& dp, std::vector<int>& weights, size_t i, size_t j) {
  if (dp[i][j] == 0) {
    return;
  } else if (dp[i][j] != dp[i - 1][j]) {
    std::cout << i << '\n';
    PrintAnswer(dp, weights, i - 1, j - weights[i - 1]);
  } else {
    PrintAnswer(dp, weights, i - 1, j);
  }
}

void SolveKnapsackProblem(std::vector<int>& weights, std::vector<int>& prices,
                          int max_weight) {
  Matrix dp(weights.size() + 1, std::vector<int>(max_weight + 1, 0));

  for (size_t i = 1; i <= weights.size(); i++) {
    for (size_t j = 1; j <= static_cast<size_t>(max_weight); j++) {
      if (j >= weights[i - 1]) {
        dp[i][j] = std::max(dp[i - 1][j],
                            dp[i - 1][j - weights[i - 1]] + prices[i - 1]);
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }

  PrintAnswer(dp, weights, weights.size(), static_cast<size_t>(max_weight));

  // std::cout << dp[weights.size()][max_weight] << '\n';
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n;
  std::cin >> m;

  std::vector<int> resources(n);  // weights
  std::vector<int> rewards(n);    // prices

  for (int i = 0; i < n; i++) {
    std::cin >> resources[i];
  }

  for (int i = 0; i < n; i++) {
    std::cin >> rewards[i];
  }

  SolveKnapsackProblem(resources, rewards, m);
}