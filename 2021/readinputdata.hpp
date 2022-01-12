#ifndef READINPUTDATA_HPP
#define READINPUTDATA_HPP
#include <algorithm>
#include <istream>
#include <iterator>
#include <vector>

template <typename T> auto readinputdata(std::istream &&is) {
  auto data{std::vector<T>{}};
  copy(std::istream_iterator<T>{is}, std::istream_iterator<T>{},
       back_inserter(data));
  return data;
}

auto readinputlines(std::istream &&is) {
  auto data{std::vector<std::string>{}};
  for (auto str{std::string{}}; getline(is, str);) {
    data.push_back(str);
  }
  return data;
}

#endif