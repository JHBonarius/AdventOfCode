#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>

int main() {
  std::array<uint64_t, 9> fishgroups{};
  {
    std::ifstream fs{"input"};
    int val;
    while (fs >> val) {
      fishgroups[val] += 1;
      if (fs.peek() == ',') {
        fs.ignore();
      }
    }
  }
  // fish = {3, 4, 3, 1, 2};
  for (auto n{0}; n < 256; ++n) {
    std::rotate(begin(fishgroups), std::next(begin(fishgroups), 1),
                end(fishgroups));
    fishgroups[6] += fishgroups[8];
  }
  auto const total = std::reduce(cbegin(fishgroups), cend(fishgroups));
  std::cout << "total fish after 80: " << total << '\n';
}