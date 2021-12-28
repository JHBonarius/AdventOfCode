#include "small_bitset.hpp"

// tests
static_assert([]() {
  auto const uut{small_bitset8<8>{}}; // when default constructed
  return uut.count() == 0;            // no bits are set
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{7}}; // 7 = binary 0b111
  return uut.count() == 3;             // so 3 bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{}}; // when default constructed
  uut.set();                    // and setting all the bits
  return uut.count() == 8;      // All bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{}}; // when default constructed
  uut.set(1);                   // and bit one is set
  return uut.count() == 1 &&    // 1 bit is set
         uut.test(1) &&         // and that's bit one
         !uut.test(0);          // and not bit zero
}());

static_assert([]() {
  auto uut{small_bitset8<8>{255}}; // 255 = binary 0b11111111
  uut.reset();                     // and clearing all the bits
  return uut.count() == 0;         // No bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{2}}; // 2 = binary 0b10
  uut.reset(1);                  // and bit one is set
  return uut.count() == 0 &&     // no bits are set
         !uut.test(1);           // espectially not bit one (just checking)
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{}}; // when default constructed
  auto const rev_uut = ~uut;          // invert the bitset
  return rev_uut.count() == 8;        // All bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{6}}; // 6 = binary 0b110
  uut &= 3;                      // and-assign 3 = binary 0b11
  return uut.count() == 1 &&     // One bit is set
         uut.test(1);            // and that's bit one
}());

static_assert([]() {
  auto uut{small_bitset8<8>{2}}; // 2 = binary 0b10
  uut |= 5;                      // or-assign 5 = binary 0b101
  return uut.count() == 3;       // Three bits are set
}());
