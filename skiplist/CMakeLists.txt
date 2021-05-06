cmake_minimum_required(VERSION 3.10)

project(SkipList VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(skiplist SHARED skiplist.cpp skiplist.hpp)
set_target_properties(skiplist PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(skiplist PROPERTIES SOVERSION 0)
set_target_properties(skiplist PROPERTIES PUBLIC_HEADER skiplist.hpp)

add_library(skiplist_map SHARED skiplist_map.cpp skiplist_map.hpp)
set_target_properties(skiplist_map PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(skiplist_map PROPERTIES SOVERSION 0)
set_target_properties(skiplist_map PROPERTIES PUBLIC_HEADER skiplist_map.hpp)
