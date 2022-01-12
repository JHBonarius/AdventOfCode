#include "../readinputdata.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto testData{"NNCB\n"
                               "\n"
                               "CH -> B\n"
                               "HH -> N\n"
                               "CB -> H\n"
                               "NH -> C\n"
                               "HB -> C\n"
                               "HC -> B\n"
                               "HN -> C\n"
                               "NN -> C\n"
                               "BH -> H\n"
                               "NC -> B\n"
                               "NB -> B\n"
                               "BN -> B\n"
                               "BB -> N\n"
                               "BC -> B\n"
                               "CC -> N\n"
                               "CN -> C\n"};

using InsertionRules_t = std::map<char, std::map<char, char>>;

static auto print(std::list<char> const &l) {
  for (auto c : l) {
    std::cout << c;
  }
  std::cout << '\n';
}

static constexpr auto N{10};

int main() {
  auto const inputStrs{readinputlines(
      /*std::stringstream{testData}*/
      std::fstream{"input"})};
  auto const polymetTemplate{inputStrs[0]};
  auto const insertionRules{[&] {
    auto insertionRules{InsertionRules_t{}};
    for_each(next(cbegin(inputStrs), 2), cend(inputStrs),
             [&](std::string const &str) {
               insertionRules[str[0]][str[1]] = str[6];
             });
    return insertionRules;
  }()};

  auto polymer{std::list<char>(cbegin(polymetTemplate), cend(polymetTemplate))};
  for (auto n{0}; n < N; ++n) {
    for (auto it{next(cbegin(polymer), 1)}; it != cend(polymer); ++it) {
      polymer.insert(it, insertionRules.at(*next(it, -1)).at(*it));
    }
    // print(polymer);
  }
  auto const letterCount{[&] {
    auto letterCount{std::map<char, uint64_t>{}};
    for (auto c : polymer) {
      ++letterCount[c];
    }
    return letterCount;
  }()};
  auto const [minEl, maxEl] = minmax_element(
      cbegin(letterCount), cend(letterCount),
      [](auto const &lhs, auto const &rhs) { return lhs.second < rhs.second; });
  std::cout << "Max - min: " << (maxEl->second - minEl->second) << '\n';
}