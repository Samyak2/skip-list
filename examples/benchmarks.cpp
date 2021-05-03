#include <iostream>
#include <skiplist.hpp>
#include <chrono>
#include <random>
using std::cerr;

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout <<
      "Usage: ./benchmarks size-of-list [no-of-iterations]" <<
      std::endl;
    return 1;
  }
  const int size = atoi(argv[1]);
  int iterations = 100;
  if (argc > 2) {
    iterations = atoi(argv[2]);
  }
  cout << "Size set to: " << size << endl;
  cout << "Iterations set to: " << iterations << endl;

  skiplist<int> list;
  for (int i = 0; i < size; ++i) {
    list.insert(i);
  }

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, size-1);


  // benchmark find
  std::chrono::duration<double, std::micro> min_time(std::numeric_limits<double>::max());
  std::chrono::duration<double, std::micro> total_time;
  for (int it = 0; it < iterations; ++it) {
    int to_find = distribution(generator);
    auto t1 = std::chrono::high_resolution_clock::now();
    list.find(to_find);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> time_taken_us = t2 - t1;
    total_time += time_taken_us;
    if (time_taken_us < min_time) {
      min_time = time_taken_us;
    }
  }
  cerr << "FIND: Total time: " << total_time.count() << " us\n";
  cerr << "FIND: Avg time:   " << total_time.count() / iterations << " us\n";
  cout << "Find: " << min_time.count() << " us" << std::endl;;

  // insert
  min_time = min_time.max();
  total_time -= total_time; // weird way to set it to 0
  for (int it = 0; it < iterations; ++it) {
    int to_insert = distribution(generator);

    auto t1 = std::chrono::high_resolution_clock::now();
    list.insert(to_insert);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> time_taken_us = t2 - t1;
    total_time += time_taken_us;
    if (time_taken_us < min_time) {
      min_time = time_taken_us;
    }

    list.erase(to_insert);
  }
  cerr << "INSERT: Total time: " << total_time.count() << " us\n";
  cerr << "INSERT: Avg time:   " << total_time.count() / iterations << " us\n";
  cout << "Insert: " << min_time.count() << " us" << std::endl;

  // erase
  min_time = min_time.max();
  total_time -= total_time; // weird way to set it to 0
  for (int it = 0; it < iterations; ++it) {
    int to_insert = distribution(generator);

    list.insert(to_insert);

    auto t1 = std::chrono::high_resolution_clock::now();
    list.erase(to_insert);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> time_taken_us = t2 - t1;
    total_time += time_taken_us;
    if (time_taken_us < min_time) {
      min_time = time_taken_us;
    }
  }
  cerr << "ERASE: Total time: " << total_time.count() << " us\n";
  cerr << "ERASE: Avg time:   " << total_time.count() / iterations << " us\n";
  cout << "Erase: " << min_time.count() << " us" << std::endl;;
}
