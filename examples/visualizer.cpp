#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <iostream>
#include <skiplist.hpp>
#include <map>

int main() {
    std::srand(time(0));

    skiplist<int> iskip;
    for(int i=0; i<30; i++)
        iskip.insert(i);

    std::cout << iskip;
}
