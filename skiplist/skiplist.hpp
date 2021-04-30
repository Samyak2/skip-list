/* 
Skip list implementation
Only the skiplist container should be visible
*/
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>
using std::vector;

// for debugging
#include <iostream>
using std::cout;

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
    { this->count = 0; }
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
        SLNode<value_type> *node;

        bool operator==(const iterator &rhs) {return node==rhs.node;}
        bool operator!=(const iterator &rhs) {return !(*this==rhs);}
    };
    
    skiplist() {};
    // TODO: Mem check with valgrind
    ~skiplist() {
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
        while(rand()/RAND_MAX > 0.5) {
            node->up = new SLNode<T>(value);
            node->up->down = node;
            node = node->up;

            keyd->up = new SLNode<T>();
            keyd->up->down = keyd;
            keyd->up = keyd;
            
            keyd->next = node;
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
    follow->next = node;
    // Probabilistically add more levels
    while(rand()/RAND_MAX > 0.5) {
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
            keyd->up = new SLNode<T>();
            keyd->up->down = keyd;
            keyd->up = keyd;
            
            keyd->next = node;
            key.push_back(keyd);
        }
    }
}

template<typename T>
// Assume that the iterator is valid
void skiplist<T>::erase(T value) {
    --size_;
}

template<typename T>
typename skiplist<T>::iterator skiplist<T>::find(T value) {
    return end();
}

#endif