#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <iostream>
#include <skiplist.hpp>
#include <map>

int main() {
    std::srand(time(0));

    skiplist<int> iskip;
    for(int i=0; i<10; i++)
        iskip.insert(i);

    visualize_width(iskip);

    for(int i=0; i<10; i++)
        iskip.insert(i);

    visualize<int>(iskip);

    visualize_width(iskip);
}
