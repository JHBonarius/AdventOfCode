#ifndef PAIR_HPP
#define PAIR_HPP

#include <iostream>
#include <variant>

#include "../named.hpp"
#include "RegularNumber.hpp"

struct index_for_vector_of_Pair;
using PairIndex = Named<int, index_for_vector_of_Pair>;

using IndexVariant = std::variant<PairIndex, RegNumIndex>;

struct Pair {
  using IndexType = PairIndex;

  PairIndex parent{};
  IndexVariant leftIdx{}, rightIdx{};
};

#endif