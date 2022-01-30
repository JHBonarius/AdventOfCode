#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

// O(N^2), could be improved

// calculates sum of all integers up to n, i.e. n=4 -> 1+2+3+4 = 10 (= (4*5)/2)
static constexpr auto gauss(std::integral auto n) { return (n * (n + 1)) / 2; }

int main() {
  auto const horizontal_postitions{[]() {
    auto horizontal_postitions{std::vector<int>{}};
    auto fs{std::ifstream{"input"}};
    if (!fs.is_open()) throw std::ios_base::failure("file not found");
    int val;
    while (fs >> val) {
      horizontal_postitions.push_back(val);
      if (fs.peek() == ',') {
        fs.ignore();
      }
    }
    return horizontal_postitions;
    // return std::vector<int>{16, 1, 2, 0, 4, 2, 7, 1, 2, 14};
  }()};
  // std::cout << std::size(horizontal_postitions) << std::endl;

  // std::cout << std::accumulate(cbegin(horizontal_postitions),
  //                              cend(horizontal_postitions), 0) /
  //                  std::size(horizontal_postitions)
  //           << std::endl;

  auto const distances{[&]() {
    auto distances{std::vector<int>(*std::max_element(
        cbegin(horizontal_postitions), cend(horizontal_postitions)))};
    std::iota(begin(distances), end(distances), 0);
    return distances;
  }()};

  auto const fuel_consumptions{[&]() {
    auto fuel_consumptions{std::vector<uint64_t>{}};
    fuel_consumptions.reserve(size(distances));
    std::transform(
        cbegin(distances), cend(distances), back_inserter(fuel_consumptions),
        [&](auto const distance) {
          return std::transform_reduce(
              cbegin(horizontal_postitions), cend(horizontal_postitions),
              uint64_t{0}, std::plus<>{}, [=](auto const horizontal_postition) {
                return gauss(std::abs(distance - horizontal_postition));
              });
        });
    return fuel_consumptions;
  }()};

  auto const least_fuel{
      *std::min_element(cbegin(fuel_consumptions), cend(fuel_consumptions))};
  std::cout << "Least fuel consumption: " << least_fuel << std::endl;
}
