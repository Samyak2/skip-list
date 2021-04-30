#include <iostream>
#include <set>
#include <skiplist.hpp>

// io ops
using std::cout;
using std::endl;

int main() {
    // set operations
    std::multiset<int> iset;
    for(int i=0; i<10; i++)
        iset.insert(i%5);
    iset.erase(0);
    iset.erase(0);
    iset.erase(1);
    std::multiset<int>::iterator it = iset.find(2);
    it = iset.find(7);

    // skip list operations
    skiplist<int> iskip;
    for(int i=0; i<10; i++)
        iskip.insert(i%5);
    iskip.erase(0);
    iskip.erase(0);
    iskip.erase(1);
    cout << "All tests passed.\n";
}
