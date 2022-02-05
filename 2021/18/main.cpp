#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

#include "../named.hpp"
#include "../readinputdata.hpp"

static constexpr auto testData{
    "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]\n"
    "[[[5,[2,8]],4],[5,[[9,9],0]]]\n"
    "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]\n"
    "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]\n"
    "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]\n"
    "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]\n"
    "[[[[5,4],[7,7]],8],[[8,3],8]]\n"
    "[[9,3],[[9,9],[6,[4,9]]]]\n"
    "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]\n"
    "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]\n"};

struct index_for_vector_of_Pair;
using PairIndex = Named<int, index_for_vector_of_Pair>;

struct index_for_vector_of_RegularNumber;
using RegNumIndex = Named<int, index_for_vector_of_RegularNumber>;

struct RegularNumber {
  using IndexType = RegNumIndex;

  int value{};
  RegNumIndex leftIdx{}, rightIdx{};
};

using IndexVariant = std::variant<PairIndex, RegNumIndex>;

struct Pair {
  using IndexType = PairIndex;

  PairIndex parent{};
  IndexVariant leftIdx{}, rightIdx{};
};

static auto pairVec{std::vector<Pair>{}};
static auto regNumVec{std::vector<RegularNumber>{}};
static auto leftRegNumIdx{RegNumIndex{-1}};

static constexpr auto invalid_index{-1};

template <typename Type>
auto get_last_index(std::vector<Type> const &vec) {
  using Index = typename Type::IndexType;
  return Index{static_cast<typename Index::UnderlyingType>(size(vec))};
}

auto generate_pair(std::string::const_iterator &it, PairIndex parent)
    -> PairIndex;

auto generate_side(std::string::const_iterator &it, PairIndex pairIdx)
    -> IndexVariant {
  if (*it == '[') {
    return generate_pair(it, pairIdx);
  } else {
    regNumVec.emplace_back(*it - '0', leftRegNumIdx,
                           RegNumIndex{invalid_index});
    auto const litIdx{get_last_index(regNumVec)};
    if (leftRegNumIdx != invalid_index) {
      regNumVec[leftRegNumIdx].rightIdx = litIdx;
    }
    leftRegNumIdx = litIdx;
    return litIdx;
  };
}

auto generate_pair(std::string::const_iterator &it,
                   PairIndex parent = PairIndex{invalid_index}) -> PairIndex {
  pairVec.emplace_back();
  auto const pairIdx{get_last_index(pairVec)};
  auto &pair{pairVec.back()};
  pair.parent = parent;

  ++it;  // skip [
  pair.leftIdx = generate_side(it, pairIdx);
  ++it;  // skip comma
  pair.rightIdx = generate_side(it, pairIdx);
  ++it;  // skip ]

  return pairIdx;
}

// To add two snailfish numbers, form a pair from the left and right parameters
// of the addition operator.
auto add_pair(PairIndex leftIdx, PairIndex rightIdx) {
  pairVec.emplace_back(PairIndex{invalid_index}, leftIdx, rightIdx);
  auto const pairIdx{get_last_index(pairVec)};
  pairVec[leftIdx].parent = pairIdx;
  pairVec[rightIdx].parent = pairIdx;
  return pairIdx;
}

auto explode_pair(PairIndex p) {
  auto const &pair{pairVec[p]};
  // the pair's left value is added to the first regular number to the left of
  // the exploding pair (if any).
  auto const &left{regNumVec[get<RegNumIndex>(pair.leftIdx)]};
  if (left.leftIdx != invalid_index) {
    regNumVec[left.leftIdx].value += left.value;
  }
  // the pair's right value is added to the first regular number to the right of
  // the exploding pair (if any).
  auto const &right{regNumVec[get<RegNumIndex>(pair.rightIdx)]};
  if (right.rightIdx != invalid_index) {
    regNumVec[right.rightIdx].value += right.value;
  }
  // the entire exploding pair is replaced with the regular number 0.
  regNumVec.emplace_back(0, left.leftIdx, right.rightIdx);
  return get_last_index(regNumVec);
}

auto split_literal(RegNumIndex l, PairIndex parent) {
  auto const &literal{regNumVec[l]};

  // the left element of the pair should be the regular number divided by two
  // and rounded **down**
  regNumVec.emplace_back(literal.value / 2, literal.leftIdx, RegNumIndex{-1});
  auto const leftLiteralIndex{get_last_index(regNumVec)};

  // the right element of the pair should be the regular number divided by two
  // and rounded **up**
  regNumVec.emplace_back((literal.value + 1) / 2, leftLiteralIndex,
                         literal.rightIdx);
  auto const rightLiteralIndex{get_last_index(regNumVec)};

  // (fix reference)
  regNumVec[leftLiteralIndex].rightIdx = rightLiteralIndex;

  // replace it with a pair
  pairVec.emplace_back(parent, leftLiteralIndex, rightLiteralIndex);
  return get_last_index(pairVec);
}

bool tryExplode(PairIndex pi) { return false; }
bool trySplit(PairIndex pi) { return false; }

void reduce(PairIndex pi) {
  while (tryExplode(pi) || trySplit(pi)) {
  }
}

int main() {
  auto const snailFishNumbers{readinputdata<std::string>(  //
      std::stringstream{testData}                          //
      // std::ifstream{"input"} //
      )};
  // WTF this assignment! Poor snailfish kids.
  auto const outerPairIdxs{[&] {
    auto outerPairIdxs{std::vector<PairIndex>{}};
    outerPairIdxs.reserve(size(snailFishNumbers));
    transform(cbegin(snailFishNumbers), cend(snailFishNumbers),
              back_inserter(outerPairIdxs), [](auto const &str) {
                auto it{cbegin(str)};  // need lvalue
                return generate_pair(it);
              });
    return outerPairIdxs;
  }()};

  return 0;
}