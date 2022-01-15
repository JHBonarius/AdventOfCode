#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace ranges = std::ranges;

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

struct InsertionRule {
  std::string pair{};
  char insert{};
};

auto index_of(std::vector<InsertionRule> const &irs, std::string pair) {
  return distance(begin(irs), ranges::find(irs, pair, &InsertionRule::pair));
}

static constexpr auto N{40};

int main() {
  auto const inputStrs{readinputlines( // std::stringstream{testData}
      std::fstream{"input"})};
  auto const polymerTemplate{inputStrs[0]};
  auto const insertionRules{[&] {
    auto insertionRules{std::vector<InsertionRule>{}};
    insertionRules.reserve(size(inputStrs) - 2);
    transform(next(cbegin(inputStrs), 2), cend(inputStrs),
              back_inserter(insertionRules), [](auto const &str) {
                return InsertionRule{str.substr(0, 2), str[6]};
              });
    return insertionRules;
  }()};

  auto const pairsCreatedFromPair{[&] {
    auto pairsCreatedFromPair{std::vector<std::pair<int, int>>{}};
    pairsCreatedFromPair.reserve(size(insertionRules));
    transform(cbegin(insertionRules), cend(insertionRules),
              back_inserter(pairsCreatedFromPair), [&](auto const &ir) {
                return std::pair{
                    index_of(insertionRules, {ir.pair[0], ir.insert}),
                    index_of(insertionRules, {ir.insert, ir.pair[1]})};
              });
    return pairsCreatedFromPair;
  }()};

  auto pairsCount{std::vector<uint64_t>(size(insertionRules), 0)};

  for (auto i{0}; i < static_cast<int>(size(polymerTemplate)) - 1; ++i) {
    ++pairsCount[index_of(insertionRules, polymerTemplate.substr(i, 2))];
  }

  for (auto n{0}; n < N; ++n) {
    auto newPairsCount{std::vector<uint64_t>(size(insertionRules), 0)};
    for_each(cbegin(pairsCount), cend(pairsCount), cbegin(pairsCreatedFromPair),
             [&](auto count, auto const &mapping) {
               newPairsCount[mapping.first] += count;
               newPairsCount[mapping.second] += count;
             });
    pairsCount = newPairsCount;
  }

  auto const letterCount{[&] {
    auto letterCount{std::map<char, uint64_t>{}};
    for_each(
        cbegin(insertionRules), cend(insertionRules), cbegin(pairsCount),
        [&](auto const &ir, auto count) { letterCount[ir.pair[0]] += count; });
    ++letterCount[polymerTemplate.back()]; // missing last letter, which is
                                           // constant
    return letterCount;
  }()};

  for (auto &&lc : letterCount) {
    std::cout << lc.first << ": " << lc.second << '\n';
  }

  auto const [minEl, maxEl] = minmax_element(
      cbegin(letterCount), cend(letterCount),
      [](auto const &lhs, auto const &rhs) { return lhs.second < rhs.second; });
  std::cout << "Max - min: " << (maxEl->second - minEl->second) << '\n';
}