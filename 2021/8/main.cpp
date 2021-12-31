#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"
#include "Display.hpp"
#include "DisplayResolver.hpp"
#include "Observation.hpp"
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

using StringObservation = Observation<std::string>;
using BinaryObservation = Observation<Display>;

template <size_t N>
constexpr auto StringObservationsToDisplayObservations(
    std::array<std::string, N> strArr) noexcept {
  auto retVal{std::array<Display, N>{}};
  transform(cbegin(strArr), cend(strArr), begin(retVal), letters_to_Display);
  return retVal;
}

static constexpr auto testData1 =
    // clang-format off
  "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf\n";
// clang-format on

static constexpr auto testData2 =
    // clang-format off
  "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\n"
  "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\n"
  "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg\n"
  "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb\n"
  "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea\n"
  "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\n"
  "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\n"
  "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef\n"
  "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb\n"
  "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce\n";
// clang-format on

int main() {
  auto const observations{readinputdata<StringObservation>(
      /*std::stringstream{testData2}*/ std::ifstream{"input"})};

  // copy(cbegin(observations), cend(observations),
  //      std::ostream_iterator<StringObservation>(std::cout, "\n"));

  auto const amountHasLength1478{transform_reduce(
      cbegin(observations), cend(observations), 0, std::plus<>{},
      [](auto const &obs) {
        return count_if(cbegin(obs.digits), cend(obs.digits),
                        [](std::string const &str) {
                          auto const strLen = size(str);
                          return is_any_value_out_of<size_t>(
                              strLen, {2 /*1*/, 3 /*7*/, 4 /*4*/, 7 /*8*/});
                        });
      })};
  std::cout << "Amount of digits with length 1, 4, 7 or 8: "
            << amountHasLength1478 << std::endl;

  auto const binaryObservations = [&]() {
    auto binaryObservations{std::vector<BinaryObservation>{}};
    binaryObservations.reserve(size(observations));
    transform(cbegin(observations), cend(observations),
              back_inserter(binaryObservations),
              [](StringObservation const &obs) {
                return BinaryObservation{
                    StringObservationsToDisplayObservations(obs.uniquePattern),
                    StringObservationsToDisplayObservations(obs.digits)};
              });
    return binaryObservations;
  }();

  // copy(cbegin(binaryObservations), cend(binaryObservations),
  //      std::ostream_iterator<BinaryObservation>(std::cout, "\n"));

  // auto const displayResolver{DisplayResolver{binaryObservations[0]}};
  // auto const mapping{displayResolver.resolve_mapping()};
  // std::cout << displayResolver.applyTo(mapping) << std::endl;

  auto const displayValues = [&]() {
    auto displayValues{std::vector<int>{}};
    displayValues.reserve(size(binaryObservations));
    std::transform(cbegin(binaryObservations), cend(binaryObservations),
                   back_inserter(displayValues),
                   [](BinaryObservation const &bo) {
                     auto const displayResolver{DisplayResolver{bo}};
                     auto const mapping = displayResolver.resolve_mapping();
                     return displayResolver.applyTo(mapping);
                   });
    return displayValues;
  }();

  auto const result = std::reduce(cbegin(displayValues), cend(displayValues));

  std::cout << result << std::endl;
}