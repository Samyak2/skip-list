#include <iostream>
#include <map>
#include <skiplist_map.hpp>

int main() {
  skiplist<int, std::string> aDict;

  aDict.insert(10, "hello");
  aDict.insert(20, "jello");
  aDict.insert(20, "yello");
  aDict.insert(40, "fello");

  std::cout << aDict;

  // foundit(aDict, DictKey{10});
  // foundit(aDict, DictKey{20});
  // foundit(aDict, DictKey{30});
  std::cout << "SKIPLIST:\n";
  for(auto e: aDict)
    std::cout << e << "\n";

  // same operations on multiset
  std::multimap<int, std::string> lolz;
  lolz.insert({10, "hello"});
  lolz.insert({20, "jello"});
  lolz.insert({20, "yello"});
  lolz.insert({40, "fello"});

  std::cout << "MULTISET:\n";
  for(auto e: lolz)
    std::cout << e.first << ":" << e.second << "\n";

  // what about a find?
  // returns the first element inserted apparently
  std::cout << "MULTISET:\n";
  auto it = lolz.find(20);
  std::cout << "Found: " << it->first << ":" << it->second << "\n";
  auto next = ++it;
  std::cout << "Next: " << next->first << ":" << next->second << "\n";
  std::cout << "Count: " << lolz.count(20) << "\n";

  // now to repeat it with our data structure
  std::cout << "SKIPLIST:\n";
  auto iter = aDict.find(20);
  std::cout << "Found: " << *iter << "\n";
  std::cout << "Next: " << *++iter << "\n";
  std::cout << "Count: " << aDict.count(20) << "\n";

  return 0;
}
