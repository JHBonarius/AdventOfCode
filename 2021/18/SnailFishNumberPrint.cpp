#include <iostream>

#include "../my_algorithm.hpp"
#include "SnailFishNumbers.hpp"

void SnailFishNumbers::print(RegNumIndex ri) {
  std::cout << regNumVec_[ri].value;
}

void SnailFishNumbers::print(PairIndex pi) {
  std::cout << '[';
  std::visit([this](auto &&i) { print(i); }, pairVec_[pi].leftIdx);
  std::cout << ',';
  std::visit([this](auto &&i) { print(i); }, pairVec_[pi].rightIdx);
  std::cout << ']';
}

auto &operator<<(std::ostream &os, IndexVariant iv) {
  std::visit(
      overloaded{[&](PairIndex pi) { os << "pair idx: " << pi.get(); },
                 [&](RegNumIndex ri) { os << "reg num idx: " << ri.get(); }},
      iv);
  return os;
}

void SnailFishNumbers::dumpVecs() {
  auto pi{0};
  for (auto const &pair : pairVec_) {
    std::cout << "Pair " << pi++ << ": parent idx: " << pair.parent.get()
              << ", left " << pair.leftIdx << ", right " << pair.rightIdx
              << '\n';
  }
  auto rni{0};
  for (auto const &regNum : regNumVec_) {
    std::cout << "Regular number " << rni++ << ": val: " << regNum.value
              << ", left: " << regNum.leftIdx.get()
              << ", right: " << regNum.rightIdx.get() << '\n';
  }
  rni = 0;
  while (true) {
    std::cout << regNumVec_[rni].value << ", ";
    if (regNumVec_[rni].rightIdx == invalid_index) {
      break;
    }
    rni = regNumVec_[rni].rightIdx;
  }
  std::cout << '\n';
}
