#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

struct CircularIndex{
  int value;
  int size;

  CircularIndex(int v, int s) : value(v), size(s) {}

  CircularIndex& operator++() {
    value = (value + 1) % size;
    return *this;
  }

  CircularIndex& operator--() {
    value = (value + size - 1) % size;
    return *this;
  }

  CircularIndex operator+(int n) const {
    return CircularIndex((value + n) % size, size);
  }

  CircularIndex operator-(int n) const {
    return CircularIndex((value - n + size) % size, size);
  }
};


void SolveProblem(const std::vector<int64_t>& missions) {
  return;
}


int main() {
  int n = 0;
  std::vector<int64_t> missions(n, 0);

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    std::cin >> missions[i];
  }

  std::cout << -2 % 3 << '\n';

  // SolveProblem(missions);
}