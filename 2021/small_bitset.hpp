#ifndef SMALL_BITSET_HPP
#define SMALL_BITSET_HPP
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>

template <int N, typename T> class small_bitset {
private:
  T val_{0};
  static constexpr T mask_{static_cast<T>((1 << N) - 1)};

public:
  [[nodiscard]] constexpr small_bitset() noexcept = default;
  [[nodiscard]] constexpr small_bitset(std::integral auto val) noexcept
      : val_{static_cast<T>(val & mask_)} {};

  constexpr auto &operator&=(small_bitset other) noexcept {
    val_ &= other.val_;
    return *this;
  }
  [[nodiscard]] friend constexpr auto operator&(small_bitset lhs,
                                                small_bitset rhs) noexcept {
    return lhs &= rhs;
  }
  constexpr auto &operator|=(small_bitset other) noexcept {
    val_ |= other.val_;
    return *this;
  }
  [[nodiscard]] friend constexpr auto operator|(small_bitset lhs,
                                                small_bitset rhs) noexcept {
    return lhs |= rhs;
  }
  [[nodiscard]] constexpr auto operator~() const noexcept {
    return small_bitset{mask_ & ~val_};
  }
  constexpr auto &operator>>=(size_t amount) noexcept {
    val_ >>= amount;
    return *this;
  }
  [[nodiscard]] friend constexpr auto operator>>(small_bitset sbs,
                                                 size_t amount) noexcept {
    return sbs >>= amount;
  }
  constexpr auto &operator<<=(size_t amount) noexcept {
    val_ = (val_ << amount) & mask_;
    return *this;
  }
  [[nodiscard]] friend constexpr auto operator<<(small_bitset sbs,
                                                 size_t amount) noexcept {
    return sbs <<= amount;
  }
  [[nodiscard]] constexpr auto
  operator<=>(small_bitset const &other) const noexcept = default;
  [[nodiscard]] constexpr bool
  operator==(small_bitset const &other) const noexcept = default;
  constexpr auto &set() noexcept {
    val_ = ~T{0};
    return *this;
  }
  constexpr auto &set(size_t pos) noexcept {
    // no pos checking!
    val_ |= T{1} << pos;
    return *this;
  }
  constexpr auto &reset() noexcept {
    val_ = T{0};
    return *this;
  }
  constexpr auto &reset(size_t pos) noexcept {
    // no pos checking!
    val_ &= ~(T{1} << pos);
    return *this;
  }
  [[nodiscard]] constexpr auto test(size_t pos) const noexcept {
    // no pos checking!
    return static_cast<bool>((val_ >> pos) & 1);
  }
  [[nodiscard]] constexpr auto count() const noexcept {
    auto acc{0};
    auto cpy{val_};
    for (int i{0}; i < N; ++i) {
      acc += cpy & 1;
      cpy >>= 1;
    }
    return acc;
  }
  // constexpr auto data() const noexcept { return val_; }

  friend auto &operator<<(std::ostream &os, small_bitset const &sb) {
    for (T m{1 << (N - 1)}; m > 0; m >>= 1) {
      os << ((sb.val_ & m) ? '1' : '0');
    }
    return os;
  }
};

template <size_t N> using small_bitset8 = small_bitset<N, uint8_t>;

#endif