#include <iostream>
#include "skiplist.hpp"
#include <algorithm>

int main() {
	skiplist<int> s1;
	skiplist<int> s2;

	for(int i = 0; i < 10; ++i) {
		s1.insert(i);
		s2.insert(i);
	}

	std::cout << std::boolalpha;
	std::cout << std::equal(begin(s1), end(s1), begin(s2)) << "\n";

	// for_each
	std::for_each(begin(s1), end(s1), [](int e) { std::cout << e << "\t"; });
}
