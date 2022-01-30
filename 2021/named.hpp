#ifndef NAMED_HPP
#define NAMED_HPP
#include <concepts>
#include <type_traits>
#include <utility>

template <typename Type, typename Description>
class Named {
 public:
  using UnderlyingType = Type;

  constexpr Named() noexcept(
      std::is_nothrow_default_constructible_v<Type>) requires
      std::default_initializable<Type>
  = default;
  explicit constexpr Named(Type const &val) noexcept(
      std::is_nothrow_copy_constructible_v<Type>)
      : val_{val} {}
  explicit constexpr Named(Type &&val) noexcept(
      std::is_nothrow_move_constructible_v<Type>)
      : val_{std::move(val)} {}

  constexpr operator Type() const noexcept(
      std::is_nothrow_copy_constructible_v<Type>) requires std::copyable<Type> {
    return val_;
  }

  // constexpr auto const &description() const { return Name; }

  constexpr auto &get() & { return val_; }
  constexpr auto const &get() const & { return val_; }
  constexpr auto &&get() && requires std::movable<Type> {
    return std::move(val_);
  }
  constexpr auto const &&get() const &&requires std::movable<Type> {
    return std::move(val_);
  }

  constexpr void set(Type const &val) noexcept(
      std::is_nothrow_copy_assignable_v<Type>) {
    val_ = val;
  }
  constexpr void set(Type &&val) noexcept(
      std::is_nothrow_move_assignable_v<Type>) {
    val_ = std::move(val);
  }

 private:
  Type val_;
};

#endif