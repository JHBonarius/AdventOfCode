#ifndef DISPLAYMAPPING_H
#define DISPLAYMAPPING_H
#include "Display.hpp"
#include <algorithm>
#include <array>
//#include <vector>

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
  template <size_t N1, size_t N2>
  constexpr void setFrom(Display const val, std::array<int, N1> const only,
                         std::array<int, N2> const exclude) noexcept {
    for (int o : only) {
      mapping[o] &= val;
    }
    for (int e : exclude) {
      mapping[e] &= ~val;
    }
  }
};

#endif