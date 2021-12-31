#ifndef DISPLAYRESOLVER_HPP
#define DISPLAYRESOLVER_HPP
#include "../my_algorithm.hpp"
#include "Display.hpp"
#include "DisplayMapping.hpp"
#include "Observation.hpp"
#include <algorithm>
#include <array>
#include <exception>
#include <numeric>

class DisplayResolver {
private:
  Observation<Display> const observation;

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
  static constexpr auto decoding{std::array{
      letters_to_Display("abcefg"),  // 0
      letters_to_Display("cf"),      // 1
      letters_to_Display("acdeg"),   // 2
      letters_to_Display("acdfg"),   // 3
      letters_to_Display("bcdf"),    // 4
      letters_to_Display("abdfg"),   // 5
      letters_to_Display("abdefg"),  // 6
      letters_to_Display("acf"),     // 7
      letters_to_Display("abcdefg"), // 8
      letters_to_Display("abcdfg"),  // 9
  }};

  static constexpr auto decode(Display d) {
    auto const it = std::find(cbegin(decoding), cend(decoding), d);
    if (it == cend(decoding)) {
      throw std::logic_error(
          "Not recognized: Display has likelynot been properly resolved.");
    }
    return std::distance(cbegin(decoding), it);
  }

  constexpr auto findOneWithLength(int length) const noexcept {
    return *std::find_if(cbegin(observation.uniquePattern),
                         cend(observation.uniquePattern),
                         [=](Display d) { return d.count() == length; });
  }

  // static auto
  // findAllWithLength(UniquePatternArray<Display> const &uniquePattern,
  //                   int length) noexcept {
  //   auto retVal{std::vector<Display>{}};
  //   retVal.reserve(nrUniquePattern);
  //   copy_if(cbegin(uniquePattern), cend(uniquePattern),
  //   back_inserter(retVal),
  //           [=](Display d) { return d.count() == length; });
  //   return retVal;
  // }

  template <size_t LEN, size_t RES_LEN>
  constexpr auto findAllWithLength() const noexcept {
    auto retVal{std::array<Display, RES_LEN>{}};
    std::copy_if(cbegin(observation.uniquePattern),
                 cend(observation.uniquePattern), begin(retVal),
                 [=](Display d) { return d.count() == LEN; });
    return retVal;
  }

public:
  constexpr DisplayResolver(Observation<Display> obs) noexcept
      : observation{obs} {}

  constexpr auto applyTo(DisplayMapping const &mapping) const /*noexcept*/ {
    auto const map{mapping.getResult()};
    auto const applyMapping{[&](Display source) {
      // std::cout << source << " -> ";
      auto sink{Display{}};
      for (auto i{0}; i < nrSegments; ++i) {
        if (source.test(i)) {
          sink.set(map[i]);
        }
      }
      // std::cout << sink << '\n';
      return sink;
    }};
    return std::accumulate(
        cbegin(observation.digits), cend(observation.digits), 0,
        [&](int acc, Display d) { return acc * 10 + decode(applyMapping(d)); });
  }

  constexpr auto resolve_mapping() const noexcept {
    auto mapping{DisplayMapping{}};
    auto const resolver1{[&](int length, std::initializer_list<int> only,
                             std::initializer_list<int> exclude) {
      mapping.setFrom(findOneWithLength(length), only, exclude);
    }};

    // "1": least segments: 2
    // determine 2 and 5
    resolver1(2, {2, 5}, {0, 1, 3, 4, 6});
    // std::cout << mapping << '\n';

    // "7": second least: 3
    // determine/finish 0
    resolver1(3, {0}, {1, 3, 4, 6}); // 2 and 5 already fixed
    // std::cout << mapping << '\n';

    // "4": third least: 4
    // determine 1 and 3
    resolver1(4, {1, 3}, {4, 6}); // ^ and 0 already at final value
    // std::cout << mapping << '\n';

    auto const allWithLength6{findAllWithLength<6, 3>()}; // 0,6,9
    auto const resolver2([&](std::initializer_list<int> maskSegments,
                             std::initializer_list<int> only,
                             std::initializer_list<int> exclude) {
      auto const mask{~mapping.combine(maskSegments)};
      auto const value = *transform_min_element(
          cbegin(allWithLength6), cend(allWithLength6),
          [&](Display element) { return (element & mask).count(); });
      mapping.setFrom(value, only, exclude);
    });

    // "9": determine/finish 4 and 6
    resolver2({0, 1, 2}, {6}, {4}); // 5 is duplicate of 2, 3 of 1
    // std::cout << mapping << '\n';

    // home stretch
    // "6": finish 2 and 5
    resolver2({0, 1, 3, 4, 6}, {5}, {2});
    // std::cout << mapping << '\n';

    // "0": finish 1 and 3
    resolver2({0, 2, 4, 5, 6}, {1}, {3});
    // std::cout << mapping << '\n';

    return mapping;
  }
};

#endif