#include "../matrix.hpp"
#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

static constexpr auto testdata{"5483143223\n"
                               "2745854711\n"
                               "5264556173\n"
                               "6141336146\n"
                               "6357385478\n"
                               "4167524645\n"
                               "2176841721\n"
                               "6882881134\n"
                               "4846848554\n"
                               "5283751526\n"};

static constexpr auto FLASH_THRESHOLD{9};

void increase_all_by_one(matrix<int> &mat) {
  transform(cbegin(mat.data), cend(mat.data), begin(mat.data),
            [](int i) { return i + 1; });
}

void increase_neighbours(matrix<int> &mat, int x, int y) {
  for (auto ymod{-1}; ymod < 2; ++ymod) {
    for (auto xmod{-1}; xmod < 2; ++xmod) {
      auto const newx{x + xmod}, newy{y + ymod};
      if (newx >= 0 && newx < static_cast<int>(mat.width) && newy >= 0 &&
          newy < static_cast<int>(mat.height)) {
        ++mat(newx, newy);
      }
    }
  }
}

void resolve_flashes(matrix<int> &mat) {
  auto hasFlashed{matrix<uint8_t>(mat.width, mat.height)}; // I hate vector bool
  while (true) {
    auto flashes{false};
    for (auto y{0}; y < static_cast<int>(mat.height); ++y) {
      for (auto x{0}; x < static_cast<int>(mat.width); ++x) {
        if (mat(x, y) > FLASH_THRESHOLD && !hasFlashed(x, y)) {
          increase_neighbours(mat, x, y);
          hasFlashed(x, y) = true;
          flashes = true;
        }
      }
    }
    if (!flashes)
      break;
  }
}

auto count_and_reset_flashes(matrix<int> &mat) {
  // std::transform_reduce(begin(mat.data), end(mat.data), 0, std::plus<>{},
  //                       [](int &val) {
  //                         if (val > 9) {
  //                           val = 0; // modifying original data is UB?
  //                           return 1;
  //                         }
  //                         return 0;
  //                       });
  auto flashes{0};
  for (auto &val : mat.data) {
    if (val > FLASH_THRESHOLD) {
      val = 0;
      ++flashes;
    }
  }
  return flashes;
  // might be able to transform into mask, reduce mask and use mask to transform
  // matrix
}

int main() {
  auto grid{[] {
    auto const stringvec{readinputdata<std::string>(
        /*std::stringstream{testdata}*/ std::fstream{"input"})};
    auto vecvec{std::vector<std::vector<int>>{}};
    vecvec.reserve(size(stringvec));
    std::transform(cbegin(stringvec), cend(stringvec), back_inserter(vecvec),
                   [](std::string line) {
                     auto vec{std::vector<int>{}};
                     vec.reserve(size(line));
                     std::transform(cbegin(line), cend(line),
                                    back_inserter(vec),
                                    [](char c) { return c - '0'; });
                     return vec;
                   });
    return matrix<int>{vecvec};
  }()};

  auto totalFlashes{0};
  auto n{0};
  for (; n < 100; ++n) {
    increase_all_by_one(grid);
    resolve_flashes(grid);
    totalFlashes += count_and_reset_flashes(grid);
    // if (is_any_value_out_of(n, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 19, 29, 39, 49,
    //                             59, 69, 79, 89, 99})) {
    //   std::cout << grid << '\n';
    // }
    // if (n==9) {
    //   std::cout << "Flashes after step 10: " << totalFlashes
    //             << std::endl;
    // }
  }
  std::cout << "Flashes after step 100: " << totalFlashes << std::endl;
  auto const allFlashCount{static_cast<int>(grid.width * grid.height)};
  for (;; ++n) {
    increase_all_by_one(grid);
    resolve_flashes(grid);
    if (count_and_reset_flashes(grid) == allFlashCount)
      break;
  }
  std::cout << "Octopi synchronize after step: " << n + 1 << std::endl;
}