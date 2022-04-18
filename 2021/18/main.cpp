#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "../readinputdata.hpp"
#include "SnailFishNumbers.hpp"

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

int main() {
  auto const snailFishNumbers{
      // readinputdata<std::string>(  //
      // std::stringstream{testData}                          //
      // std::ifstream{"input"} //
      std::vector<std::string>{"[0,[1,2]]", "[3,4]"}
      //)//
  };

  auto sfn{SnailFishNumbers{}};

  // WTF this assignment! Poor snailfish kids.
  auto const outerPairIdxs{[&] {
    auto outerPairIdxs{std::vector<PairIndex>{}};
    outerPairIdxs.reserve(size(snailFishNumbers));
    transform(cbegin(snailFishNumbers), cend(snailFishNumbers),
              back_inserter(outerPairIdxs), [&sfn](auto const &str) {
                auto it{cbegin(str)};  // need lvalue
                auto leftRegNumIdx{RegNumIndex{invalid_index}};
                return sfn.generate_pair(it, leftRegNumIdx);
              });
    return outerPairIdxs;
  }()};

  for (auto outerPairIdx : outerPairIdxs) {
    sfn.print(outerPairIdx);
    std::cout << '\n';
  }

  // auto const addedIdx{add_pair(outerPairIdxs[0], outerPairIdxs[1])};
  // print(addedIdx);
  // std::cout << '\n';
  // // dumpVecs();

  // pairVec[0].rightIdx = explode_pair(PairIndex{1});
  // print(addedIdx);
  // std::cout << '\n';
  // // dumpVecs();

  // pairVec[2].leftIdx = split_literal(RegNumIndex{3}, PairIndex{2});
  // print(addedIdx);
  // std::cout << '\n';
  // dumpVecs();

  return 0;
}