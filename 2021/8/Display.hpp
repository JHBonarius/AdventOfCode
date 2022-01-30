#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <string_view>

#include "../small_bitset.hpp"

static constexpr auto nrSegments{7};
using Display = small_bitset8<nrSegments>;

[[nodiscard]] constexpr auto letters_to_Display(std::string_view const str) {
  auto retVal{Display{}};
  for (char c : str) {
    retVal.set(c - 'a');
  }
  return retVal;
}

#endif