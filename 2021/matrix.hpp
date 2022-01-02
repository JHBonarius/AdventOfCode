#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

struct point {
  size_t x{}, y{};
};

template <typename T> class matrix {
public:
  size_t width{}, height{};
  std::vector<T> data{};

private:
  [[nodiscard]] constexpr auto locToIndex(size_t x, size_t y) const noexcept {
    return y * width + x;
  }
  //[[nodiscard]] constexpr auto locToIndex(point p) const noexcept  { return
  // locToIndex(p.x, p.y); }
  auto &elementAt(size_t x, size_t y) noexcept {
    return data[locToIndex(x, y)];
  }
  auto const &elementAt(size_t x, size_t y) const noexcept {
    return data[locToIndex(x, y)];
  }

public:
  matrix(size_t width, size_t height) noexcept
      : width{width}, height{height}, data{std::vector<T>(width * height)} {}
  matrix(std::vector<std::vector<T>> const &vecvec)
      : width{size(vecvec.at(0))}, height{size(vecvec)}, data{std::vector<T>(
                                                             width * height)} {
    auto dataIt{begin(data)};
    for (auto const &vec : vecvec) {
      assert(size(vec) == width);
      dataIt = copy(cbegin(vec), cend(vec), dataIt);
    }
  }

  auto &operator()(size_t x, size_t y) noexcept { return elementAt(x, y); }
  auto &operator()(point p) noexcept { return elementAt(p.x, p.y); }
  auto const &operator()(size_t x, size_t y) const noexcept {
    return elementAt(x, y);
  }
  auto const &operator()(point p) const noexcept { return elementAt(p.x, p.y); }

  friend auto &operator<<(std::ostream &os, matrix const &mat) {
    auto it{cbegin(mat.data)};
    auto x{size_t{0}};
    while (it != cend(mat.data)) {
      os << *it++;
      ++x;
      if (x == mat.width) {
        os << '\n';
        x = 0;
      }
    }
    return os;
  }
};

#endif