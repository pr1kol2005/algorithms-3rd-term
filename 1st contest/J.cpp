#include <iostream>
#include <utility>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using Pair = std::pair<size_t, size_t>;

void FindLCS(std::string& first, std::string& second) {
  first.shrink_to_fit();
  second.shrink_to_fit();

  Matrix<size_t> dp(first.size() + 1,
                    std::vector<size_t>(second.size() + 1, 0));
  Matrix<Pair> prev(first.size() + 1, std::vector<Pair>(second.size() + 1));

  for (size_t i = 1; i <= first.size(); i++) {
    for (size_t j = 1; j <= second.size(); j++) {
      if (first[i - 1] == second[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
        prev[i][j] = Pair(i - 1, j - 1);
      } else {
        dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
        prev[i][j] =
            (dp[i][j - 1] >= dp[i - 1][j]) ? Pair(i, j - 1) : Pair(i - 1, j);
      }
    }
  }

  std::vector<size_t> lcs_first;
  std::vector<size_t> lcs_second;
  size_t k = first.size();
  size_t l = second.size();

  while (k > 0 && l > 0) {
    if (first[k - 1] == second[l - 1]) {
      lcs_first.push_back(k - 1);
      lcs_second.push_back(l - 1);
      --k;
      --l;
    } else if (dp[k - 1][l] > dp[k][l - 1]) {
      --k;
    } else {
      --l;
    }
  }

  std::cout << dp[first.size()][second.size()] << '\n';

  for (auto i = lcs_first.rbegin(); i < lcs_first.rend(); i++) {
    std::cout << *i + 1 << ' ';
  }
  std::cout << '\n';

  for (auto i = lcs_second.rbegin(); i < lcs_second.rend(); i++) {
    std::cout << *i + 1 << ' ';
  }
}

int main() {
  std::string first;
  std::string second;

  std::cin >> first;
  std::cin >> second;

  FindLCS(first, second);
}