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

using PairIndex = Named<int, struct index_for_vector_of_Pair>;
using LiteralIndex = Named<int, struct index_for_vector_of_Literal>;

struct Literal {
  int value{};
  LiteralIndex leftLiteralIndex{}, rightLiteralIndex{};
};

using IndexVariant = std::variant<PairIndex, LiteralIndex>;

struct Pair {
  PairIndex parent{};
  IndexVariant left{}, right{};
};

static auto pairVec{std::vector<Pair>{}};
static auto literalVec{std::vector<Literal>{}};
static auto leftLiteralIndex{LiteralIndex{-1}};

auto generate_pair(std::string::const_iterator &it, PairIndex parent)
    -> PairIndex;

auto generate_single(std::string::const_iterator &it, PairIndex pairIdx)
    -> IndexVariant {
  if (*it == '[') {
    return generate_pair(it, pairIdx);
  } else {
    literalVec.emplace_back(*it - '0', leftLiteralIndex, LiteralIndex{-1});
    auto const litIdx{LiteralIndex{
        static_cast<LiteralIndex::UnderlyingType>(size(literalVec))}};
    if (leftLiteralIndex != -1) {
      literalVec[leftLiteralIndex].rightLiteralIndex = litIdx;
    }
    leftLiteralIndex = litIdx;
    return litIdx;
  };
}

auto generate_pair(std::string::const_iterator &it,
                   PairIndex parent = PairIndex{-1}) -> PairIndex {
  pairVec.emplace_back();
  auto const pairIdx{
      PairIndex{static_cast<PairIndex::UnderlyingType>(size(pairVec))}};
  auto &pair{pairVec.back()};
  pair.parent = parent;

  ++it;  // skip [
  pair.left = generate_single(it, pairIdx);
  ++it;  // skip comma
  pair.right = generate_single(it, pairIdx);
  ++it;  // skip ]

  return pairIdx;
}

auto explode_pair(PairIndex p) {
  auto const &pair{pairVec[p]};
  // the pair's left value is added to the first regular number to the left of
  // the exploding pair (if any)
  auto const &left{literalVec[get<LiteralIndex>(pair.left)]};
  if (left.leftLiteralIndex != -1) {
    literalVec[left.leftLiteralIndex].value += left.value;
  }
  // the pair's right value is added to the first regular number to the right of
  // the exploding pair (if any)
  auto const &right{literalVec[get<LiteralIndex>(pair.right)]};
  if (right.rightLiteralIndex != -1) {
    literalVec[right.rightLiteralIndex].value += right.value;
  }
  // the entire exploding pair is replaced with the regular number 0
  literalVec.emplace_back(0, left.leftLiteralIndex, right.rightLiteralIndex);
  return LiteralIndex{
      static_cast<LiteralIndex::UnderlyingType>(size(literalVec))};
}

auto split_literal(LiteralIndex l, PairIndex parent) {
  auto const &literal{literalVec[l]};

  // the left element of the pair should be the regular number divided by two
  // and rounded **down**
  literalVec.emplace_back(literal.value / 2, literal.leftLiteralIndex,
                          LiteralIndex{-1});
  auto const leftLiteralIndex{LiteralIndex{
      static_cast<LiteralIndex::UnderlyingType>(size(literalVec))}};

  // the right element of the pair should be the regular number divided by two
  // and rounded **up**
  literalVec.emplace_back((literal.value + 1) / 2, leftLiteralIndex,
                          literal.rightLiteralIndex);
  auto const rightLiteralIndex{LiteralIndex{
      static_cast<LiteralIndex::UnderlyingType>(size(literalVec))}};

  // (fix reference)
  literalVec[leftLiteralIndex].rightLiteralIndex = rightLiteralIndex;

  // replace it with a pair
  pairVec.emplace_back(parent, leftLiteralIndex, rightLiteralIndex);
  return PairIndex{static_cast<PairIndex::UnderlyingType>(size(pairVec))};
}

int main() {
  auto const snailFishNumbers{readinputdata<std::string>(  //
      std::stringstream{testData}                          //
      // std::ifstream{"input"} //
      )};
  // WTF this assignment! Poor snailfish kids.
  auto const outerPairs{[&] {
    auto outerPairs{std::vector<PairIndex>{}};
    outerPairs.reserve(size(snailFishNumbers));
    transform(cbegin(snailFishNumbers), cend(snailFishNumbers),
              back_inserter(outerPairs), [](auto const str) {
                auto it{cbegin(str)};  // need lvalue
                return generate_pair(it);
              });
    return outerPairs;
  }()};

  return 0;
}