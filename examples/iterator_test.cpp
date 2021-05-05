#include <iostream>
#include <skiplist.hpp>

template<typename T>
void display(T first, T last) {
    while(first != last) {
        std::cout << *first << " ";
        ++first;
    }
    std::cout << "\n";
}

int main() {
    skiplist<int> iskip;
    for(int i=0; i<21; i++)
    {
        if(i%2)
            iskip.insert(i%7);
        else
            iskip.insert(i+2);
    }
    std::cout << iskip;

    std::cout << "Forward iterator :\n";
    display(iskip.begin(), iskip.end());

    std::cout << "Reverse iterator:\n";
    display(iskip.rbegin(), iskip.rend());

    std::cout << "Erasing with iterators:\n";
    auto it = iskip.find(3);
    if(it != std::end(iskip)) {
        iskip.erase(it);
        std::cout << "Erased (I think?) \n";
    }
    else
        std::cout << "Something very wrong\n";

    std::cout << "Forward constant iterator :\n";
    display(iskip.cbegin(), iskip.cend());

    std::cout << "Reverse constant iterator:\n";
    display(iskip.crbegin(), iskip.crend());

    std::cout << "\nENDL\n\n";
    std::cout << "Creating a new skiplist with the copy constructor:\n";

    skiplist<int> uskip(iskip);
    std::cout << "Reverse constant iterator\n";
    display(uskip.crbegin(), uskip.crend());
    std::cout << "Deleting elements - 0 20 22\n";
    uskip.erase(20);
    uskip.erase(0);
    uskip.erase(22);
    std::cout << "Forward iterator :\n";
    display(uskip.begin(), uskip.end());

    std::cout << "\nENDL\n\n";
    std::cout << "Creating a new skiplist with the copy assignment operator:\n";

    skiplist<int> yskip = uskip;
    std::cout << "Forward iterator :\n";
    display(yskip.begin(), yskip.end());
}
