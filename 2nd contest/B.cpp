#include <cstdint>
#include <iostream>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using Cube = Matrix<std::vector<T>>;

int LevenshteinDistance(const std::string& str1, const std::string& str2) {
  std::vector<std::vector<int>> dp(str1.size() + 1,
                                   std::vector<int>(str2.size() + 1, 0));

  for (int i = 0; i <= str1.size(); ++i) {
    for (int j = 0; j <= str2.size(); ++j) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (j == 0) {
        dp[i][j] = i;
      } else if (str1[i - 1] == str2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + std::min(dp[i - 1][j],
                                std::min(dp[i][j - 1], dp[i - 1][j - 1]));
      }
    }
  }

  return dp[str1.size()][str2.size()];
}

int MinHammingDist(const std::string& str1, const std::string& str2, int k) {
  size_t max_len = std::max(str1.size(), str2.size());

  Cube<int> dp(max_len + 1,
               Matrix<int>(max_len + 1, std::vector<int>(k + 2, INT32_MAX)));
  Cube<int> done_changes(max_len + 1,
                         Matrix<int>(max_len + 1, std::vector<int>(k + 2, 0)));

  // dp[i][j][k] == hmin hamming distance from str1[0..i - 1] to str2[0..j - 1]
  // with k changes

  dp[0][0][0] = 0;

  for (int i = 0; i <= str1.size(); ++i) {
    for (int j = 0; j <= str2.size(); ++j) {
      for (int l = 0; l <= k; ++l) {
        if (dp[i][j][l] == INT32_MAX) {
          continue;
        }
        bool flag = false;
        if (i < str1.size() && l < k) {
          if (flag) {
            std::cout << "1\n";
          }
          dp[i + 1][j][l + 1] = std::min(dp[i + 1][j][l + 1], dp[i][j][l]);
        } 
        if (j < str2.size() && l < k) {
          if (flag) {
            std::cout << "2\n";
          }
          dp[i][j + 1][l + 1] = std::min(dp[i][j + 1][l + 1], dp[i][j][l]);
        }
        if (i >= str1.size() || j >= str2.size()) {
          continue;
        }
        if (str1[i] == str2[j]) {
          if (flag) {
            std::cout << "3\n";
          }
          dp[i + 1][j + 1][l] = std::min(dp[i + 1][j + 1][l], dp[i][j][l]);
        } else {
          if (l < k) {
            if (flag) {
              std::cout << "4\n";
            }
            dp[i + 1][j + 1][l + 1] =
                std::min(dp[i + 1][j + 1][l + 1], dp[i][j][l]);
          }
          if (flag) {
            std::cout << "5\n";
          }
          dp[i + 1][j + 1][l] = std::min(dp[i + 1][j + 1][l], dp[i][j][l] + 1);
        }
      }
    }
  }

  return dp[str1.size()][str2.size()][k];
}

int MinPositions(const std::string& alpha, const std::string& beta, int k) {
  int len_diff =
      std::abs(static_cast<int>(alpha.size()) - static_cast<int>(beta.size()));
  int min_ham_dist = MinHammingDist(alpha, beta, k);
  int lev_dist = LevenshteinDistance(alpha, beta);

  if (len_diff <= k) {
    if (lev_dist <= k) {
      return std::max(std::min(min_ham_dist, lev_dist - k), 0);
    }
    return min_ham_dist;
  } else {
    return -1;
  }
}

int main() {
  std::string alpha;
  std::string beta;
  int k = 0;

  std::cin >> alpha;
  std::cin >> beta;
  std::cin >> k;

  std::cout << MinPositions(alpha, beta, k) << '\n';

  return 0;
}