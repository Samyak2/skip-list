#include <iostream>
#include <set>
#include <skiplist.hpp>

// standard
using std::begin;
using std::end;

// io ops
using std::cout;
using std::endl;

void foundit(skiplist<int> &s, int x) {
    skiplist<int>::iterator itr = s.find(x);
    if(itr != end(s))
        cout << "Value: " << itr.node->val << " Count: " << itr.node->count << endl;
    else
        cout << "Not found: " << x << "\n";
}

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

    cout << "The mulitset has:\n";
    for(auto e: iset) {
        cout << e << " ";
    }
    cout << "\n";

    // skip list operations, must mirror the multiset operations
    skiplist<int> iskip;
    
    for(int i=0; i<10; i++)
        iskip.insert(i%5);
    
    iskip.erase(0);
    iskip.erase(0);
    iskip.erase(2);
    
    foundit(iskip, 2);
    foundit(iskip, 7);

    // need to make operator++, operator* for the iterator for this one
    // maybe instead of following the multiset method we print out (key, count)?
    // cout << "The skiplist has:\n";
    // for(auto e: iskip) {
    //     cout << e << " ";
    // }
    // cout << "\n";

    cout << "No runtime exceptions. YAY!\n";
}
