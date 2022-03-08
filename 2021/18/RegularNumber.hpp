#ifndef REGULARNUMBER_HPP
#define REGULARNUMBER_HPP

#include "../named.hpp"

struct index_for_vector_of_RegularNumber;
using RegNumIndex = Named<int, index_for_vector_of_RegularNumber>;

struct RegularNumber {
  using IndexType = RegNumIndex;

  int value{};
  RegNumIndex leftIdx{}, rightIdx{};
};

#endif