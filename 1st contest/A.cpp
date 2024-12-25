#include <iostream>

int64_t Fibonacci(int64_t n) {
  int64_t first = 0;
  int64_t second = 1;
  int64_t result = 0;

  if (n <= 1) {
    return n;
  }

  for(int64_t i = 2; i <= n; i++) {
    result = (first % 1000003 + second % 1000003);
    first = second % 1000003;
    second = result % 1000003;
  }

  return result % 1000003;
}

int PisanoPeriod(int m) {
  int current = 0;
  int next = 1;
  int period = 0;
  int old_next = 0;
  while (true) {
    old_next = next;
    next = (current + next) % m;
    current = old_next;
    period = period + 1;
    if (current == 0 && next == 1) {
      return period;
    }
  }
}

int main() {
  int64_t n = 0;
  int64_t p = 0;

  std::cin >> n;

  p = PisanoPeriod(1000003);

  // std::cout << p << '\n';

  std::cout << Fibonacci((n - 1) % p) << '\n';

  return 0;
}