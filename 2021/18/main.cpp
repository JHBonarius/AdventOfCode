#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

#include "../my_algorithm.hpp"
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

auto &operator<<(std::ostream &os, IndexVariant iv) {
  std::visit(
      overloaded{[&](PairIndex pi) { os << "pair idx: " << pi.get(); },
                 [&](RegNumIndex ri) { os << "reg num idx: " << ri.get(); }},
      iv);
  return os;
}

struct Pair {
  using IndexType = PairIndex;

  PairIndex parent{};
  IndexVariant leftIdx{}, rightIdx{};
};

static auto pairVec{std::vector<Pair>{}};
static auto regNumVec{std::vector<RegularNumber>{}};

static constexpr auto invalid_index{-1};

template <typename Type>
auto get_last_index(std::vector<Type> const &vec) {
  using Index = typename Type::IndexType;
  return Index{static_cast<typename Index::UnderlyingType>(size(vec) - 1)};
}

auto generate_pair(std::string::const_iterator &it, RegNumIndex &leftRegNumIdx,
                   PairIndex parent) -> PairIndex;

auto generate_side(std::string::const_iterator &it, RegNumIndex &leftRegNumIdx,
                   PairIndex pairIdx) -> IndexVariant {
  if (*it == '[') {
    return generate_pair(it, leftRegNumIdx, pairIdx);
  }
  // else {
  regNumVec.emplace_back(*it++ - '0', leftRegNumIdx,
                         RegNumIndex{invalid_index});
  auto const regNumIdx{get_last_index(regNumVec)};
  if (leftRegNumIdx != invalid_index) {
    regNumVec[leftRegNumIdx].rightIdx = regNumIdx;
  }
  leftRegNumIdx = regNumIdx;
  return regNumIdx;
  //};
}

auto generate_pair(std::string::const_iterator &it, RegNumIndex &leftRegNumIdx,
                   PairIndex parent = PairIndex{invalid_index}) -> PairIndex {
  pairVec.emplace_back();
  auto const pairIdx{get_last_index(pairVec)};
  pairVec[pairIdx].parent = parent;

  // NEED to use the index instead of reference, since the calls to
  // "generate_side" will reallocate the pairVec
  ++it;  // skip [
  pairVec[pairIdx].leftIdx = generate_side(it, leftRegNumIdx, pairIdx);
  // std::cout << "Check pair leftidx: " << pair.leftIdx << '\n';
  ++it;  // skip comma
  pairVec[pairIdx].rightIdx = generate_side(it, leftRegNumIdx, pairIdx);
  // std::cout << "Check pair rightidx: " << pair.rightIdx << '\n';
  ++it;  // skip ]

  // std::cout << "return pair index: " << pairIdx.get() << '\n';
  return pairIdx;
}

template <typename Projection>
auto get_outer_regnumidx(Pair const &pair, Projection proj) {
  auto idx{std::invoke(proj, pair)};
  while (holds_alternative<PairIndex>(idx)) {
    idx = std::invoke(proj, pairVec[get<PairIndex>(idx)]);
  }
  return get<RegNumIndex>(idx);
}

// To add two snailfish numbers, form a pair from the left and right parameters
// of the addition operator.
auto add_pair(PairIndex leftIdx, PairIndex rightIdx) {
  pairVec.emplace_back(PairIndex{invalid_index}, leftIdx, rightIdx);
  auto const pairIdx{get_last_index(pairVec)};

  // update childs' parent
  auto &left = pairVec[leftIdx];
  left.parent = pairIdx;
  auto &right = pairVec[rightIdx];
  right.parent = pairIdx;

  // finally update regular number connections
  auto const leftRegNumIdx = get_outer_regnumidx(left, &Pair::rightIdx);
  auto const rightRegNumIdx = get_outer_regnumidx(right, &Pair::leftIdx);
  regNumVec[rightRegNumIdx].rightIdx = leftRegNumIdx;
  regNumVec[leftRegNumIdx].leftIdx = rightRegNumIdx;

  return pairIdx;
}

template <typename Projection>
auto add_value_to_neighbor(IndexVariant myIdx, Projection proj,
                           RegNumIndex newIdx) {
  auto const &current{regNumVec[get<RegNumIndex>(myIdx)]};
  auto const neighborIdx = std::invoke(proj, current);
  if (neighborIdx != invalid_index) {
    auto &neighbor = regNumVec[neighborIdx];
    neighbor.value += current.value;
    neighbor.rightIdx = newIdx;
  }
  return neighborIdx;
}

auto explode_pair(PairIndex p) {
  // the entire exploding pair is replaced with the regular number 0.
  regNumVec.emplace_back(0);
  auto const newIdx = get_last_index(regNumVec);

  auto const &pair{pairVec[p]};
  // the pair's left value is added to the first regular number to the left of
  // the exploding pair (if any).
  auto const leftIdx =
      add_value_to_neighbor(pair.leftIdx, &RegularNumber::leftIdx, newIdx);

  // the pair's right value is added to the first regular number to the right of
  // the exploding pair (if any).
  auto const rightIdx =
      add_value_to_neighbor(pair.rightIdx, &RegularNumber::rightIdx, newIdx);

  // finally update regular number connections
  auto &newRegNum = regNumVec[newIdx];
  newRegNum.leftIdx = leftIdx;
  newRegNum.rightIdx = rightIdx;

  return newIdx;
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

// bool tryExplode(PairIndex pi) { return false; }
// bool trySplit(PairIndex pi) { return false; }

// void reduce(PairIndex pi) {
//   while (tryExplode(pi) || trySplit(pi)) {
//   }
// }

void print(RegNumIndex ri) { std::cout << regNumVec[ri].value; }

void print(PairIndex pi) {
  std::cout << '[';
  std::visit([](auto &&i) { print(i); }, pairVec[pi].leftIdx);
  std::cout << ',';
  std::visit([](auto &&i) { print(i); }, pairVec[pi].rightIdx);
  std::cout << ']';
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
                auto leftRegNumIdx{RegNumIndex{invalid_index}};
                return generate_pair(it, leftRegNumIdx);
              });
    return outerPairIdxs;
  }()};

  // auto pi{0};
  // for (auto const &pair : pairVec) {
  //   std::cout << "Pair " << pi++ << ": parent idx: " << pair.parent.get()
  //             << ", left " << pair.leftIdx << ", right " << pair.rightIdx
  //             << '\n';
  // }
  // auto rni{0};
  // for (auto const &regNum : regNumVec) {
  //   std::cout << "Regular number " << rni++ << ": val: " << regNum.value
  //             << ", left: " << regNum.leftIdx.get()
  //             << ", right: " << regNum.rightIdx.get() << '\n';
  // }
  // rni = 0;
  // while (true) {
  //   std::cout << regNumVec[rni].value << ", ";
  //   if (regNumVec[rni].rightIdx == invalid_index) {
  //     break;
  //   }
  //   rni = regNumVec[rni].rightIdx;
  // }
  // std::cout << '\n';

  for (auto outerPairIdx : outerPairIdxs) {
    print(outerPairIdx);
    std::cout << '\n';
  }

  return 0;
}