#include <iostream>
#include <set>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Item {
  int weight = 0;
  int price = 0;

  Item(int w, int p) : weight(w), price(p) {}
};

void InputData(int n, Matrix<Item>& grous) {
  for (int i = 0; i < n; i++) {
    int w = 0;
    int p = 0;
    int g = 0;

    std::cin >> w;
    std::cin >> p;
    std::cin >> g;

    grous[g - 1].push_back(Item(w, p));
  }
}

// void OutputData(const Matrix<Item>& groups) {
//   for (auto& group : groups) {
//     for (auto& item : group) {
//       std::cout << item.weight << ' ' << item.price << " | ";
//     }
//     std::cout << '\n';
//   }
// }

void SolveKnapsackProblem(const Matrix<Item>& groups, int max_weight) {
  std::vector<int> dp(max_weight + 1, 0);

  // dp[w] == max price in knapsack with weight w

  for (int i = 0; i < groups.size(); i++) {
    std::vector<int> group_dp = dp;

    for (auto& item : groups[i]) {
      for (int j = max_weight; j >= item.weight; --j) {
        group_dp[j] = std::max(group_dp[j], dp[j - item.weight] + item.price);
      }
    }

    dp = group_dp;
  }

  std::cout << dp[max_weight] << '\n';
}

int main() {
  int n = 0;
  int g = 0;
  int k = 0;
  std::cin >> n;
  std::cin >> g;
  std::cin >> k;

  Matrix<Item> groups(g);

  InputData(n, groups);

  //  OutputData(groups);

  SolveKnapsackProblem(groups, k);
}