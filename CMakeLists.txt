cmake_minimum_required(VERSION 3.10)

project(SkipListExamples VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# for editor support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tester examples/tester.cpp)

target_link_libraries(tester PUBLIC skiplist)

target_include_directories(tester PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/skiplist"
                           )
add_subdirectory(skiplist)
