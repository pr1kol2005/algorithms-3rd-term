cmake_minimum_required(VERSION 3.28)

project(aisd_contest_5)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++20 -fsanitize=undefined")

add_executable(test D.cpp)