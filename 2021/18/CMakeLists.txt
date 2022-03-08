cmake_minimum_required(VERSION 3.10)

# set the project name
project(AoC18)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(AoC18 main.cpp)
target_sources(AoC18
    PRIVATE
        SnailFishNumberGenerator.cpp
        SnailFishNumberArithmetic.cpp
        SnailFishNumberPrint.cpp
)
