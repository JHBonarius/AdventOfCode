#include "../readinputdata.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto testdata{"2199943210\n"
                               "3987894921\n"
                               "9856789892\n"
                               "8767896789\n"
                               "9899965678\n"};

struct point {
  int x{}, y{};
};

int main() {
  auto const data{readinputdata<std::string>(
      /*std::stringstream{testdata}*/ std::fstream{"input"})};
  auto heightMap{[&] {
    auto heightMap{std::vector<std::vector<uint8_t>>{}};
    heightMap.reserve(size(data));
    std::transform(cbegin(data), cend(data), back_inserter(heightMap),
                   [](std::string const &str) {
                     auto heightRow{std::vector<uint8_t>{}};
                     heightRow.reserve(size(str));
                     std::transform(cbegin(str), cend(str),
                                    back_inserter(heightRow),
                                    [](char c) { return c - '0'; });
                     return heightRow;
                   });
    return heightMap;
  }()};

  auto const width{static_cast<int>(size(heightMap[0]))};
  auto const height{static_cast<int>(size(heightMap))};

  auto const lowPoints{[&] {
    auto points{std::vector<point>{}};
    for (auto y{0}; y < height; ++y) {
      auto const ym1{y - 1}, yp1{y + 1};
      auto const &heightRow{heightMap[y]};
      for (auto x{0}; x < width; ++x) {
        auto const xm1{x - 1}, xp1{x + 1};
        auto const val{heightRow[x]};
        if ((ym1 < 0 || heightMap[ym1][x] > val) &&
            (yp1 >= height || heightMap[yp1][x] > val) &&
            (xm1 < 0 || heightRow[xm1] > val) &&
            (xp1 >= width || heightRow[xp1] > val)) {
          points.push_back(point{x, y});
        }
      }
    }
    return points;
  }()};

  std::cout << "Number of low points: " << size(lowPoints) << '\n';

  std::cout << "Combined risk level:"
            << transform_reduce(
                   cbegin(lowPoints), cend(lowPoints), 0, std::plus<>{},
                   [&](point p) { return heightMap[p.y][p.x] + 1; })
            << std::endl;

  // modifies the heightmap!!
  auto const floodfill{[&](point p, auto &&floodfill) -> int {
    if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height ||
        heightMap[p.y][p.x] > 8) {
      return 0;
    }
    heightMap[p.y][p.x] = 10;
    return 1 + floodfill({p.x, p.y - 1}, floodfill) +
           floodfill({p.x, p.y + 1}, floodfill) +
           floodfill({p.x - 1, p.y}, floodfill) +
           floodfill({p.x + 1, p.y}, floodfill);
  }};

  auto const basinSizes{[&] {
    auto basinSizes{std::vector<int>{}};
    basinSizes.reserve(size(lowPoints));
    transform(cbegin(lowPoints), cend(lowPoints), back_inserter(basinSizes),
              [&](auto startingPoint) {
                return floodfill(startingPoint, floodfill);
              });
    return basinSizes;
  }()};

  auto const threeLargest{[&] {
    auto threeLargest{std::array<int, 3>{}};
    partial_sort_copy(cbegin(basinSizes), cend(basinSizes), begin(threeLargest),
                      end(threeLargest), std::greater<>{});
    return threeLargest;
  }()};

  auto const result = std::reduce(cbegin(threeLargest), cend(threeLargest), 1,
                                  std::multiplies<>{});
  std::cout << "product of 3 largest basins: " << result << std::endl;
}