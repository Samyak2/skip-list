#include <iostream>
#include <skiplist.hpp>

template<typename T>
void display(T first, T last) {
    while(first != last) {
        cout << *first << " ";
        ++first;
    }
    cout << "\n";
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
    visualize(iskip);

    cout << "Forward iterator :\n";
    display(iskip.begin(), iskip.end());

    cout << "Reverse iterator:\n";
    display(iskip.rbegin(), iskip.rend());

    cout << "Forward constant iterator :\n";
    display(iskip.cbegin(), iskip.cend());

    cout << "Reverse constant iterator:\n";
    display(iskip.crbegin(), iskip.crend());
}
