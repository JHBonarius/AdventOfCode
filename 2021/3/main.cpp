#include <algorithm>
#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

static constexpr auto N = 12;
// static constexpr auto N = 5;

using BinaryData = std::bitset<N>;
using DataIntArray = std::array<int, N>;

template <typename T>
auto readinputdata() {
  std::vector<T> data;
  std::ifstream fs("input");
  T val;
  while (fs >> val) {
    data.push_back(val);
  }
  return data;
}

auto map(BinaryData b) {
  DataIntArray d;
  for (int i = 0; i < N; ++i) {
    d[i] = static_cast<int>(b[i]) * 2 - 1;
  }
  return d;
}

auto resolve(DataIntArray d) {
  BinaryData b;
  for (int i = 0; i < N; ++i) {
    b[i] = d[i] >= 0;
  }
  return b;
}

// don't know why, but not condidered during ADL
// DataIntArray operator+(DataIntArray lhs, DataIntArray const &rhs) {
//   std::transform(cbegin(lhs), cend(lhs), cbegin(rhs), begin(lhs),
//                  std::plus<>{});
//   return lhs;
// }

auto resolve(std::vector<BinaryData> data) {
  auto const newData = [&]() {
    std::vector<DataIntArray> newData;
    newData.reserve(data.size());
    std::transform(cbegin(data), cend(data), back_inserter(newData), map);
    return newData;
  }();

  auto const acc =
      std::accumulate(cbegin(newData), cend(newData), DataIntArray{},
                      [](DataIntArray lhs, DataIntArray const &rhs) {
                        std::transform(cbegin(lhs), cend(lhs), cbegin(rhs),
                                       begin(lhs), std::plus<>{});
                        return lhs;
                      });

  //   std::cout << "resulting array: ";
  //   for (int i = N - 1; i >= 0; --i) {
  //     std::cout << acc[i] << ", ";
  //   }
  //   std::cout << '\n';

  return resolve(acc);
}

auto filter(std::vector<BinaryData> data, bool invert) {
  for (int i = N - 1; i >= 0; --i) {
    auto const resolved = invert ? ~resolve(data) : resolve(data);
    std::erase_if(data, [r = resolved[i], i](auto d) { return d[i] != r; });
    if (data.size() == 1)  // not catching size==0
      break;
  }
  return data[0];
}

int main() {
  auto const data = readinputdata<BinaryData>();
  //   auto const data = std::vector<BinaryData>{0b00100, 0b11110, 0b10110,
  //   0b10111,
  //                                             0b10101, 0b01111, 0b00111,
  //                                             0b11100, 0b10000, 0b11001,
  //                                             0b00010, 0b01010};
  auto const resolved = resolve(data);
  //   std::cout << "resulting bitset: " << resolved << "\n";

  auto const gamme_rate = resolved.to_ulong();
  std::cout << "Gamma rate: " << gamme_rate << "\n";
  auto const epsilon_rate = (~resolved).to_ulong();
  std::cout << "Epsilon rate: " << epsilon_rate << "\n";
  auto const power_consumption = gamme_rate * epsilon_rate;
  std::cout << "Power consumption: " << power_consumption << "\n";

  auto const oxygen_rating = filter(data, false).to_ulong();
  std::cout << "Oxygen ratiny: " << oxygen_rating << "\n";
  auto const CO2_scrubber_rating = filter(data, true).to_ulong();
  std::cout << "CO2 scrubber rating: " << CO2_scrubber_rating << "\n";
  auto const life_support_rating = oxygen_rating * CO2_scrubber_rating;
  std::cout << "Life support rating: " << life_support_rating << "\n";
}
