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

    // indexing
    std::cout << "At index 0: " << iskip[0] << std::endl;
    std::cout << "At index 1: " << iskip[1] << std::endl;
    std::cout << "At index 5: " << iskip[5] << std::endl;

    for(int i=0; i<10; i++)
        iskip.insert(i);

    visualize<int>(iskip);

    visualize_width(iskip);

    // indexing
    std::cout << "At index 0: " << iskip[0] << std::endl;
    std::cout << "At index 1: " << iskip[1] << std::endl;
    std::cout << "At index 5: " << iskip[5] << std::endl;
}
