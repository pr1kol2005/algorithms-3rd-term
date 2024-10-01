#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct LcsData {
  std::vector<size_t> lcs_first;
  std::vector<size_t> lcs_second;
};

void PrintLCS(const std::vector<size_t>& lcs) {
  for (auto i = lcs.begin(); i < lcs.end(); i++) {
    std::cout << *i + 1 << ' ';
  }
  std::cout << '\n';
}

void FindLCS(const std::string& str_first, const std::string& str_second,
             LcsData& answer) {
  Matrix<size_t> dp_lcs_of_prefixes(
      str_first.size() + 1, std::vector<size_t>(str_second.size() + 1, 0));

  for (size_t i = 1; i <= str_first.size(); i++) {
    for (size_t j = 1; j <= str_second.size(); j++) {
      if (str_first[i - 1] == str_second[j - 1]) {
        dp_lcs_of_prefixes[i][j] = dp_lcs_of_prefixes[i - 1][j - 1] + 1;
      } else {
        dp_lcs_of_prefixes[i][j] = std::max(dp_lcs_of_prefixes[i][j - 1],
                                            dp_lcs_of_prefixes[i - 1][j]);
      }
    }
  }

  size_t current_elem_first = str_first.size();
  size_t current_elem_second = str_second.size();

  while (current_elem_first > 0 && current_elem_second > 0) {
    if (str_first[current_elem_first - 1] ==
        str_second[current_elem_second - 1]) {
      answer.lcs_first.push_back(current_elem_first - 1);
      answer.lcs_second.push_back(current_elem_second - 1);
      --current_elem_first;
      --current_elem_second;
    } else if (dp_lcs_of_prefixes[current_elem_first - 1][current_elem_second] >
               dp_lcs_of_prefixes[current_elem_first]
                                 [current_elem_second - 1]) {
      --current_elem_first;
    } else {
      --current_elem_second;
    }
  }

  std::reverse(answer.lcs_first.begin(), answer.lcs_first.end());
  std::reverse(answer.lcs_second.begin(), answer.lcs_second.end());
}

int main() {
  std::string str_first;
  std::string str_second;
  LcsData answer;

  std::cin >> str_first;
  std::cin >> str_second;

  FindLCS(str_first, str_second, answer);

  std::cout << answer.lcs_first.size() << '\n';

  PrintLCS(answer.lcs_first);
  PrintLCS(answer.lcs_second);
}