#ifndef DISPLAYRESOLVER
#define DISPLAYRESOLVER
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
      letters_to_Display("cd"),      // 1
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
          "Not recognized: Display has likelynot been proprly resolved.");
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
  constexpr auto findAllWithLength5() const noexcept {
    auto retVal{std::array<Display, 3>{}}; // 0,6,9 ALWAYS! or bad data
    std::copy_if(cbegin(observation.uniquePattern),
                 cend(observation.uniquePattern), begin(retVal),
                 [=](Display d) { return d.count() == 5; });
    return retVal;
  }

public:
  constexpr DisplayResolver(Observation<Display> obs) noexcept
      : observation{obs} {}

  constexpr auto applyTo(DisplayMapping const &mapping) const /*noexcept*/ {
    return std::transform_reduce(
        cbegin(observation.digits), cend(observation.digits), 0,
        [](int acc, int val) { return acc * 10 + val; },
        [&](Display d) { return decode(mapping.applyTo(d)); });
  }

  /*constexpr*/ auto resolve_mapping() const {
    auto mapping{DisplayMapping{}};
    {
      // 1 least segments: 2
      auto const one{findOneWithLength(2)};
      mapping.setFrom(one, {2, 5}, {0, 1, 3, 4, 6});
    }
    std::cout << mapping << '\n';
    {
      // 7 second least: 3
      auto const seven{findOneWithLength(3)};
      mapping.setFrom(seven, {0 /*, 2, 5*/}, {1, 3, 4, 6});
    }
    std::cout << mapping << '\n';
    {
      // 4 third least: 4
      auto const four{findOneWithLength(4)};
      mapping.setFrom(four, {1, /*2,*/ 3 /*, 5*/}, {/*0,*/ 4, 6});
    }
    std::cout << mapping << '\n';
    auto const allWithLength5{findAllWithLength5()}; // 0,6,9
    // // 9
    // { auto const one = find(uniquePattern, 5); }
    // // 6
    // { auto const one = find(uniquePattern, 6); }
    // // 0
    // { auto const one = find(uniquePattern, 7); }
    return mapping;
  }
};

#endif