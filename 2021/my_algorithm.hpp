#ifndef MY_ALGORITHM_HPP
#define MY_ALGORITHM_HPP
#include <functional>
#include <limits>

template <class ForwardIt, class UnaryTransformOp = std::identity,
          class Compare = std::less<>>
[[nodiscard]] constexpr auto transform_min_element(
    ForwardIt first, ForwardIt last, UnaryTransformOp transformOp = {},
    Compare comp = {}) noexcept {
  if (first == last) {
    return first;  // or throw??
  }
  auto min_value{transformOp(*first)};
  auto min_element{first};
  while (++first != last) {
    auto const value{transformOp(*first)};
    if (comp(value, min_value)) {
      min_value = value;
      min_element = first;
    }
  }
  return min_element;
}

template <class ForwardIt, class UnaryTransformOp = std::identity,
          class Compare = std::less<>>
[[nodiscard]] constexpr auto transform_max_element(
    ForwardIt first, ForwardIt last, UnaryTransformOp transformOp = {},
    Compare comp = {}) noexcept {
  if (first == last) {
    return first;  // or throw??
  }
  auto max_value{transformOp(*first)};
  auto max_element{first};
  while (++first != last) {
    auto const value{transformOp(*first)};
    if (!comp(value, max_value)) {
      max_value = value;
      max_element = first;
    }
  }
  return max_element;
}

template <class T>
[[nodiscard]] constexpr bool is_any_value_out_of(
    T val, std::initializer_list<T> list) noexcept {
  return std::cend(list) != std::find(cbegin(list), cend(list), val);
}

// need ranges::zip
template <class InputIt1, class InputIt2, class BinaryFunction>
constexpr BinaryFunction for_each(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, BinaryFunction f) {
  for (; first1 != last1; ++first1, ++first2) {
    f(*first1, *first2);
  }
  return f;  // implicit move since C++11
}

constexpr auto hex_char_to_int(char c) {
  return c >= 'A' ? c - 'A' + 10 : c - '0';
}

// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif