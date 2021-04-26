cmake_minimum_required(VERSION 3.10)

project(SkipList VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(skiplist skiplist.cpp skiplist.hpp)
