#include "../readinputdata.hpp"
#include <concepts>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

static constexpr auto testdata{"[({(<(())[]>[[{[]{<()<>>\n"
                               "[(()[<>])]({[<{<<[]>>(\n"
                               "{([(<{}[<>[]}>{[]{[(<()>\n"
                               "(((({<>}<{<{<>}{[]{[]{}\n"
                               "[[<[([]))<([[{}[[()]]]\n"
                               "[{[{({}]{}}([{[{{{}}([]\n"
                               "{<[[]]>}<{[{[{[]{()[[[]\n"
                               "[<(<(<(<{}))><([]([]()\n"
                               "<{([([[(<>()){}]>(<<{{\n"
                               "<{([{{}}[<[[[<>{}]]]>[]]\n"};

int main() {
  auto const data{readinputdata<std::string>(
      /*std::stringstream{testdata}*/ std::fstream{"input"})};

  auto const scores{[&] {
    auto scores{std::vector<int64_t>{}};
    scores.reserve(size(data));
    transform(cbegin(data), cend(data), back_inserter(scores),
              [](std::string line) -> int64_t {
                auto stack{std::vector<char>{}};
                for (char c : line) {
                  if (c == '(' || c == '[' || c == '{' || c == '<') {
                    stack.push_back(c);
                  } else {
                    if (c == ')' && stack.back() != '(') {
                      return -3;
                    } else if (c == ']' && stack.back() != '[') {
                      return -57;
                    } else if (c == '}' && stack.back() != '{') {
                      return -1197;
                    } else if (c == '>' && stack.back() != '<') {
                      return -25137;
                    }
                    stack.pop_back();
                  }
                }
                return std::accumulate(crbegin(stack), crend(stack), int64_t{0},
                                       [](std::integral auto acc, char c) {
                                         return acc * 5 + [](char c) {
                                           switch (c) {
                                           case '(':
                                             return 1;
                                           case '[':
                                             return 2;
                                           case '{':
                                             return 3;
                                           case '<':
                                             return 4;
                                           }
                                           return 0; // will not reach
                                         }(c);
                                       });
              });
    return scores;
  }()};

  auto const incorrectScore{std::transform_reduce(
      cbegin(scores), cend(scores), 0, std::plus<>{},
      [](std::integral auto val) { return val < 0 ? -val : 0; })};

  std::cout << "Incorrect score: " << incorrectScore << '\n';

  auto const incompleteScores{[&] {
    auto incompleteScores{std::vector<int64_t>{}};
    incompleteScores.reserve(
        size(scores)); // could filter all positive numbers, but whatever.
    copy_if(cbegin(scores), cend(scores), back_inserter(incompleteScores),
            [](std::integral auto score) { return score > 0; });
    return incompleteScores;
  }()};
  std::cout << "Nr incomplete scores: " << size(incompleteScores) << '\n';

  auto const middleIndex{size(incompleteScores) / 2};

  auto const middleScore{[&] {
    auto sortedScores{std::vector<int64_t>(middleIndex + 1)};
    partial_sort_copy(cbegin(incompleteScores), cend(incompleteScores),
                      begin(sortedScores), end(sortedScores));
    return sortedScores.back();
  }()};

  std::cout << "Incomplete score: " << middleScore << std::endl;
}