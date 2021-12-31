#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include "../small_bitset.hpp"
#include <string_view>

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