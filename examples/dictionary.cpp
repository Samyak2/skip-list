#include <iostream>
#include <set>
#include <skiplist.hpp>

class DictKey {
private:
  int key_;
  std::string value_;

public:
  DictKey() : key_(0), value_("") {}
  DictKey(int key) : key_(key), value_("") {}
  DictKey(int key, std::string value) : key_(key), value_(value) {}
  friend bool operator<(const DictKey &, const DictKey &);
  friend bool operator==(const DictKey &, const DictKey &);
  friend bool operator!=(const DictKey &, const DictKey &);
  friend std::ostream &operator<<(std::ostream &out, const DictKey &o);
};

bool operator<(const DictKey &left, const DictKey &right) {
  return left.key_ < right.key_;
}
bool operator==(const DictKey &left, const DictKey &right) {
  return left.key_ == right.key_;
}
bool operator!=(const DictKey &left, const DictKey &right) {
  return left.key_ != right.key_;
}
std::ostream &operator<<(std::ostream &out, const DictKey &o) {
  return out << "<" << o.key_ << ": " << o.value_ << ">";
}

void foundit(skiplist<DictKey> &s, DictKey x) {
  skiplist<DictKey>::iterator itr = s.find(x);
  if (itr != std::end(s))
    std::cout << "Value: " << itr.node->val << " Count: " << itr.node->count
         << std::endl;
  else
    std::cout << "Not found: " << x << "\n";
}

int main() {
  skiplist<DictKey> aDict;

  aDict.insert(DictKey{10, "hello"});
  aDict.insert(DictKey{20, "jello"});
  aDict.insert(DictKey{20, "yello"});
  aDict.insert(DictKey{40, "fello"});

  // visualize(aDict);

  // foundit(aDict, DictKey{10});
  // foundit(aDict, DictKey{20});
  // foundit(aDict, DictKey{30});
  std::cout << "SKIPLIST:\n";
  for(auto e: aDict)
    std::cout << e << "\n";

  // same operations on multiset
  std::multiset<DictKey> lolz;
  lolz.insert(DictKey{10, "hello"});
  lolz.insert(DictKey{20, "jello"});
  lolz.insert(DictKey{20, "yello"});
  lolz.insert(DictKey{40, "fello"});

  std::cout << "MULTISET:\n";
  for(auto e: lolz)
    std::cout << e << "\n";

  // what about a find?
  // returns the first element inserted apparently
  std::cout << "MULTISET:\n";
  auto it = lolz.find(DictKey{20, "yomama"});
  std::cout << "Found: " << *it << "\n";
  std::cout << "Next: " << *++it << "\n";
  std::cout << "Count: " << lolz.count(DictKey{20, "yomama"}) << "\n";

  // now to repeat it with our data structure
  std::cout << "SKIPLIST:\n";
  auto iter = aDict.find(DictKey{20, "yomama"});
  std::cout << "Found: " << *iter << "\n";
  std::cout << "Next: " << *++iter << "\n";
  std::cout << "Count: " << aDict.count(DictKey{20, "yomama"}) << "\n";

  return 0;
}
