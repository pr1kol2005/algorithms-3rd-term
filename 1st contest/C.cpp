#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using PairVector = std::vector<std::pair<int, std::string>>;

const int kTaxiCapacity = 4;

void InputData(PairVector& data) {
  int n = 0;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    std::string name;
    int dist;

    std::cin >> name;
    std::cin >> dist;

    data.emplace_back(dist, name);
  }

  std::sort(data.begin(), data.end());
  std::reverse(data.begin(), data.end());
}

void ManageRides(const PairVector& boys, const PairVector& girls,
                 Matrix<std::string>& rides, int64_t& min_dist) {
  Matrix<int> dp(boys.size() + 1, std::vector(girls.size() + 1, INT32_MAX));
  Matrix<std::pair<int, int>> prev(
      boys.size() + 1,
      std::vector<std::pair<int, int>>(girls.size() + 1, {-1, -1}));

  dp[0][0] = 0;

  for (size_t i = 0; i <= boys.size(); i++) {
    for (size_t j = 0; j <= girls.size(); j++) {
      if (dp[i][j] == INT32_MAX) {
        continue;
      }

      for (int boys_taken = 1;
           boys_taken <= kTaxiCapacity && boys_taken + i <= boys.size();
           boys_taken++) {
        for (int girls_taken = 0; girls_taken <= kTaxiCapacity - boys_taken &&
                                  girls_taken + j <= girls.size();
             girls_taken++) {
          int current_dist = 0;

          for (int k = 0; k < boys_taken; k++) {
            current_dist = std::max(current_dist, boys[i + k].first);
          }
          for (int k = 0; k < girls_taken; k++) {
            current_dist = std::max(current_dist, girls[j + k].first);
          }

          int next_i = i + boys_taken;
          int next_j = j + girls_taken;
          int new_dist = dp[i][j] + current_dist;

          if (new_dist < dp[next_i][next_j]) {
            dp[next_i][next_j] = new_dist;
            prev[next_i][next_j] = {i, j};
          }
        }
      }
    }
  }

  // for (int i = 0; i < dp.size(); i++) {
  //   for (int j = 0; j < dp[i].size(); j++) {
  //     std::cout << dp[i][j] << ' ';
  //   }
  //   std::cout << '\n';
  // }

  min_dist = dp[boys.size()][girls.size()];

  int i = boys.size();
  int j = girls.size();

  while (i > 0 || j > 0) {
    std::vector<std::string> ride;
    std::pair<int, int> current_ride = prev[i][j];
    int boys_taken = i - current_ride.first;
    int girls_taken = j - current_ride.second;

    for (int k = 0; k < boys_taken; k++) {
      ride.push_back(boys[current_ride.first + k].second);
    }
    for (int k = 0; k < girls_taken; k++) {
      ride.push_back(girls[current_ride.second + k].second);
    }

    rides.push_back(ride);
    i = current_ride.first;
    j = current_ride.second;
  }
}

void OutputRides(const Matrix<std::string>& rides) {
  std::cout << rides.size() <<"\n";

  for (int i = 0; i < rides.size(); i++) {
    std::cout << "Taxi " << i + 1 << ": ";
    for (int j = 0; j < rides[i].size(); j++) {
      std::cout << rides[i][j];
      if (j == rides[i].size() - 1) {
        std::cout << ".";
      } else if (j == rides[i].size() - 2) {
        std::cout << " and ";
      } else {
        std::cout << ", ";
      }
    }
    std::cout << '\n';
  }
}


int main() {
  PairVector boys;
  PairVector girls;

  InputData(boys);
  InputData(girls);

  Matrix<std::string> rides;
  int64_t min_dist = 0;

  ManageRides(boys, girls, rides, min_dist);

  std::cout << min_dist << '\n';

  OutputRides(rides);
}
