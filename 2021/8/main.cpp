#include <algorithm>
#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto nrUniquePattern{10};
static constexpr auto nrDigits{4};

using Observation = std::array<std::string, nrUniquePattern + nrDigits>;

static constexpr auto cdigitbegin(Observation const &obs) {
  return next(cbegin(obs), nrUniquePattern);
}

std::istream &operator>>(std::istream &is, Observation &obs) {
  auto isIt{std::istream_iterator<std::string>{is}};
  auto obsIt{begin(obs)};
  obsIt = copy_n(isIt, nrUniquePattern, obsIt);
  advance(isIt, 2); // get the '|' no idea why it should be 2 and not 1...
  copy_n(isIt, nrDigits, obsIt);
  return is;
}

std::ostream &operator<<(std::ostream &os, Observation const &obs) {
  auto osIt{std::ostream_iterator<std::string>(os, " ")};
  copy_n(cbegin(obs), nrUniquePattern, osIt);
  os << "| ";
  copy_n(cdigitbegin(obs), nrDigits, osIt);
  return os;
}

//   0:      1:      2:      3:      4:
//  aaaa    ....    aaaa    aaaa    ....
// b    c  .    c  .    c  .    c  b    c
// b    c  .    c  .    c  .    c  b    c
//  ....    ....    dddd    dddd    dddd
// e    f  .    f  e    .  .    f  .    f
// e    f  .    f  e    .  .    f  .    f
//  gggg    ....    gggg    gggg    ....

//   5:      6:      7:      8:      9:
//  aaaa    aaaa    aaaa    aaaa    aaaa
// b    .  b    .  .    c  b    c  b    c
// b    .  b    .  .    c  b    c  b    c
//  dddd    dddd    ....    dddd    dddd
// .    f  e    f  .    f  e    f  .    f
// .    f  e    f  .    f  e    f  .    f
//  gggg    gggg    ....    gggg    gggg

// a = 0, etc

// #include <optional>
// std::optional<std::string> findWithLength(Observation const &obs,
//                                           size_t length) {
//   auto const it = find(cbegin(obs), cend(obs), [=](std::string const &str) {
//     return size(str) == length;
//   });
//   if (it != cend(obs))
//     return *it;
//   return {};
// }

// auto resolve_mapping(Observation const &obs) {
//   auto mapping{std::array<int, 7>{}};
//   auto likelyMap{std::array<std::bitset<7>, 7>{}};
//   // fill(begin(likelyMap), end(likelyMap), std::bitset<7>{(2 << 8) - 1});
//   for (auto &bs : likelyMap)
//     bs.set();
//   return mapping;
// }

template <typename T> auto readinputdata() {
  auto data{std::vector<T>{}};
  auto fs{std::ifstream{"input"}};
  auto val{T{}};
  while (fs >> val) {
    data.push_back(val);
  }
  // std::copy(std::istream_iterator<T>{fs}, std::istream_iterator<T>{},
  //           back_inserter(data));
  return data;
}

int main() {
  auto const observations{
      readinputdata<Observation>()
      // std::vector<std::string>{"be cfbegad cbdgef fgaecd cgeb fdcge agebfd "
      //                          "fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
      //                          "edbfga begcd cbg gc gcadebf fbgde acbgfd
      //                          abcde " "gfcbed gfec | fcgedb cgb dgebacf gc",
      //                          "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad
      //                          gfac " "gcb cdgabef | cg cg fdcagb cbg",
      //                          "fbegcd cbd adcefb dageb afcb bc aefdc ecdab "
      //                          "fgdeca fcdbega | efabcd cedba gadfec cb",
      //                          "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb
      //                          " "dgceab fcbdga | gecf egdcabf bgf bfgea",
      //                          "fgeab ca afcebg bdacfeg cfaedg gcfdb baec "
      //                          "bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
      //                          "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc "
      //                          "dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
      //                          "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced
      //                          " "adcbefg gebcd | ed bcgafe cdgba cbgef",
      //                          "egadfb cdbfeg cegd fecab cgb gbdefca cg
      //                          fgcdab " "egfdb bfceg | gbdfcae bgc cg cgb",
      //                          "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge
      //                          " "fdbac fegbdc | fgae cfgab fg bagce"}
  };
  // copy(cbegin(observations), cend(observations),
  //      std::ostream_iterator<Observation>(std::cout, "\n"));
  for (auto const &obs : observations)
    std::cout << obs << '\n';

  auto const amountHasLength1478{transform_reduce(
      cbegin(observations), cend(observations), 0, std::plus<>{},
      [](auto const &obs) {
        return count_if(cdigitbegin(obs), cend(obs),
                        [](std::string const &str) {
                          auto const strLen = size(str);
                          return strLen == 2 /*1*/ || strLen == 3 /*7*/ ||
                                 strLen == 4 /*4*/ || strLen == 7 /*8*/;
                        });
      })};
  std::cout << "Amount of digits with length 1, 4, 7 or 8: "
            << amountHasLength1478 << std::endl;
}