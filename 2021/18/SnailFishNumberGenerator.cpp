#include "SnailFishNumbers.hpp"

auto SnailFishNumbers::generate_side(std::string::const_iterator &it,
                                     RegNumIndex &leftRegNumIdx,
                                     PairIndex pairIdx) -> IndexVariant {
  if (*it == '[') {
    return generate_pair(it, leftRegNumIdx, pairIdx);
  }
  // else {
  regNumVec_.emplace_back(*it++ - '0', leftRegNumIdx,
                          RegNumIndex{invalid_index});
  auto const regNumIdx{get_last_index(regNumVec_)};
  if (leftRegNumIdx != invalid_index) {
    regNumVec_[leftRegNumIdx].rightIdx = regNumIdx;
  }
  leftRegNumIdx = regNumIdx;
  return regNumIdx;
  //};
}

auto SnailFishNumbers::generate_pair(std::string::const_iterator &it,
                                     RegNumIndex &leftRegNumIdx,
                                     PairIndex parent) -> PairIndex {
  pairVec_.emplace_back();
  auto const pairIdx{get_last_index(pairVec_)};
  pairVec_[pairIdx].parent = parent;

  // NEED to use the index instead of reference, since the calls to
  // "generate_side" will reallocate the pairVec
  ++it;  // skip [
  pairVec_[pairIdx].leftIdx = generate_side(it, leftRegNumIdx, pairIdx);
  // std::cout << "Check pair leftidx: " << pair.leftIdx << '\n';
  ++it;  // skip comma
  pairVec_[pairIdx].rightIdx = generate_side(it, leftRegNumIdx, pairIdx);
  // std::cout << "Check pair rightidx: " << pair.rightIdx << '\n';
  ++it;  // skip ]

  // std::cout << "return pair index: " << pairIdx.get() << '\n';
  return pairIdx;
}