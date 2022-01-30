#include "Display.hpp"

static_assert([]() {
  auto const uut{Display{}};  // default constructed display
  return uut.count() == 0;    // has no segments set
}());

static_assert([]() {
  auto const uut{~Display{}};        // inverted default constructed display
  return uut.count() == nrSegments;  // has all segments set
}());

// mapping of letters to bits
//  a   0
// b c 1 2
//  d   3
// e f 4 5
//  g   6

static_assert([]() {
  auto const uut{letters_to_Display("")};  // when no letters are set
  return uut.count() == 0;                 // has no segments set
}());

static_assert([]() {
  auto const uut{letters_to_Display("abcdefg")};  // when all letters are set
  return uut.count() == nrSegments;               // all segments set
}());

static_assert([]() {
  auto const uut{letters_to_Display("bdf")};  // when three letters are set
  return uut.count() == 3 &&                  // three segments set
         !uut.test(0) &&                      // 0 is not set
         uut.test(1);                         // but 1 is
}());
