#include <iostream>
#include <set>
#include <skiplist.hpp>

// io ops
using std::cout;
using std::endl;

void foundit(skiplist<int> &s, int x) {
    skiplist<int>::iterator itr = s.find(x);
    if(itr != s.end())
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

    // skip list operations
    skiplist<int> iskip;
    for(int i=0; i<10; i++)
        iskip.insert(i%5);
    iskip.erase(1);
    iskip.erase(1);
    iskip.erase(2);
    
    foundit(iskip, 0);
    foundit(iskip, 1);
    foundit(iskip, 2);
    foundit(iskip, 7);

    iskip.insert(-1);
    iskip.insert(99);
    cout << "The first element is: " << iskip.begin().node->val << endl;

    cout << "No runtime errors YAY!\n";
}
