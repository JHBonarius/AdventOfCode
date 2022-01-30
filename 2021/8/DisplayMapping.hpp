#ifndef DISPLAYMAPPING_HPP
#define DISPLAYMAPPING_HPP
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>

#include "Display.hpp"

class DisplayMapping {
 private:
  std::array<Display, nrSegments> mapping{};

 public:
  [[nodiscard]] constexpr DisplayMapping() noexcept {
    // initialize all values to set
    for (auto &m : mapping) {
      m.set();
    }
  }

  constexpr void setFrom(Display const val,
                         std::initializer_list<int> const only,
                         std::initializer_list<int> const exclude) noexcept {
    for (auto o : only) {
      mapping[o] &= val;
    }
    for (auto e : exclude) {
      mapping[e] &= ~val;
    }
  }

  [[nodiscard]] constexpr auto combine(
      std::initializer_list<int> sources) const noexcept {
    return std::transform_reduce(cbegin(sources), cend(sources), Display{},
                                 std::bit_or<>{},
                                 [&](auto i) { return mapping[i]; });
  }

  // assuming every mapping element has only one bit set.
  [[nodiscard]] constexpr auto getResult() const noexcept {
    auto retVal{std::array<int, nrSegments>{}};
    for (auto i{0}; i < nrSegments; ++i) {
      auto bitNr{0};
      while (!mapping[i].test(bitNr)) {
        ++bitNr;
      }
      retVal[bitNr] = i;
    }
    // std::copy(cbegin(retVal), cend(retVal),
    //           std::ostream_iterator<int>{std::cout, ", "});
    // std::cout << '\n';
    return retVal;
  }

  friend auto &operator<<(std::ostream &os, DisplayMapping const &dm) {
    std::copy(cbegin(dm.mapping), cend(dm.mapping),
              std::ostream_iterator<Display>{os, ", "});
    return os;
  }
};

#endif