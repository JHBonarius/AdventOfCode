#ifndef DISPLAYRESOLVER
#define DISPLAYRESOLVER
#include "Display.hpp"
#include "DisplayMapping.hpp"
#include "Observation.hpp"
#include <algorithm>
#include <numeric>

class DisplayResolver {
private:
  constexpr auto findLength(UniquePatternArray<Display> const &uniquePattern,
                            int length) const noexcept {
    return *std::find_if(cbegin(uniquePattern), cend(uniquePattern),
                         [=](Display d) { return d.count() == length; });
  }

public:
  constexpr auto applyTo(DigitArray<Display> const &digits) const noexcept {
    return std::transform_reduce(
        cbegin(digits), cend(digits), 0,
        [](int acc, int val) { return acc * 10 + val; },
        [&](Display d) { return decoder[mapping(d)]; });
  }

  auto resolve_mapping(UniquePatternArray<Display> const &uniquePattern) {
    auto mapping{DisplayMapping{}};
    // 1
    {
      auto const one = findLength(uniquePattern, 2);
      mapping.setFrom(one, {2, 5}, {0, 1, 3, 4, 6});
    }
    // 7
    {
      auto const seven = findLength(uniquePattern, 3);
      mapping.setFrom(seven, {0, 2, 5}, {1, 3, 4, 6});
    }
    // 4
    {
      auto const four = findLength(uniquePattern, 4);
      mapping.setFrom(four, {1, 2, 3, 5}, {0, 4, 6});
    }
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