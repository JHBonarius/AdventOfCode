#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

struct Loc {
  int x{}, y{};
};

// std::istream &operator>>(std::istream &is, Loc &loc) {
//   std::string locStr;
//   is >> locStr;
//   std::stringstream locStrSs{locStr};
//   locStrSs >> loc.x;
//   locStrSs.ignore();
//   locStrSs >> loc.y;
//   return is;
// }

struct Line {
  Loc start{}, end{};
};

std::istream &operator>>(std::istream &is, Line &line) {
  std::string dummy;
  // is >> line.start >> dummy >> line.end;
  is >> line.start.x;
  is.ignore();
  is >> line.start.y;
  is >> dummy;
  is >> line.end.x;
  is.ignore();
  is >> line.end.y;
  return is;
}

template <typename T> auto readinputdata() {
  std::vector<T> data;
  std::ifstream fs("input");
  T val;
  while (fs >> val) {
    data.push_back(val);
  }
  return data;
}

int main() {
  // std::vector<Line> const lines{
  //     {{0, 9}, {5, 9}}, {{8, 0}, {0, 8}}, {{9, 4}, {3, 4}}, {{2, 2}, {2, 1}},
  //     {{7, 0}, {7, 4}}, {{6, 4}, {2, 0}}, {{0, 9}, {2, 9}}, {{3, 4}, {1, 4}},
  //     {{0, 0}, {8, 8}}, {{5, 5}, {8, 2}}};
  auto const lines = readinputdata<Line>();
  // only horizontal & vertical lines
  // std::erase_if(lines, [](Line line) {
  //       return line.start.x != line.end.x && line.start.y != line.end.y;
  //   // auto result = line.start.x != line.end.x && line.start.y !=
  //   line.end.y;
  //   // std::cout << (result ? "yes " : "no ");
  //   // return result;
  // });
  // std::cout << std::endl;
  auto const max = std::transform_reduce(
      cbegin(lines), cend(lines), Loc{},
      [](Loc curr, Loc val) {
        return Loc{std::max(curr.x, val.x), std::max(curr.y, val.y)};
      },
      [](Line line) {
        return Loc{std::max(line.start.x, line.end.x),
                   std::max(line.start.y, line.end.y)};
      });
  std::vector<int> map((max.x + 1) * (max.y + 1), 0);
  auto loc = [maxx = max.x](int x, int y) { return y * (maxx + 1) + x; };
  for (auto line : lines) {
    auto const dx = std::clamp(line.end.x - line.start.x, -1, 1);
    auto const dy = std::clamp(line.end.y - line.start.y, -1, 1);
    auto const steps = std::max(std::abs(line.end.x - line.start.x),
                                std::abs(line.end.y - line.start.y)) +
                       1;
    for (auto n{0}, x{line.start.x}, y{line.start.y}; n < steps;
         ++n, x += dx, y += dy) {
      map[loc(x, y)] += 1;
    }
  }
  // int i{0};
  // for (auto y(0); y <= max.y; ++y) {
  //   for (auto x{0}; x <= max.x; ++x) {
  //     std::cout << map[i++];
  //   }
  //   std::cout << '\n';
  // }
  auto const result =
      std::count_if(cbegin(map), cend(map), [](int loc) { return loc > 1; });
  std::cout << "result: " << result << std::endl;
}