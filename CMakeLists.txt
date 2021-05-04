cmake_minimum_required(VERSION 3.10)

project(SkipListExamples VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# for editor support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(include_dirs
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/skiplist"
    )

add_executable(tester examples/tester.cpp)
add_executable(dictionary examples/dictionary.cpp)
add_executable(visualizer examples/visualizer.cpp)
add_executable(benchmarks examples/benchmarks.cpp)
add_executable(iterator_test examples/iterator_test.cpp)

target_link_libraries(tester PUBLIC skiplist)
target_link_libraries(dictionary PUBLIC skiplist)
target_link_libraries(visualizer PUBLIC skiplist)
target_link_libraries(benchmarks PUBLIC skiplist)
target_link_libraries(iterator_test PUBLIC skiplist)

target_include_directories(tester PUBLIC ${include_dirs})
target_include_directories(dictionary PUBLIC ${include_dirs})
target_include_directories(visualizer PUBLIC ${include_dirs})
target_include_directories(benchmarks PUBLIC ${include_dirs})
target_include_directories(iterator_test PUBLIC ${include_dirs})

add_subdirectory(skiplist)
