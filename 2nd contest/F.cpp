#include <iostream>
#include <numeric>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

bool No2x2Blocks(int64_t mask, int64_t prev_mask, int64_t max_mask) {
  int64_t prev_1 = -1;
  int64_t prev_2 = -1;
  for (int64_t i = 0; i < int64_t(log2(max_mask)) + 1; ++i) {
    int64_t curr_1 = ((mask >> i) & 1);
    int64_t curr_2 = ((prev_mask >> i) & 1);
    if (prev_1 == prev_2 && curr_1 == curr_2 && curr_1 == prev_1) {
      return false;
    }
    prev_1 = curr_1;
    prev_2 = curr_2;
  }
  return true;
}

int64_t CountVariants(int64_t m, int64_t n) {
  int64_t max_mask = (1 << n) - 1;
  Matrix<int64_t> dp(m + 1, std::vector<int64_t>(max_mask + 1, 0));

  // dp[i][mask] == varaints of block distribution in first i rows and last row
  // is mask

  for (int64_t mask = 0; mask <= max_mask; ++mask) {
    dp[1][mask] = 1;
  }

  for (int64_t i = 2; i <= m; ++i) {
    for (int64_t mask = 0; mask <= max_mask; ++mask) {
      for (int64_t prev_mask = 0; prev_mask <= max_mask; ++prev_mask) {
        if (No2x2Blocks(mask, prev_mask, max_mask)) {
          dp[i][mask] += dp[i - 1][prev_mask];
        }
      }
    }
  }

  // for (int64_t i = 1; i <= m; ++i) {
  //   for (int64_t j = 0; j <= max_mask; ++j) {
  //     std::cout << dp[i][j] << ' ';
  //   }
  //   std::cout << '\n';
  // }

  return std::accumulate(dp[m].begin(), dp[m].end(), 0);
}

int main() {
  int64_t m = 0;
  int64_t n = 0;

  std::cin >> m;
  std::cin >> n;

  std::cout << CountVariants(std::max(m, n), std::min(m, n)) << '\n';

  // std::cout << No2x2Blocks(m, n) << '\n';

  // std::cout << (m ^ n) << '\n';

  return 0;
}
