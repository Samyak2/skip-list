/*
Skip list implementation
Only the skiplist container should be visible
*/
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <iterator>
#include <initializer_list>

#include <iomanip>
#include <iostream>

template<
    typename val_type,
    typename compare_t = std::less<val_type>
>
class skiplist;

template<
    typename val_type,
    typename compare_t
>
std::ostream &operator<<(std::ostream &out, const skiplist<val_type, compare_t>&);

template<typename T>
struct SLNode {
    // Poorly named - left, right, up, down pointers
    SLNode *back, *next, *up, *down;
    // Value, should be templated
    T val;
    // Storage for multiple elements
    std::vector<T> valz;
    // Count is integer only, will only be 0 for key nodes
    int count;
    SLNode(T val_)
    : back(nullptr), next(nullptr), up(nullptr), down(nullptr), val(val_), count(1) {}
    // TODO: not nice, T must have default constructor, must figure a workaround
    SLNode()
    : back(nullptr), next(nullptr), up(nullptr), down(nullptr), count(0) {}
    
    // Special copy constructor
    SLNode(SLNode other, int)
    : back(nullptr), next(nullptr), up(nullptr), down(nullptr), valz(other.valz) {
        val = other.val;
        count = other.count;
    }
};

template<
    typename val_type,
    typename compare_t
>
class skiplist {
private:
    // header nodes for all levels
    std::vector<SLNode<val_type>*> key;
    // number of nodes in the skiplist (including non-unique ones)
    int size_;
    // the last node at level 0
    SLNode<val_type>* last;

    // stuff required for random number generation
    // see constructor for description/reference
    std::mt19937_64 mt_;
    std::uniform_real_distribution<double> dist_;

    // template objects, since compare is supposed to be a functor
    compare_t compare;

    // setup random number generator and uniform
    // distribution to help with probabilistic
    // insertion.
    void _setup_random_number_generator() {
        // set up random number generator
        // ref: https://stackoverflow.com/a/19666713/11199009
        std::random_device rd;
        // ref: https://www.cplusplus.com/reference/random/mt19937_64/
        std::mt19937_64 mt(rd());
        // usage of this is: dist(mt)
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        this->mt_ = mt;
        this->dist_ = dist;
    }

public:
    // one mega iterator
    // because... everything is cake?
    template<bool reversal = false>
    class cake_iterator;

    // all the other iterators
    using const_iterator = cake_iterator<>;
    using iterator = const_iterator;
    using const_reverse_iterator = cake_iterator<true>;
    using reverse_iterator = const_reverse_iterator;

    skiplist() : size_(0), last(nullptr) {_setup_random_number_generator();}

    // iterator range is assumed to be valid.
    // can we validate range? no need, screw the user :)
    template<typename InputIterator>
    skiplist(InputIterator first, InputIterator last): size_(0), last(nullptr) {
        _setup_random_number_generator();
        // last and size_ are taken care of during insertion.
        while(first != last) {
            insert(*first);
            ++first;
        }
    }

    skiplist(std::initializer_list<val_type> l) : size_(0), last(nullptr) {
        _setup_random_number_generator();
        auto first = l.begin();
        auto last = l.end();
        while(first != last) {
            insert(*first);
            ++first;
        }
    }

    // At every level, go on till nullptr and delete everything in its path
    // then go on to the upper level
    void destroy_all_levels() {
        SLNode<val_type> *tmp;
        for(auto level: key)
            while(level) {
                tmp = level->next;
                delete level;
                level = tmp;
            }
        key.clear();
    }
    
    ~skiplist() { destroy_all_levels(); }

    // move constructor
    skiplist(skiplist &&other) 
    : key(other.key), size_(other.size_), last(other.last) {
        _setup_random_number_generator();
        // thief! thief! resources gon :(
        other.key.clear();
    }

    // move assignment. Apparently, this cannot be a friend :(
    skiplist& operator=(skiplist &&rhs) {
        // sanity check
        if(this == &rhs)
        // what have you bought upon this cursed land?
            return *this;

        // Ruthlessly STEAAAAL
        destroy_all_levels();
        key = rhs.key;
        size_ = rhs.size_;
        last = rhs.last;

        rhs.key.clear();
        rhs.size_ = 0;
        rhs.last = nullptr;

        return *this;
    }

    // copy a key structure from one node to another
    // O(n) time & O(n) space algorithm 
    void perform_key_transfer(const skiplist &other) {
        if(other.key.empty())
            return;
        
        // map node pointers through s p a c e. has info about ...
        // which node of L (i.e LHS) is holding the node from R (i.e. RHS)
        // useful to map horizontal pointers between two levels
        std::unordered_map<SLNode<val_type>*, SLNode<val_type>*> twister, jenga;
        SLNode<val_type> *trav_r, *trav_l;
        // handy, because all level 0 nodes have a down pointer to nullptr
        jenga[nullptr] = nullptr;

        // map level 0
        // now map the higher levels
        // oh chuck, just map them all
        for(int i=0; i<other.key.size(); i++) {
            
            trav_r = other.key[i]->next;
            trav_l = new SLNode<val_type>();
            key.push_back(trav_l);
            while(trav_r) {
                trav_l->next = new SLNode<val_type>(*trav_r, 15081947);
                trav_l->next->back = trav_l;
                trav_l = trav_l->next;
                
                twister[trav_r] = trav_l;
                // my favourite exit strategy :)
                if(jenga.find(trav_r->down)==jenga.end()) {
                    std::cout << "Should never see here! In Copy constructor\n";
                    std::cout.flush();
                    int *x(nullptr);
                    *x = *x;
                }
                // continue with normal execution
                trav_l->down = jenga[trav_r->down];
                
                if(trav_l->down)
                    trav_l->down->up = trav_l;
                
                trav_r = trav_r->next;
            }
            // special case for level 0. need to get the last node pointer
            if(i==0)
                last = twister[other.last];

            // move the current level pointers to the prev-level-pointer store
            jenga = twister;
            twister.clear();
        }
        // properly interleave the up down nodes of the key stack
        for(int i=1; i<key.size(); i++) {
            key[i]->down = key[i-1];
            key[i-1]->up = key[i];
        }
    }

    // copy constructor
    skiplist(const skiplist &other) 
    : size_(other.size_) {
        _setup_random_number_generator();
        perform_key_transfer(other);
    }

    // copy assignment operator
    skiplist& operator=(const skiplist &rhs) {
        // sanity check
        if(this == &rhs)
        // what have you bought upon this cursed land?
            return *this;

        // name's cat ... *copy* cat :|
        destroy_all_levels();
        perform_key_transfer(rhs);
        size_ = rhs.size_;
        last = rhs.last;

        return *this;
    }

    // specialized functions
    void insert(val_type value);

    // erase can be overloaded
    // this version finds the value and deletes the node if it exists
    // one more version of erase is passing an iterator object
    void erase(val_type value);
    iterator erase(iterator it);

    iterator find(val_type value);
    // smol count function to match set interface

    int count(val_type value) {
        auto it = find(value);
        return  it != end() ? it.node->count : 0;
    }
    friend std::ostream &operator<<<val_type, compare_t>(std::ostream &out, const skiplist<val_type, compare_t>& sl);
    int size() { return size_;}

    // forward iterator to begin
    iterator begin() { return key.empty() ? end() : iterator(key[0]->next); }
    // forward iterator to one beyond last.
    iterator end() { return iterator(nullptr); };
    // reverse iterator pointing to last element
    reverse_iterator rbegin() { return reverse_iterator(last);}
    // sneak trick -> if the list is empty, last will be a nullptr
    // thus returning rbegin() is okay. it also provides good symmetry wrt begin
    reverse_iterator rend() { return key.empty() ? rbegin() : reverse_iterator(key[0]);}
    // constant forward iterator
    const_iterator cbegin() { return key.empty() ? cend() : const_iterator(key[0]->next);}
    // constant forward iterator pointing to one beyond the last node
    const_iterator cend() { return const_iterator(nullptr); }
    // constant reverse iterator pointing to last element
    const_reverse_iterator crbegin() { return const_reverse_iterator(last); }
    // constant reverse iterator pointing to last one before the first node
    const_reverse_iterator crend() {
        return key.empty() ? crbegin() : const_reverse_iterator(key[0]); 
    }
};

// Iterator always points to a level 0 node
// or a nullptr for end()
template<
    typename val_type, 
    typename compare_t
>
template<bool reversal>
class skiplist<val_type, compare_t>::cake_iterator {
private:
    // since the skip list supports having non-unique elements with
    // the help of a count, to keep track of whether the iterator
    // should actually move front or back given non-unique elements, we make
    // use of these variables.

    // node_count_ reprsents how many counts of the current node are
    // remaining to be processed. node_count_ <= node_count_ref
    int node_count_;
    // node_count_ref_ is an unchanging count
    // of the current's node's count, used to
    // keep track of when to move the iterator
    // in the reverse direction.
    int node_count_ref_;

    // Is the iterator a reverse iterator?
    // If true, the increment operations should move to node->back instead of node->next
    // Count calculations, technically, should not be affected I think
    bool reverse_;
    // One edge case we need to take care of is a reverse ++ on the first node
    // Instead of moving to nullptr, it will move to the key vector
    // This is currently handled by rend and crend
public:
    // types
    using difference_type = std::ptrdiff_t;
    using value_type = val_type;
    using pointer = val_type*;
    using reference = val_type&;
    using iterator_category = std::bidirectional_iterator_tag;
    // To increment or decrement this iterator, just change node
    
    SLNode<val_type> *node;
    cake_iterator(SLNode<val_type> *node_) : node(node_) {
        // check iterator constructor for explanation
        node_count_ = 0;
        node_count_ref_ = 0;

        if(node_) {
            node_count_ = node_->count - 1;
            node_count_ref_ = node_count_;
        }
        reverse_ = reversal;
    }

    bool operator==(const cake_iterator &rhs) {
        return node==rhs.node && node_count_ == rhs.node_count_;
    }
    bool operator!=(const cake_iterator &rhs) { return !(*this==rhs); }
    
    const val_type& operator*() const {
        // black magic. who's gonna read this anyway?
        return node->valz[node_count_ref_ - node_count_];
    }
    // pre-increment operator
    const cake_iterator& operator++() {
        if(node_count_ != 0) {
            --node_count_;
            return *this;
        }
        if(reverse_)
            node = node->back;
        else
            node = node->next;
        if(node) {
            node_count_ = node->count - 1;
            node_count_ref_ = node_count_;
        }
        return *this;
    }
    // post-increment opreator
    cake_iterator operator++(int) {
        cake_iterator temp(node);
        ++*this;
        return temp;
    }

    // pre-decrement operator
    const cake_iterator& operator--() {
        if(node_count_ + 1 < node_count_ref_) {
            ++node_count_;
            return *this;
        }
        if(reverse_)
            node = node->next;
        else
            node = node->back;
        if(node) {
            node_count_ = node->count - 1;
            node_count_ref_ = node_count_;
        }
        return *this;
    }
    // post-decrement operator
    cake_iterator operator--(int) {
        cake_iterator temp(node);
        --*this;
        return temp;
    }
};


/*
================================================================================
=================== NOTE! THE PART BELOW IS FROM THE CPP FILE ==================
================================================================================
*/

// utility function to check equality, am lazy
// https://twitter.com/acemarke/status/1072342186396667905
template<typename T, typename op>
bool _420_is_equal(T &a, T& b, op less_than) {
    return !less_than(a, b) && !less_than(b, a);
}

template<typename T, typename X>
// Inserting same will put it in a store and increment count
// Insertion always starts at level 0
void skiplist<T, X>::insert(T value) {
    ++size_;

    // If entire container empty
    if(key.empty()) {
        SLNode<T> *node = new SLNode<T>(value);
        SLNode<T> *keyd = new SLNode<T>();
        key.push_back(keyd);
        key[0]->next = node;
        node->back = key[0];
        last = node;
        // Put the value into the store
        node->valz.push_back(value);
        // Now insert some more, probabilistically
        while(this->dist_(this->mt_) > 0.5) {
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
    std::vector<SLNode<T>*> history;
    // Search always starts from the upper left node
    SLNode<T>* follow = key.back();
    // Go on till level 0
    while(follow->down) {
        while(follow->next && compare(follow->next->val,value))
            follow = follow->next;
        history.push_back(follow);
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && compare(follow->next->val,value))
        follow = follow->next;

    // If node already exists, add the new value to the store
    if(follow->next && _420_is_equal(follow->next->val, value, compare)) {
        follow = follow->next;
        follow->count++;
        follow->valz.push_back(value);
        return;
    }

    // Value does not exist. Insert a new node
    SLNode<T> *node = new SLNode<T>(value), *keyd = key.back();
    node->next = follow->next;
    node->back = follow;
    if(follow->next)
        follow->next->back = node;
    follow->next = node;
    if(!node->next)
        last = node;
    // Add into storage
    node->valz.push_back(value);

    // Probabilistically add more levels
    while(this->dist_(this->mt_) > 0.5) {
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

template<typename T, typename X>
// Cannot assume element exists
void skiplist<T, X>::erase(T value) {
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
        while(follow->next && compare(follow->next->val, value))
            follow = follow->next;
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && compare(follow->next->val, value))
        follow = follow->next;

    // If not exist, leave
    if(!follow->next || !_420_is_equal(follow->next->val, value, compare))
        return;

    // This is the node for sure
    follow = follow->next;
    follow->valz.pop_back();
    follow->count--;
    --size_;

    if(follow->count)
        return;
    // Remove it if count is zero
    last = follow->back;
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
    // TODO : We have decided to leave the key structure unaltered
    // This means even if a level is empty, it is still preserved.
    // Need to discuss the benefits / costs of doing that

    // I don't think leaving it unaltered makes the implmentation incorrect...
    // but please check that once if you come across unexpected behaviour later on
}

template<typename T, typename X>
// Assume that iterator is valid
// After erasing, move on to the next element
typename skiplist<T, X>::iterator skiplist<T, X>::erase(typename skiplist<T, X>::iterator it) {
    SLNode<T> *follow = it.node;
    // This is the node for sure
    follow->count--;
    follow->valz.pop_back();
    --size_;

    if(follow->count)
        return it;

    // Remove it if count is zero
    last = follow->back;
    SLNode<T> *tmp, *ret(follow->next);
    // Go up all levels of that node and delete them
    while(follow) {
        follow->back->next = follow->next;
        if(follow->next)
            follow->next->back = follow->back;
        tmp = follow->up;
        delete follow;
        follow = tmp;
    }
    return iterator(ret);
    // NOTE: If there are bugs in this function due to layer management
    // check the erase function above for explanantion and fixes
}

template<typename T, typename X>
typename skiplist<T, X>::iterator skiplist<T, X>::find(T value) {
    // Same algorithm as erase, but without erasing anything ;)
    if(key.empty())
        return end();

    // Start from top left
    SLNode<T>* follow = key.back();
    // Go on till level 0
    while(follow->down) {
        while(follow->next && compare(follow->next->val, value))
            follow = follow->next;
        follow = follow->down;
    }
    // Traverse at level 0 till dest reached
    while(follow->next && compare(follow->next->val, value))
        follow = follow->next;

    // If not exist, leave
    if(!follow->next || !_420_is_equal(follow->next->val, value, compare))
        return end();

    // This is the node for sure
    follow = follow->next;
    return iterator(follow);
}

template<typename T, typename X>
std::ostream &operator<<(std::ostream &out, const skiplist<T, X>& sl) {
    if (sl.key.empty()) {
        return out << "EMPTY SKIPLIST" << std::endl;
    }

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
        out << "S";

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
                out << "----";
            }

            // width of printed element is hardcoded to 3 for now
            // the padding is filled with ""-"
            out << "-" << std::setfill('-') << std::setw(3) << it->val;
            it = it->next;

            index = next_index;
        }
        next_index = cur_index;

        for (int i = 1; i < next_index - index; ++i) {
            out << "----";
        }

        out << "-E";

        out << std::endl;

        level++;
    }

    return out;
}
// End of cpp file

#endif
// End of header file
