#include "../readinputdata.hpp"
#include "graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto testData1{"start-A\n"
                                "start-b\n"
                                "A-c\n"
                                "A-b\n"
                                "b-d\n"
                                "A-end\n"
                                "b-end\n"};

static constexpr auto testData2{"dc-end\n"
                                "HN-start\n"
                                "start-kj\n"
                                "dc-start\n"
                                "dc-HN\n"
                                "LN-dc\n"
                                "HN-end\n"
                                "kj-sa\n"
                                "kj-HN\n"
                                "kj-dc\n"};

static constexpr auto testData3{"fs-end\n"
                                "he-DX\n"
                                "fs-he\n"
                                "start-DX\n"
                                "pj-DX\n"
                                "end-zg\n"
                                "zg-sl\n"
                                "zg-pj\n"
                                "pj-he\n"
                                "RW-he\n"
                                "fs-DX\n"
                                "pj-RW\n"
                                "zg-RW\n"
                                "start-pj\n"
                                "he-WI\n"
                                "zg-he\n"
                                "pj-fs\n"
                                "start-RW\n"};

static constexpr auto start{"start"};
static constexpr auto end{"end"};

void print_path(std::vector<std::string> const &vec) {
  std::cout << "Path: ";
  for (auto &&node : vec) {
    std::cout << node << " ";
  }
  std::cout << end << '\n';
}

auto dfs(graph const &graph) {
  auto nrOfPaths{0};
  auto visited{std::vector<std::string>{start}};
  auto const may_still_visit{[&](std::string const &node) {
    return isupper( // UB when char is not convertible to unsigned char
               node[0]) ||
           find(cbegin(visited), cend(visited), node) == cend(visited);
  }};
  auto const inner{[&](auto &&inner, bool has_visited_twice) -> void {
    auto const nodes{graph.adjacent_nodes(visited.back())};
    if (find(cbegin(nodes), cend(nodes), end) != cend(nodes)) {
      ++nrOfPaths;
      // print_path(visited);
    }
    for (auto const &node : nodes) {
      if (node != start && node != end) {
        if (may_still_visit(node)) {
          visited.push_back(node);
          inner(inner, has_visited_twice);
          visited.pop_back();
        } else if (!has_visited_twice) {
          visited.push_back(node);
          inner(inner, true);
          visited.pop_back();
        }
      }
    }
  }};
  inner(inner, false);
  return nrOfPaths;
}

int main() {
  auto const inputStrs{readinputdata<std::string>(
      // clang-format off
      // std::stringstream{testData1}
      // std::stringstream{testData2}
      // std::stringstream{testData3}
      std::ifstream{"input"} // clang-format on
      )};
  auto const paths{[&] {
    auto paths{std::vector<std::pair<std::string, std::string>>{}};
    paths.reserve(size(inputStrs));
    transform(cbegin(inputStrs), cend(inputStrs), back_inserter(paths),
              [](std::string const &str) {
                auto const hyphenPos = str.find("-");
                return make_pair(str.substr(0, hyphenPos),
                                 str.substr(hyphenPos + 1, std::string::npos));
              });
    return paths;
  }()};
  // for (auto const& connection : input) {
  //   std::cout << connection.first << ", " << connection.second << '\n';
  // }
  auto const node_graph{[&] {
    auto g{graph{}};
    for (auto const &path : paths) {
      g.add_two_way_vertex(path.first, path.second);
    }
    return g;
  }()};
  // std::cout << node_graph << '\n';
  std::cout << "Number of paths " << dfs(node_graph) << std::endl;
}