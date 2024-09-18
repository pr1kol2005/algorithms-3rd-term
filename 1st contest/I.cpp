#include <iostream>
#include <vector>

void PrintAnswer(std::vector<int>& dp, std::vector<int>& blocks) {
  std::cout << dp[blocks.size()] << '\n';
  for (size_t i = 1; i < blocks.size(); i++) {
    if (dp[i] < dp[i + 1]) {
      std::cout << blocks[i - 1] << ' ';
    }
  }
  std::cout << *(blocks.end() - 1) << '\n';
}

void CountConqueredBlocks(std::vector<int>& blocks) {
  std::vector<int> dp_1(blocks.size() + 1, 1);
  std::vector<int> dp_2(blocks.size() + 1, 1);

  for (size_t i = 2; i <= blocks.size(); i++) {
    for (size_t j = i - 1; j >= 1; --j) {
      if ((dp_1[j] % 2 == 0 && blocks[j - 1] > blocks[i - 1]) ||
          (dp_1[j] % 2 != 0 && blocks[j - 1] < blocks[i - 1])) {
        dp_1[i] = std::max(dp_1[j] + 1, dp_1[i]);
      }
      if ((dp_2[j] % 2 == 0 && blocks[j - 1] < blocks[i - 1]) ||
          (dp_2[j] % 2 != 0 && blocks[j - 1] > blocks[i - 1])) {
        dp_2[i] = std::max(dp_2[j] + 1, dp_2[i]);
      }
    }
  }

  if (dp_1[blocks.size()] > dp_2[blocks.size()]) {
    PrintAnswer(dp_1, blocks);
  } else {
    PrintAnswer(dp_2, blocks);
  }
}

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<int> blocks(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> blocks[i];
  }

  CountConqueredBlocks(blocks);
}