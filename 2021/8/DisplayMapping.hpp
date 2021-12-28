#ifndef DISPLAYMAPPING_H
#define DISPLAYMAPPING_H
#include "Display.hpp"
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>

class DisplayMapping {
private:
  std::array<Display, nrSegments> mapping{};

public:
  constexpr DisplayMapping() noexcept {
    // initialize all values to set
    for (auto &m : mapping) {
      m.set();
    }
  }

  // exclude could be derived from only, but lets not do that for simplicity
  constexpr void setFrom(Display const val,
                         std::initializer_list<int> const only,
                         std::initializer_list<int> const exclude) noexcept {
    for (int o : only) {
      mapping[o] &= val;
    }
    for (int e : exclude) {
      mapping[e] &= ~val;
    }
  }

  constexpr auto applyTo(Display d) const noexcept { return 0; }

  friend auto &operator<<(std::ostream &os, DisplayMapping const &dm) {
    std::copy(cbegin(dm.mapping), cend(dm.mapping),
              std::ostream_iterator<Display>{os, ", "});
    return os;
  }
};

#endif