#ifndef NAMED_HPP
#define NAMED_HPP
#include <concepts>
#include <type_traits>
#include <utility>

template <typename Type, typename Description>
class Named {
 public:
  using UnderlyingType = Type;

  // clang-format off
  constexpr Named()
    noexcept(std::is_nothrow_default_constructible_v<Type>)
    requires std::default_initializable<Type>
  = default;
  // template <typename... Args>
  // constexpr Named(Args... args)
  //   noexcept(std::is_nothrow_constructible_v<Type,Args...>) 
  //   requires std::constructible_from<Type,Args...>
  //   : val_{args...}
  // {}
  explicit constexpr Named(Type const &val)
    noexcept(std::is_nothrow_copy_constructible_v<Type>) 
    requires std::copyable<Type>
    : val_{val}
  {}
  explicit constexpr Named(Type&& val)
    noexcept(std::is_nothrow_move_constructible_v<Type>)
    requires std::movable<Type>
    : val_{std::move(val)}
  {}

  constexpr operator Type() const&
    noexcept(std::is_nothrow_copy_constructible_v<Type>)
    requires std::copyable<Type>
  {
    return val_;
  }
  constexpr operator Type() const&&
    noexcept(std::is_nothrow_move_constructible_v<Type>) // move assignable?
    requires std::movable<Type>
  {
    return std::move(val_);
  }

  constexpr auto& get() &
  {
    return val_;
  }
  constexpr auto const& get() const&
  {
    return val_;
  }
  constexpr auto&& get() &&
    noexcept(std::is_nothrow_move_constructible_v<Type>)
    requires std::movable<Type>
  {
    return std::move(val_);
  }
  constexpr auto const&& get() const&&
    noexcept(std::is_nothrow_move_constructible_v<Type>)
    requires std::movable<Type>
  {
    return std::move(val_);
  }

  constexpr void set(Type const& val)
    noexcept(std::is_nothrow_copy_assignable_v<Type>)
    requires std::copyable<Type>
  {
    val_ = val;
  }
  constexpr void set(Type&& val)
    noexcept(std::is_nothrow_move_assignable_v<Type>)
    requires std::movable<Type>
  {
    val_ = std::move(val);
  }

 private:
  Type val_;
  // clang-format on
};

#endif