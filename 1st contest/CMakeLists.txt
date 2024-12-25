cmake_minimum_required(VERSION 3.28)

project(aisd_contest_1)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -lm -std=c++20")

add_executable(test H.cpp)