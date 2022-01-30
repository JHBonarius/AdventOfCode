#include <algorithm>
#include <array>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static constexpr auto N = 5;
static constexpr auto N2 = N * N;

struct Board {
  std::array<int, N2> numbers{};
  std::bitset<N2> marked{};
};

void Print(Board const &board) {
  auto i{0};
  for (auto row{0}; row < N; ++row) {
    for (auto col{0}; col < N; ++col) {
      std::cout << std::setw(2) << board.numbers[i]
                << (board.marked[i] ? '*' : ' ') << ' ';
      ++i;
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

void State(std::vector<Board> const &boards) {
  std::for_each(cbegin(boards), cend(boards), Print);
}

std::istream &operator>>(std::istream &is, Board &board) {
  std::copy_n(std::istream_iterator<int>{is}, N2, begin(board.numbers));
  return is;
}

static constexpr auto winningBoards = []() {
  std::array<std::bitset<N2>, 2 * N> winningBoards{};
  auto i{0};
  auto val{(1ull << N) - 1};
  for (auto row{0}; row < N; ++row) {
    // auto &winningBoard = winningBoards[i++];
    // for (auto col{0}; col < N; ++col) {
    //   winningBoard[row * N + col] = true;
    // }
    winningBoards[i++] = std::bitset<N2>{val};
    val <<= N;
  }
  for (auto col{0}; col < N; ++col) {
    auto val{1ull << col};
    for (auto row{0}; row < N; ++row) {
      val = (val << N) + (1 << col);
    }
    winningBoards[i++] = std::bitset<N2>{val};
  }
  return winningBoards;
}();

bool IsWinning(Board const &board) {
  // return std::find(cbegin(winningBoards), cend(winningBoards), board.marked)
  // !=
  //        cend(winningBoards);
  return std::any_of(cbegin(winningBoards), cend(winningBoards),
                     [&](auto winningBoard) {
                       return (board.marked & winningBoard) == winningBoard;
                     });
}

auto Score(Board const &board) {
  // return std::transform_reduce(
  //     cbegin(board.marked), cend(board.marked), cbegin(board.numbers), 0,
  //     std::plus<>{},
  //     [](bool marked, int number) { return marked ? 0 : number; });
  auto acc{0};
  for (auto i{0}; i < N2; ++i) {
    if (!board.marked[i]) {
      acc += board.numbers[i];
    }
  }
  return acc;
}

auto FindWinningBoard(std::vector<Board> &boards,
                      std::vector<int> const &numbers) {
  std::vector<int> winningScores{};

  for (auto number : numbers) {
    std::cout << "number: " << number << "\n\n";
    std::for_each(begin(boards), end(boards), [number](Board &board) {
      auto const it =
          std::find(cbegin(board.numbers), cend(board.numbers), number);
      if (it != cend(board.numbers)) {
        board.marked[std::distance(cbegin(board.numbers), it)] = true;
      }
    });

    auto const winIt =
        std::find_if(cbegin(boards), cend(boards),
                     [](Board const &board) { return IsWinning(board); });
    if (winIt != cend(boards)) {
      winningScores.push_back(Score(*winIt) * number);
      // boards.erase(winIt);
      // } else {
      //   // boards.erase(winIt);
      std::cout << ", erased: "
                << std::erase_if(
                       boards,
                       [](Board const &board) { return IsWinning(board); })
                << ' ';
    }
    if (boards.size() == 0) break;
    //    State(boards);
    std::cout << '\n';
  }
  return winningScores.back();
}

int main() {
  std::vector<int> numbers{};
  std::vector<Board> boards{};
  {
    std::ifstream fs("input");
    std::string numberLine;
    fs >> numberLine;
    std::stringstream numberLineSs{numberLine};
    for (int i; numberLineSs >> i;) {
      numbers.push_back(i);
      if (numberLineSs.peek() == ',') numberLineSs.ignore();
    }

    Board val{};
    while (fs >> val) {
      boards.push_back(val);
    }
  }

  // numbers =
  //     std::vector<int>{7,  4, 9,  5,  11, 17, 23, 2,  0, 14, 21, 24, 10, 16,
  //                      13, 6, 15, 25, 12, 22, 18, 20, 8, 19, 3,  26, 1};
  // boards = std::vector<Board>{
  //     Board{{22, 13, 17, 11, 0, 8,  2, 23, 4,  24, 21, 9, 14,
  //            16, 7,  6,  10, 3, 18, 5, 1,  12, 20, 15, 19},
  //           {}},
  //     Board{{3,  15, 0,  2,  22, 9,  18, 13, 17, 5,  19, 8, 7,
  //            25, 23, 20, 11, 10, 24, 4,  14, 21, 16, 12, 6},
  //           {}},
  //     Board{{14, 21, 17, 24, 4,  10, 16, 15, 9, 19, 18, 8, 23,
  //            26, 20, 22, 11, 13, 6,  5,  2,  0, 12, 3,  7},
  //           {}}};

  std::cout << "Number of numbers: " << numbers.size() << '\n';
  std::cout << "Number of boards: " << boards.size() << '\n';
  Print(boards[0]);
  std::cout << "score of board 0: " << Score(boards[0]) << '\n';

  auto winningBoardScore = FindWinningBoard(boards, numbers);
  std::cout << "score of the winning board: " << winningBoardScore << '\n';
}