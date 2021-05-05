#include <iostream>
#include <set>
#include <skiplist.hpp>
#include <vector>

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

template<typename T>
void display(T first, T last) {
    while(first != last) {
        cout << *first << "\t";
        first++;
    }
    cout << "\n";
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

    cout << "Inserting into the skip list:\n";
    for(int i=0; i<10; i++)
    {
        iskip.insert(i%5);
        cout << i%5 << " ";
    }
    cout << "\n";

    iskip.erase(0);
    iskip.erase(0);
    iskip.erase(2);

    foundit(iskip, 2);
    foundit(iskip, 7);

	std::vector<int> v = {10, 4, 3, 6, 1};
	skiplist<int> skiplistFromVector(begin(v), end(v));
	cout << "Skiplist constructed from vector:\n";
	display(begin(skiplistFromVector), end(skiplistFromVector));

	skiplist<double> slFromInitializer = {1.1, 0.5, 100.3};
	cout << "Skiplist constructed using initialzer_list:\n";
	display(begin(slFromInitializer), end(slFromInitializer));
    cout << "No runtime exceptions. YAY!\n";
}
