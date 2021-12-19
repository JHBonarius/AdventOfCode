#include <fstream>
#include <vector>
#include <numeric>
#include <iostream>

struct command {
    std::string direction;
    int amount;
};

std::istream& operator>> (std::istream& is, command& cmd)
{
    is >> cmd.direction >> cmd.amount;
    return is;
}

template<typename T>
auto readinputdata()
{
    std::vector<T> data;
    std::ifstream fs("input");
    T val;
    while (fs >> val)
    {
        data.push_back(val);
    }
    return data;
}

struct location{
    int distance;
    int depth;
    int aim;
};

int main()
{
    auto const data = readinputdata<command>();

    auto const result = std::accumulate(
        std::cbegin(data),
        std::cend(data),
        location{0,0,0},
        [](location current, command cmd){
    //std::cout << "command: " << cmd.direction << " " << cmd.amount << '\n';
            if (cmd.direction == "forward"){
                current.distance += cmd.amount;
                current.depth += current.aim * cmd.amount;
            }
            if (cmd.direction == "down") {
                current.aim += cmd.amount;
            }
            if (cmd.direction == "up"){
                current.aim -= cmd.amount;
            }
    //std::cout << "distance: " << current.distance << ", depth: " << current.depth << '\n';
            return current;
        }
    );

    std::cout << "distance: " << result.distance << ", depth: " << result.depth << '\n';
    std::cout << "multiplied: " << result.distance * result.depth << '\n';
}