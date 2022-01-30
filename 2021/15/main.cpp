#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "../matrix.hpp"
#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"

static constexpr auto testData{
    "1163751742\n"
    "1381373672\n"
    "2136511328\n"
    "3694931569\n"
    "7463417111\n"
    "1319128137\n"
    "1359912421\n"
    "3125421639\n"
    "1293138521\n"
    "2311944581\n"};

static constexpr auto infinity{std::numeric_limits<int>().max()};

struct Node {
  // size=2 to 4, so could use fixed_vector
  std::vector<std::pair<Node *, uint8_t>> path_to_neighbors{};
  int lowest_distance{infinity};
  bool is_visited{false};
};

static constexpr auto fieldDupe{5};

int main() {
  auto graph{[&] {
    auto const strVec{
        readinputdata<std::string>(  // std::stringstream{"8"}
                                     // std::stringstream{testData}
            std::fstream{"input"})};

    auto const intVec{[&] {
      auto intVec{std::vector<std::vector<uint8_t>>{}};
      intVec.reserve(size(strVec) * fieldDupe);
      auto yIt = back_inserter(intVec);
      for (auto y{0}; y < fieldDupe; ++y) {
        yIt =
            transform(cbegin(strVec), cend(strVec), yIt, [=](auto const &str) {
              auto intRow{std::vector<uint8_t>{}};
              intRow.reserve(size(str) * fieldDupe);
              auto xIt = back_inserter(intRow);
              for (auto x{0}; x < fieldDupe; ++x) {
                xIt = transform(cbegin(str), cend(str), xIt, [=](auto c) {
                  return ((c - '0') + x + y - 1) % 9 + 1;
                });
              }
              return intRow;
            });
      }
      return intVec;
    }()};

    auto const width{size(intVec[0])};
    auto const height{size(intVec)};

    // for (auto y{0}; y < height; ++y) {
    //   for (auto x{0}; x < width; ++x) {
    //     std::cout << static_cast<int>(intVec[y][x]);
    //   }
    //   std::cout << '\n';
    // }

    auto graph{matrix<Node>(width, height)};
    // add neighbours
    for (auto y{0}; y < height; ++y) {
      for (auto x{0}; x < width; ++x) {
        auto const make_connection{[&](size_t x_o, size_t y_o) {
          graph(x, y).path_to_neighbors.push_back(
              std::pair{&graph(x_o, y_o), intVec[y_o][x_o]});
        }};
        if (x > 0) {
          make_connection(x - 1, y);
        }
        if (y > 0) {
          make_connection(x, y - 1);
        }
        if (x < width - 1) {
          make_connection(x + 1, y);
        }
        if (y < height - 1) {
          make_connection(x, y + 1);
        }
      }
    }
    return graph;
  }()};

  // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
  // 1)
  auto unvisited{[&] {
    auto unvisited{std::list<Node *>{}};
    transform(begin(graph.data), end(graph.data), back_inserter(unvisited),
              [](auto &node) { return &node; });
    return unvisited;
  }()};
  // 2)
  auto currentIt{begin(unvisited)};
  (*currentIt)->lowest_distance = 0;
  // 5) mod
  while (*currentIt != unvisited.back()) {
    auto &current{**currentIt};
    // 3)
    for (auto &path_to_neighbor : current.path_to_neighbors) {
      auto &neighbor{*path_to_neighbor.first};
      if (!neighbor.is_visited) {
        neighbor.lowest_distance =
            std::min(neighbor.lowest_distance,
                     current.lowest_distance + path_to_neighbor.second);
      }
    }
    // 4)
    current.is_visited = true;
    unvisited.erase(currentIt);
    // 6)
    currentIt = transform_min_element(
        begin(unvisited), end(unvisited),
        [](auto neighbor) { return neighbor->lowest_distance; });
  }

  std::cout << "Min path to end: " << (*currentIt)->lowest_distance << '\n';
}