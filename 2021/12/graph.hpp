#ifndef GRAPH_HPP
#define GRAPH_HPP
//#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

// https://stackoverflow.com/a/58446
class graph {
 private:
  std::map<std::string, std::set<std::string>> map{};

 public:
  void add_edge(std::string const &node1, std::string const &node2) {
    map[node1].insert(node2);
  }

  void add_two_way_vertex(std::string const &node1, std::string const &node2) {
    add_edge(node1, node2);
    add_edge(node2, node1);
  }

  // NOTE: returns a copy
  auto adjacent_nodes(std::string const &last) const {
    auto const it = map.find(last);
    if (it == std::end(map)) {
      return std::vector<std::string>{};
    }
    return std::vector(cbegin((*it).second), cend((*it).second));
  }

  friend auto &operator<<(std::ostream &os, graph const &g) {
    for (auto &&node : g.map) {
      os << "Node " << node.first << ": ";
      for (auto &&other : node.second) {
        os << other << ", ";
      }
      os << '\n';
    }
    return os;
  }
};

#endif