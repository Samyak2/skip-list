#include <iostream>
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
    cout << "Value: " << itr.node->val << " Count: " << itr.node->count
         << std::endl;
  else
    cout << "Not found: " << x << "\n";
}

int main() {
  skiplist<DictKey> aDict;

  aDict.insert(DictKey{10, "hello"});
  aDict.insert(DictKey{20, "jello"});

  foundit(aDict, DictKey{10});
  foundit(aDict, DictKey{20});
  foundit(aDict, DictKey{30});

  return 0;
}
