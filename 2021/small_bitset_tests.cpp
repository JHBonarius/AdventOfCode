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
  auto const uut{
      small_bitset8<8>{}
          .set()};         // when default constructed and setting all the bits
  return uut.count() == 8; // All bits are set
}());

static_assert([]() {
  auto const uut{
      small_bitset8<8>{}.set(1)}; // when default constructedand bit one is set
  return uut.count() == 1 &&      // 1 bit is set
         uut.test(1) &&           // and that's bit one
         !uut.test(0);            // and not bit zero
}());

static_assert([]() {
  auto const uut{
      small_bitset8<8>{255}
          .reset()};       // 255 = binary 0b11111111 and clearing all the bits
  return uut.count() == 0; // No bits are set
}());

static_assert([]() {
  auto const uut{
      small_bitset8<8>{2}.reset(1)}; // 2 = binary 0b10 and that bit is reset
  return uut.count() == 0 &&         // no bits are set
         !uut.test(1);               // espectially not bit one (just checking)
}());

static_assert([]() {
  auto const uut{
      ~small_bitset8<8>{}}; // when default constructed and invert the bitset
  return uut.count() == 8;  // All bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{6}}; // 6 = binary 0b110
  uut &= 3; // and-assign 3 = binary 0b11 (implicit construction)
  // result = 0b010
  return uut.count() == 1 && // One bit is set
         uut.test(1);        // and that's bit one
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{6} // 6 = binary 0b110
                 & 3}; // and 3 = binary 0b11 (implicit construction)
  // result = 0b010
  return uut.count() == 1 && // One bit is set
         uut.test(1);        // and that's bit one
}());

static_assert([]() {
  auto uut{small_bitset8<8>{2}}; // 2 = binary 0b10
  uut |= 5; // or-assign 5 = binary 0b101 (implicit construction)
  // result = 0b111
  return uut.count() == 3; // Three bits are set
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{2} // 2 = binary 0b10
                 | 5}; // or 5 = binary 0b101 (implicit construction)
  // result =   0b111
  return uut.count() == 3; // Three bits are set
}());

static_assert([]() {
  auto uut{small_bitset8<8>{4}}; // 4 = binary 0b100
  uut >>= 1;                     // after shifting right 0b10
  return uut.count() == 1 &&     // One bit is set
         uut.test(1);            // and that's bit one
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{4} // 4 = binary 0b100
                 >> 1};              // after shifting right 0b10
  return uut.count() == 1 &&         // One bit is set
         uut.test(1);                // and that's bit one
}());

static_assert([]() {
  auto uut{small_bitset8<8>{2}}; // 2 = binary 0b10
  uut <<= 1;                     // after shifting left 0100
  return uut.count() == 1 &&     // One bit is set
         uut.test(2);            // and that's bit two
}());

static_assert([]() {
  auto const uut{small_bitset8<8>{2} // 2 = binary 0b10
                 << 1};              // after shifting left 0100
  return uut.count() == 1 &&         // One bit is set
         uut.test(2);                // and that's bit two
}());

// start mask test
static_assert([]() {
  auto const uut{(small_bitset8<1>{1} >> 1) << 1}; // (1>>1)<<1 = 0<<1 = 0
  return uut.count() == 0;                         // no bits set
}());

static_assert([]() {
  auto const uut{(small_bitset8<1>{1} << 1) >> 1}; // (1<<1)>>1 = 0>>1 = 0
  return uut.count() == 0;                         // no bits set
}());

static_assert([]() {
  auto const uut{(~small_bitset8<1>{}) >> 1}; // (~0)>>1 = 1>>1 = 0
  return uut.count() == 0;                    // no bits set
}());
// end mask test
