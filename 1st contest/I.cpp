#include <iostream>
#include <vector>

void PrintAnswer(const std::vector<int>& dp, const std::vector<int>& blocks) {
  std::cout << dp[blocks.size()] << '\n';
  for (size_t i = 1; i < blocks.size(); i++) {
    if (dp[i] < dp[i + 1]) {
      std::cout << blocks[i - 1] << ' ';
    }
  }
  std::cout << *(blocks.end() - 1) << '\n';
}

std::vector<int> CountConqueredBlocks(const std::vector<int>& blocks) {
  std::vector<int> dp_less_more(blocks.size() + 1, 1);
  std::vector<int> dp_more_less(blocks.size() + 1, 1);

  for (size_t i = 2; i <= blocks.size(); i++) {
    for (size_t j = i - 1; j >= 1; --j) {
      if ((dp_less_more[j] % 2 == 0 && blocks[j - 1] > blocks[i - 1]) ||
          (dp_less_more[j] % 2 != 0 && blocks[j - 1] < blocks[i - 1])) {
        dp_less_more[i] = std::max(dp_less_more[j] + 1, dp_less_more[i]);
      }
      if ((dp_more_less[j] % 2 == 0 && blocks[j - 1] < blocks[i - 1]) ||
          (dp_more_less[j] % 2 != 0 && blocks[j - 1] > blocks[i - 1])) {
        dp_more_less[i] = std::max(dp_more_less[j] + 1, dp_more_less[i]);
      }
    }
  }

  if (dp_less_more[blocks.size()] > dp_more_less[blocks.size()]) {
    return dp_less_more;
  }
  return dp_more_less;
}

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<int> blocks(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> blocks[i];
  }

  PrintAnswer(CountConqueredBlocks(blocks), blocks);
}