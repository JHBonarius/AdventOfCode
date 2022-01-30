#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include "../readinputdata.hpp"

struct Range {
  int start{}, end{};
};
struct Area {
  Range x{}, y{};
};

static constexpr auto testData{Area{{20, 30}, {-5, -10}}};
static constexpr auto liveData{Area{{269, 292}, {-44, -68}}};

struct Position {
  int x{}, y{};
};
struct Velocity {
  int x{}, y{};
};
struct Probe {
  Position position{0, 0};  // explicit starting point
  Velocity velocity{};

  constexpr void DeStep() noexcept {
    position.x += velocity.x;
    position.y += velocity.y;
    velocity.x = std::max(0, velocity.x - 1);
    velocity.y -= 1;
  }
};

// 1+2+3+4+5 = (5*6)/2 = 15
constexpr auto triangular_number(std::integral auto n) {
  return (n * (n + 1)) / 2;
}

auto inverse_triangular_number(std::integral auto n) {
  // quadratic formula (half of it)
  // a = 0.5, b = 0.5, c = -n
  return (-0.5 + std::sqrt(0.25 + 2.0 * n));
}

int main() {
  auto const targetArea{
      // testData //
      liveData  //
  };
  // part one is very simple
  std::cout << "Max height: " << triangular_number(-targetArea.y.end - 1)
            << '\n';

  auto nrOfPaths{0};
  // 1 step
  // nrOfPaths += (targetArea.x.end - targetArea.x.start + 1) *
  //              (targetArea.y.end - targetArea.y.start + 1);
  // // dead-drop (vx = 0 eventually)
  // x velocity range for dead-drop (likely not used)
  [[maybe_unused]] auto const vx_deaddrop_min{static_cast<int>(
      std::ceil(inverse_triangular_number(targetArea.x.start)))};
  [[maybe_unused]] auto const vx_deaddrop_max{static_cast<int>(
      std::floor(inverse_triangular_number(targetArea.x.end)))};
  // std::cout << vx_deaddrop_min << ".." << vx_deaddrop_max << '\n';

  // nrOfPaths += (vx_deaddrop_max - vx_deaddrop_min + 1) *
  //              (targetArea.y.end - targetArea.y.start + 1);
  // f*** it: brute force
  for (auto vx{vx_deaddrop_min}; vx <= targetArea.x.end; ++vx) {
    for (auto vy{targetArea.y.end}; vy < -targetArea.y.end; ++vy) {
      auto probe{Probe{0, 0, vx, vy}};
      while (probe.position.x < targetArea.x.start ||
             probe.position.y > targetArea.y.start) {
        probe.DeStep();
      }
      if (probe.position.x <= targetArea.x.end &&
          probe.position.y >= targetArea.y.end) {
        ++nrOfPaths;
      }
    }
  }

  std::cout << "Number of paths: " << nrOfPaths << '\n';
  return EXIT_SUCCESS;
}