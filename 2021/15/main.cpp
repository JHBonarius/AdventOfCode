#include "../matrix.hpp"
#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto testData{"1163751742\n"
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
  std::vector<std::pair<Node *, int>> path_to_neighbors{};
  int lowest_distance{infinity};
  bool is_visited{false};
};

int main() {
  Node *start{nullptr};
  Node *end{nullptr};
  auto graph{[&] {
    auto const strVec{readinputdata<std::string>( // std::stringstream{testData}
        std::fstream{"input"})};
    auto const width{size(strVec[0])};
    auto const height{size(strVec)};
    auto graph{matrix<Node>(width, height)};
    // add neighbours
    for (auto y{0}; y < height; ++y) {
      for (auto x{0}; x < width; ++x) {
        auto const make_connection{[&](size_t x_o, size_t y_o) {
          graph(x, y).path_to_neighbors.push_back(
              std::pair{&graph(x_o, y_o), strVec[y_o][x_o] - '0'});
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
    start = &graph(0, 0);
    end = &graph(width - 1, height - 1);
    return graph;
  }()};

  // dijkstra's algorithm
  // 1)
  auto unvisited{[&] {
    auto unvisited{std::list<Node *>{}};
    transform(std::begin(graph.data), std::end(graph.data),
              back_inserter(unvisited), [](auto &node) { return &node; });
    return unvisited;
  }()};
  // 2)
  start->lowest_distance = 0;
  auto currentIt{unvisited.begin()};
  while (*currentIt != end) {
    auto &current{**currentIt};
    // // 5)
    // if (end->is_visited || current.lowest_distance == infinity) {
    //   break;
    // }
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
        std::begin(unvisited), std::end(unvisited),
        [](auto neighbor) { return neighbor->lowest_distance; });
  }

  std::cout << "Min path to end: " << end->lowest_distance << '\n';
}