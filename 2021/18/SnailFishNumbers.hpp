#ifndef SNAILFISHNUMBERS_HPP
#define SNAILFISHNUMBERS_HPP

#include <string>
#include <vector>

#include "Pair.hpp"
#include "RegularNumber.hpp"
#include "global.hpp"

class SnailFishNumbers {
 private:
  std::vector<Pair> pairVec_{};
  std::vector<RegularNumber> regNumVec_{};

 public:
  auto generate_pair(std::string::const_iterator &it,
                     RegNumIndex &leftRegNumIdx,
                     PairIndex parent = PairIndex{invalid_index}) -> PairIndex;

  auto add_pair(PairIndex leftIdx, PairIndex rightIdx);
  void reduce(PairIndex pi);

  void print(RegNumIndex ri);
  void print(PairIndex pi);
  void dumpVecs();

 private:
  auto generate_side(std::string::const_iterator &it,
                     RegNumIndex &leftRegNumIdx, PairIndex pairIdx)
      -> IndexVariant;

  auto explode_pair(PairIndex p);
  auto split_literal(RegNumIndex l, PairIndex parent);

  bool tryExplode(PairIndex pi, int depth = 0);
  bool trySplitSide(IndexVariant iv);
  bool trySplit(PairIndex pi);

 private:
  template <typename NeighborIdx>
  auto get_outer_regnumidx(Pair const &pair, NeighborIdx neightborIdx);

  template <typename NeighborIdx, typename IdxBack>
  auto add_value_to_neighbor(IndexVariant myIdx, NeighborIdx neightborIdx,
                             RegNumIndex newIdx, IdxBack idxBack);
};

auto &operator<<(std::ostream &os, IndexVariant iv);

template <typename Type>
auto get_last_index(std::vector<Type> const &vec) {
  using Index = typename Type::IndexType;
  return Index{static_cast<typename Index::UnderlyingType>(size(vec) - 1)};
}

#endif