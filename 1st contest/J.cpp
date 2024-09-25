#include <iostream>
#include <utility>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void PrintLCS(const std::vector<size_t>& lcs) {
  for (auto i = lcs.rbegin(); i < lcs.rend(); i++) {
    std::cout << *i + 1 << ' ';
  }
  std::cout << '\n';
}

void FindLCS(const std::string& first, const std::string& second,
             std::vector<size_t>& lcs_first, std::vector<size_t>& lcs_second) {
  Matrix<size_t> dp_lcs_of_prefixes(first.size() + 1,
                                    std::vector<size_t>(second.size() + 1, 0));

  for (size_t i = 1; i <= first.size(); i++) {
    for (size_t j = 1; j <= second.size(); j++) {
      if (first[i - 1] == second[j - 1]) {
        dp_lcs_of_prefixes[i][j] = dp_lcs_of_prefixes[i - 1][j - 1] + 1;
      } else {
        dp_lcs_of_prefixes[i][j] = std::max(dp_lcs_of_prefixes[i][j - 1],
                                            dp_lcs_of_prefixes[i - 1][j]);
      }
    }
  }

  size_t k = first.size();
  size_t l = second.size();

  while (k > 0 && l > 0) {
    if (first[k - 1] == second[l - 1]) {
      lcs_first.push_back(k - 1);
      lcs_second.push_back(l - 1);
      --k;
      --l;
    } else if (dp_lcs_of_prefixes[k - 1][l] > dp_lcs_of_prefixes[k][l - 1]) {
      --k;
    } else {
      --l;
    }
  }
}

int main() {
  std::string first;
  std::string second;
  std::vector<size_t> lcs_first;
  std::vector<size_t> lcs_second;

  std::cin >> first;
  std::cin >> second;

  FindLCS(first, second, lcs_first, lcs_second);

  std::cout << lcs_first.size() << '\n';

  PrintLCS(lcs_first);
  PrintLCS(lcs_second);
}