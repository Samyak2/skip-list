/* 
Skip list implementation
Only the skiplist container should be visible
*/
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
using std::vector;

// for debugging
#include <iostream>
using std::cout;
using std::endl;

template<typename T>
class skiplist;

template<typename T>
void visualize(const skiplist<T>&);

template<typename T>
struct SLNode {
    // Poorly named - left, right, up, down pointers
    SLNode *back, *next, *up, *down;
    // Value, should be templated
    T val;
    // Count is integer only
    int count;
    SLNode(T val_=T())
    : back(nullptr), next(nullptr), 
      up(nullptr), down(nullptr), val(val_) 
    { count = 1; }
};

template<typename value_type>
class skiplist {
    private:
    // header nodes for all levels
    vector<SLNode<value_type>*> key;
    int size_;

    public:
    // Iterator always points to a level 0 node
    // or a nullptr for end()
    class iterator {
        public:
        // To increment or decrement this iterator, just change node
        SLNode<value_type> *node;
        iterator(SLNode<value_type> *node_) : node(node_) {}

        bool operator==(const iterator &rhs) {return node==rhs.node;}
        bool operator!=(const iterator &rhs) {return !(*this==rhs);}
    };
    
    skiplist() : size_(0) {};
    // TODO: Mem check with valgrind
    ~skiplist() {
        // Terribly written destructor
        // At every level, go on till nullptr and delete everything in its path
        SLNode<value_type> *tmp;
        for(auto level: key) {
            while(level) {
                tmp = level->next;
                delete level;
                level = tmp;
            }
        }
    }
    int size() {return size_;};
    
    iterator begin() {
        if(key.empty())
            return end();
        else
            return iterator(key[0]->next);
    }
    iterator end() {return iterator(nullptr);};

    // specialized functions
    void insert(value_type value);
    // erase can be overloaded
    // this version finds the value and deletes the node if it exists
    // one more version of erase is passing an iterator object
    void erase(value_type value);
    iterator find(value_type value);
    friend void visualize<value_type>(const skiplist<value_type>&);
};


/*
================================================================================
=================== NOTE! THE PART BELOW IS FROM THE CPP FILE ==================
================================================================================
*/
#include <cstdlib>

template<typename T>
// Inserting same value just increments count
// Insertion always starts at level 0
void skiplist<T>::insert(T value) {
    ++size_;

    // If entire container empty
    if(key.empty()) {
        SLNode<T> *node = new SLNode<T>(value);
        SLNode<T> *keyd = new SLNode<T>();
        key.push_back(keyd);
        key[0]->next = node;
        node->back = key[0];
        // Now insert some more, probabilistically
        while((double)rand()/RAND_MAX > 0.5) {
            node->up = new SLNode<T>(value);
            node->up->down = node;
            node = node->up;

            keyd->up = new SLNode<T>();
            keyd->up->down = keyd;
            keyd = keyd->up;
            
            keyd->next = node;
            node->back = keyd;
            key.push_back(keyd);
        }
        return;
    }
    // Else
    // Find the correct position, insert
    // If not exists, create upper levels for that element
    
    // This is the prev nodes for all levels
    vector<SLNode<T>*> history;
    // Search always starts from the upper left node
    SLNode<T>* follow = key.back();
    // Go on till level 0
    while(follow->down) {
        while(follow->next && follow->next->val < value)
            follow = follow->next;
        history.push_back(follow);
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && follow->next->val < value)
        follow = follow->next;
    
    // If node already exists, just increment its count
    if(follow->next && follow->next->val == value) {
        follow = follow->next;
        while(follow) {
            follow->count++;
            follow = follow->up;
        }
        return;
    }

    // Value does not exist. Insert a new node
    SLNode<T> *node = new SLNode<T>(value), *keyd = key.back();
    node->next = follow->next;
    node->back = follow;
    if(follow->next)
        follow->next->back = node;
    follow->next = node;
    // Probabilistically add more levels
    while((double)rand()/RAND_MAX > 0.5) {
        node->up = new SLNode<T>(value);
        node->up->down = node;
        node = node->up;
        if(!history.empty()) {
            node->next = history.back()->next;
            node->back = history.back();
            history.back()->next = node;
            history.pop_back();
        }
        else {
            // add directly to the key
            // mess around with the pointers
            keyd->up = new SLNode<T>();
            keyd->up->down = keyd;
            keyd = keyd->up;
            
            keyd->next = node;
            node->back = keyd;
            key.push_back(keyd);
        }
    }
}

template<typename T>
// Assume that the iterator is valid
// But cannot assume element exists
void skiplist<T>::erase(T value) {
    if(key.empty())
        return;
    // Find value
    // Decrement its counter
    // If counter is zero, remove it
    // If value does not exist, exit

    // Start from top left
    SLNode<T>* follow = key.back();
    // Go on till level 0
    while(follow->down) {
        while(follow->next && follow->next->val < value)
            follow = follow->next;
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && follow->next->val < value)
        follow = follow->next;
    
    // If not exist, leave
    if(!follow->next || follow->next->val != value)
        return;
    
    // This is the node for sure
    follow = follow->next;
    follow->count--;
    --size_;

    if(follow->count)
        return;
    // Remove it if count is zero
    SLNode<T> *tmp;
    // Go up all levels of that node and delete them
    while(follow) {
        follow->back->next = follow->next;
        if(follow->next)
            follow->next->back = follow->back;
        tmp = follow->up;
        delete follow;
        follow = tmp;
    }
}

template<typename T>
typename skiplist<T>::iterator skiplist<T>::find(T value) {
    // Same algorithm as erase, but without erasing anything ;)
    if(key.empty())
        return end();
    
    // Start from top left
    SLNode<T>* follow = key.back();
    // Go on till level 0
    while(follow->down) {
        while(follow->next && follow->next->val < value)
            follow = follow->next;
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && follow->next->val < value)
        follow = follow->next;
    
    // If not exist, leave
    if(!follow->next || follow->next->val != value)
        return end();
    
    // This is the node for sure
    follow = follow->next;
    return iterator(follow);
}

template<typename T>
void visualize(const skiplist<T>& sl) {

    // store mapping of node->index in a map
    std::map<T, int> index_store{};
    auto bottom_it = (*(sl.key.begin()))->next;
    int cur_index = 0;
    while (bottom_it) {
        index_store[bottom_it->val] = cur_index;
        cur_index++;
        bottom_it = bottom_it->next;
    }

    auto level = sl.key.rbegin();
    auto end = sl.key.rend();
    while(level != end) {
        // S denotes start of level, could be something different
        std::cout << "S";

        // to store previous element's index
        // and current element's index
        int index = -1, next_index;
        // skip the first element - it is always 0
        auto it = (*level)->next;
        // iterate through the list at this level
        while (it) {
            next_index = index_store[it->val];

            // filler to align it correctly
            // when elements in between don't exist at this level
            for (int i = 1; i < next_index - index; ++i) {
                std::cout << "----";
            }

            // width of printed element is hardcoded to 3 for now
            // the padding is filled with ""-"
            std::cout << "-" << std::setfill('-') << std::setw(3) << it->val;
            it = it->next;

            index = next_index;
        }
        std::cout << std::endl;

        level++;
    }
}
// End of cpp file

#endif
// End of header file
