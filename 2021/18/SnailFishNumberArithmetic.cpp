#include <functional>

#include "SnailFishNumbers.hpp"

template <typename NeighborIdx>
auto SnailFishNumbers::get_outer_regnumidx(Pair const &pair,
                                           NeighborIdx neightborIdx) {
  auto idx{std::invoke(neightborIdx, pair)};
  while (holds_alternative<PairIndex>(idx)) {
    idx = std::invoke(neightborIdx, pairVec_[get<PairIndex>(idx)]);
  }
  return get<RegNumIndex>(idx);
}

// To add two snailfish numbers, form a pair from the left and right parameters
// of the addition operator.
auto SnailFishNumbers::add_pair(PairIndex leftIdx, PairIndex rightIdx) {
  pairVec_.emplace_back(PairIndex{invalid_index}, leftIdx, rightIdx);
  auto const pairIdx{get_last_index(pairVec_)};

  // update childs' parent
  auto &left = pairVec_[leftIdx];
  left.parent = pairIdx;
  auto &right = pairVec_[rightIdx];
  right.parent = pairIdx;

  // finally update regular number connections
  auto const leftRegNumIdx = get_outer_regnumidx(left, &Pair::rightIdx);
  auto const rightRegNumIdx = get_outer_regnumidx(right, &Pair::leftIdx);
  // std::cout << leftRegNumIdx.get() << " " << rightRegNumIdx.get() << '\n';
  regNumVec_[rightRegNumIdx].leftIdx = leftRegNumIdx;
  regNumVec_[leftRegNumIdx].rightIdx = rightRegNumIdx;

  return pairIdx;
}

template <typename NeighborIdx, typename IdxBack>
auto SnailFishNumbers::add_value_to_neighbor(IndexVariant myIdx,
                                             NeighborIdx neightborIdx,
                                             RegNumIndex newIdx,
                                             IdxBack idxBack) {
  auto const &current{regNumVec_[get<RegNumIndex>(myIdx)]};
  auto const neighborIdx = std::invoke(neightborIdx, current);
  if (neighborIdx != invalid_index) {
    auto &neighbor = regNumVec_[neighborIdx];
    neighbor.value += current.value;
    std::invoke(idxBack, neighbor) = newIdx;
  }
  return neighborIdx;
}

auto SnailFishNumbers::explode_pair(PairIndex p) {
  // the entire exploding pair is replaced with the regular number 0.
  // note: could probably reuse one of the old regular numbers. But this is ok
  regNumVec_.emplace_back(0);
  auto const newIdx = get_last_index(regNumVec_);

  auto const &pair{pairVec_[p]};
  // the pair's left value is added to the first regular number to the left of
  // the exploding pair (if any).
  auto const leftIdx = add_value_to_neighbor(
      pair.leftIdx, &RegularNumber::leftIdx, newIdx, &RegularNumber::rightIdx);

  // the pair's right value is added to the first regular number to the right of
  // the exploding pair (if any).
  auto const rightIdx = add_value_to_neighbor(
      pair.rightIdx, &RegularNumber::rightIdx, newIdx, &RegularNumber::leftIdx);

  // finally update regular number connections
  auto &newRegNum = regNumVec_[newIdx];
  newRegNum.leftIdx = leftIdx;
  newRegNum.rightIdx = rightIdx;

  return newIdx;
}

auto SnailFishNumbers::split_literal(RegNumIndex l, PairIndex parent) {
  auto const &literal{regNumVec_[l]};

  // the left element of the pair should be the regular number divided by two
  // and rounded **down**
  regNumVec_.emplace_back(literal.value / 2, literal.leftIdx, RegNumIndex{-1});
  auto const leftLiteralIndex{get_last_index(regNumVec_)};

  // the right element of the pair should be the regular number divided by two
  // and rounded **up**
  regNumVec_.emplace_back((literal.value + 1) / 2, leftLiteralIndex,
                          literal.rightIdx);
  auto const rightLiteralIndex{get_last_index(regNumVec_)};

  // (fix reference)
  regNumVec_[leftLiteralIndex].rightIdx = rightLiteralIndex;

  // replace it with a pair
  pairVec_.emplace_back(parent, leftLiteralIndex, rightLiteralIndex);
  return get_last_index(pairVec_);
}

bool SnailFishNumbers::tryExplode(PairIndex pi, int depth /*= 0 */) {
  auto &pair = pairVec_[pi];
  if (depth == 4) {
    if (holds_alternative<PairIndex>(pair.leftIdx)) {
      explode_pair(get<PairIndex>(pair.leftIdx));
      return true;
    }
    if (holds_alternative<PairIndex>(pair.rightIdx)) {
      explode_pair(get<PairIndex>(pair.rightIdx));
      return true;
    }
    return false;
  }
  return (holds_alternative<PairIndex>(pair.leftIdx) &&
          tryExplode(get<PairIndex>(pair.leftIdx), depth + 1)) ||
         (holds_alternative<PairIndex>(pair.rightIdx) &&
          tryExplode(get<PairIndex>(pair.rightIdx), depth + 1));
}

bool SnailFishNumbers::trySplitSide(IndexVariant iv) {
  return (holds_alternative<RegNumIndex>(iv) &&
          regNumVec_[get<RegNumIndex>(iv)].value > 9) ||
         trySplit(get<PairIndex>(iv));
}

bool SnailFishNumbers::trySplit(PairIndex pi) {
  auto &pair = pairVec_[pi];
  return trySplitSide(pair.leftIdx) || trySplitSide(pair.rightIdx);
}

void SnailFishNumbers::reduce(PairIndex pi) {
  while (tryExplode(pi) || trySplit(pi)) {
  }
}