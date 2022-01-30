#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  auto const data = []() {
    std::vector<int> data;
    std::ifstream fs("input");
    int num;
    while (fs >> num) {
      data.push_back(num);
    }
    return data;
  }();

  auto const newdata = [&]() {
    std::vector<int> newdata(data.size() - 2);
    auto it1 = data.cbegin(), it2 = std::next(it1), it3 = std::next(it2);
    auto ot = newdata.begin();
    while (it3 != data.cend()) {
      *ot++ = *it1++ + *it2++ + *it3++;
    }
    return newdata;
  }();

  auto const result = std::transform_reduce(
      std::next(std::cbegin(newdata)), std::cend(newdata), std::cbegin(newdata),
      0, std::plus<>{}, std::greater<>{}
      //[](int second, int first){ return first < second ? 1 : 0; }
  );

  std::cout << "total is " << result << '\n';
}