#ifndef NAMED_HPP
#define NAMED_HPP
#include <concepts>
#include <utility>

// requires a static constexpr const char[] literal
template <typename Type, char const *Name> class Named {
public:
  using UnderlyingType = Type;

  constexpr Named() requires std::default_initializable<Type>
  = default;
  explicit constexpr Named(Type const &val) : val_{val} {}
  explicit constexpr Named(Type &&val) : val_{std::move(val)} {}
  explicit constexpr operator Type() const { return val_; }

  constexpr auto const &description() const { return Name; }

private:
  Type val_;
};

#endif