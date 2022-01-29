#include "../named.hpp"
#include "../readinputdata.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

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

static constexpr const char pairIndexName[]{"pair index"};
using PairIndex = Named<int, pairIndexName>;
static constexpr const char literalIndexName[]{"literal index"};
using LiteralIndex = Named<int, literalIndexName>;

struct Literal {
  int value{};
  LiteralIndex left{}, right{};
};

using IndexVariant = std::variant<PairIndex, LiteralIndex>;

struct Pair {
  PairIndex parent{};
  IndexVariant left{}, right{};
};

static auto pairVec{std::vector<Pair>{}};
static auto literalVec{std::vector<Literal>{}};
static auto left{LiteralIndex{-1}};

auto generate_pair(std::string::const_iterator &it, PairIndex parent)
    -> PairIndex;

auto generate_single(std::string::const_iterator &it, PairIndex pairIdx)
    -> IndexVariant {
  if (*it == '[') {
    return generate_pair(it, pairIdx);
  } else {
    literalVec.emplace_back(*it - '0', left, LiteralIndex{-1});
    auto const litIdx{LiteralIndex{
        static_cast<LiteralIndex::UnderlyingType>(literalVec.size())}};
    auto const leftIdx{static_cast<LiteralIndex::UnderlyingType>(left)};
    if (leftIdx != -1) {
      literalVec[leftIdx].right = litIdx;
    }
    left = litIdx;
    return litIdx;
  };
}

auto generate_pair(std::string::const_iterator &it,
                   PairIndex parent = PairIndex{-1}) -> PairIndex {
  pairVec.emplace_back();
  auto const pairIdx{PairIndex{static_cast<int>(pairVec.size())}};
  auto &pair{pairVec.back()};
  pair.parent = parent;

  ++it; // skip [
  pair.left = generate_single(it, pairIdx);
  ++it; // skip comma
  pair.right = generate_single(it, pairIdx);
  ++it; // skip ]

  return pairIdx;
}

int main() {
  auto const snailFishNumbers{readinputdata<std::string>( //
      std::stringstream{testData}                         //
                                  // std::ifstream{"input"} //
      )};
  // WTF this assignment! Poor snailfish kids.
  auto const outerPairs{[&] {
    auto outerPairs{std::vector<PairIndex>{}};
    outerPairs.reserve(size(snailFishNumbers));
    transform(cbegin(snailFishNumbers), cend(snailFishNumbers),
              back_inserter(outerPairs), [](auto const str) {
                auto it{cbegin(str)}; // need lvalue
                return generate_pair(it);
              });
    return outerPairs;
  }()};

  return 0;
}