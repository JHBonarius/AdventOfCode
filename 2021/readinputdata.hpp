#ifndef READINPUTDATA_H
#define READINPUTDATA_H
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

#endif