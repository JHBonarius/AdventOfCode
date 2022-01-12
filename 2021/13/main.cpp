#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <tuple>
#include <vector>

static constexpr auto testData{
    "6,10\n0,14\n9,10\n0,3\n10,4\n4,11\n6,0\n6,12\n4,1\n"
    "0,13\n10,12\n3,4\n3,0\n8,4\n1,10\n2,14\n8,10\n9,0\n"
    "\n"
    "fold along y=7\n"
    "fold along x=5\n"};

struct point {
  size_t x{}, y{};
};

struct matrix {
  size_t width{}, height{};
  // I wish vector<bool> wasnt a thing.
  std::vector<std::vector<uint8_t>> data{};
};

auto fold_up(matrix const &mat) {
  auto newHeight{mat.height / 2};
  auto retMat{matrix{mat.width, newHeight, {}}};
  retMat.data.reserve(newHeight);
  transform(cbegin(mat.data), next(cbegin(mat.data), newHeight),
            crbegin(mat.data), back_inserter(retMat.data),
            [](auto const &upperRow, auto const &lowerRow) {
              auto retRow{decltype(upperRow){}};
              retRow.reserve(size(upperRow));
              transform(cbegin(upperRow), cend(upperRow), cbegin(lowerRow),
                        back_inserter(retRow), std::logical_or<>{});
              return retRow;
            });
  return retMat;
}

auto fold_left(matrix const &mat) {
  auto newWidth{mat.width / 2};
  auto retMat{matrix{newWidth, mat.height, {}}};
  retMat.data.reserve(mat.height);
  transform(cbegin(mat.data), cend(mat.data), back_inserter(retMat.data),
            [=](auto const &row) {
              auto retRow{decltype(row){}};
              retRow.reserve(newWidth);
              transform(cbegin(row), next(cbegin(row), newWidth), crbegin(row),
                        back_inserter(retRow), std::logical_or<>{});
              return retRow;
            });
  return retMat;
}

auto &print(std::ostream &os, matrix const &mat) {
  for (auto const &row : mat.data) {
    for (auto el : row) {
      std::cout << (el == 0 ? '.' : '#');
    }
    std::cout << '\n';
  }
  return os;
}

auto count_dots(matrix const &mat) {
  return transform_reduce(
      cbegin(mat.data), cend(mat.data), 0, std::plus<>{},
      [](auto const &row) { return reduce(cbegin(row), cend(row)); });
}

enum class fold_type {
  up,
  left,
};

int main() {
  auto const [dots, folds, width, height] = [] {
    auto is{/*std::istringstream{testData}*/ std::ifstream{"input"}};
    auto dots{std::vector<point>{}};
    for (auto str{std::string{}}; getline(is, str) && str.length() != 0;) {
      auto ss{std::istringstream{str}};
      auto x{int{}}, y{int{}};
      ss >> x;
      ss.ignore();
      ss >> y;
      dots.emplace_back(x, y);
    }
    auto width{0}, height{0};
    auto folds{std::vector<fold_type>{}};
    for (auto str{std::string{}}; getline(is, str);) {
      if (str[11] == 'y') {
        if (!height) {
          height = stoi(str.substr(13)) * 2 + 1;
        }
        folds.push_back(fold_type::up);
      } else {
        if (!width) {
          width = stoi(str.substr(13)) * 2 + 1;
        }
        folds.push_back(fold_type::left);
      }
    }
    return std::tuple{dots, folds, width, height};
  }();
  std::cout << width << ", " << height << '\n';

  // for (auto dot : dots) {
  //   std::cout << dot.x << "," << dot.y << '\n';
  // }
  // std::cout << '\n';
  // for (auto fold : folds) {
  //   std::cout << (fold == fold_type::up ? "up" : "left") << '\n';
  // }

  auto paper{[&dots, width, height] {
    auto mat{matrix{width, height, {}}};
    mat.data.resize(mat.height, std::vector<uint8_t>(mat.width));
    for (auto dot : dots) {
      mat.data[dot.y][dot.x] = 1;
    }
    return mat;
  }()};

  for (auto fold : folds) {
    std::cout << "Nr of dots: " << count_dots(paper) << "\n\n";
    paper = (fold == fold_type::up ? fold_up(paper) : fold_left(paper));
  }
  print(std::cout, paper);
  std::cout << "Nr of dots: " << count_dots(paper) << '\n';
}