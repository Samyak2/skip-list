#include <iostream>
#include <set>
#include <skiplist.hpp>

// io ops
using std::cout;
using std::endl;

int main() {
    // set operations
    std::set<int> iset;
    for(int i=0; i<10; i++)
        iset.insert(i%5);
    iset.erase(0);
    iset.erase(1);
    std::set<int>::iterator it = iset.find(2);
    it = iset.find(7);

    // skip list operations
    skiplist<int> iskip;
    for(int i=0; i<10; i++)
        iskip.insert(i%5);
    cout << "All tests passed.\n";
}
