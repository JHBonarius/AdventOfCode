#ifndef OBSERVATION_H
#define OBSERVATION_H
#include <array>
#include <iostream>
#include <iterator>

static constexpr auto nrUniquePattern{10};
static constexpr auto nrDigits{4};

template <typename T> using UniquePatternArray = std::array<T, nrUniquePattern>;
template <typename T> using DigitArray = std::array<T, nrDigits>;

// using Observation = std::array<std::string, nrUniquePattern + nrDigits>;
template <typename T> struct Observation {
  UniquePatternArray<T> uniquePattern;
  DigitArray<T> digits;
};

template <typename T>
std::istream &operator>>(std::istream &is, Observation<T> &obs) {
  auto isIt{std::istream_iterator<T>{is}};
  copy_n(isIt, nrUniquePattern, begin(obs.uniquePattern));
  advance(isIt, 2); // get the '|' no idea why it should be 2 and not 1...
  copy_n(isIt, nrDigits, begin(obs.digits));
  return is;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, Observation<T> const &obs) {
  auto osIt{std::ostream_iterator<T>(os, " ")};
  copy_n(cbegin(obs.uniquePattern), nrUniquePattern, osIt);
  os << "| ";
  copy_n(cbegin(obs.digits), nrDigits, osIt);
  return os;
}

#endif