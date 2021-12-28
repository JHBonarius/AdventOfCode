#ifndef SMALL_BITSET_H
#define SMALL_BITSET_H
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>

template <int N, typename T> class small_bitset {
private:
  T val_{0};

public:
  constexpr small_bitset() noexcept = default;
  constexpr small_bitset(std::integral auto val) noexcept
      : val_{static_cast<T>(val)} {};

  constexpr auto &operator&=(small_bitset other) noexcept {
    val_ &= other.val_;
    return *this;
  }
  constexpr auto &operator|=(small_bitset other) noexcept {
    val_ |= other.val_;
    return *this;
  }
  constexpr auto operator~() const noexcept { return small_bitset{~val_}; }
  constexpr auto
  operator<=>(small_bitset const &other) const noexcept = default;
  constexpr bool operator==(small_bitset const &other) const noexcept = default;
  constexpr auto &set() noexcept {
    val_ = ~T{0};
    return *this;
  }
  constexpr auto &set(size_t pos) noexcept {
    val_ |= T{1} << pos;
    return *this;
  }
  constexpr auto &reset() noexcept {
    val_ = T{0};
    return *this;
  }
  constexpr auto &reset(size_t pos) noexcept {
    val_ &= ~(T{1} << pos);
    return *this;
  }
  constexpr auto test(size_t pos) const noexcept {
    return static_cast<bool>((val_ >> pos) & 1);
  }
  constexpr auto count() const noexcept {
    auto acc{0};
    auto cpy{val_};
    for (int i{0}; i < N; ++i) {
      acc += cpy & 1;
      cpy >>= 1;
    }
    return acc;
  }

  friend auto &operator<<(std::ostream &os, small_bitset const &sb) {
    for (T m{1 << (N - 1)}; m > 0; m >>= 1) {
      os << ((sb.val_ & m) ? '1' : '0');
    }
    return os;
  }
};

template <size_t N> using small_bitset8 = small_bitset<N, uint8_t>;

#endif