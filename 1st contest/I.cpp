#include <iostream>
#include <vector>

void PrintAnswer(const std::vector<int>& result) {
  std::cout << result.size() << '\n';
  for (auto i : result) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

std::vector<int> FindOutResult(const std::vector<int>& dp,
                               const std::vector<int>& blocks) {
  std::vector<int> result;

  for (size_t i = 1; i < blocks.size(); i++) {
    if (dp[i] < dp[i + 1]) {
      result.push_back(blocks[i - 1]);
    }
  }
  result.push_back(blocks.back());

  return result;
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
    return FindOutResult(dp_less_more, blocks);
  }
  return FindOutResult(dp_more_less, blocks);
}

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<int> blocks(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> blocks[i];
  }

  PrintAnswer(CountConqueredBlocks(blocks));
}