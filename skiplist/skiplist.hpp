/*
Skip list implementation
Only the skiplist container should be visible
*/
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>
#include <map>
#include <random>
#include <iterator>

#include <iomanip>
#include <iostream>

template<
    typename value_type,
    typename compare_t = std::less<value_type>
>
class skiplist;

template<typename T>
void visualize(const skiplist<T>&);

template<typename T>
struct SLNode {
    // Poorly named - left, right, up, down pointers
    SLNode *back, *next, *up, *down;
    // Value, should be templated
    T val;
    // Count is integer only, will only be 0 for key nodes
    int count;
    SLNode(T val_)
        : back(nullptr), next(nullptr), up(nullptr), down(nullptr), val(val_), count(1) {}
    // TODO: not nice, T must have default constructor, must figure a workaround
    SLNode()
        : back(nullptr), next(nullptr), up(nullptr), down(nullptr), count(0) {}
};

template<
    typename value_type,
    typename compare_t
>
class skiplist {
private:
    // header nodes for all levels
    std::vector<SLNode<value_type>*> key;
    // number of nodes in the skiplist (including non-unique ones)
    int size_;
    // the last node at level 0
    SLNode<value_type>* last;

    // stuff required for random number generation
    // see constructor for description/reference
    std::mt19937_64 mt_;
    std::uniform_real_distribution<double> dist_;

    // template objects, since compare is supposed to be a functor
    compare_t compare;

public:
    // Iterator always points to a level 0 node
    // or a nullptr for end()
    class iterator : std::bidirectional_iterator_tag {
    private:
        // since the skip list supports
        // having non-unique elements with
        // the help of a count, to keep
        // track of whether the iterator
        // should actually move front or back
        // given non-unique elements, we make
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
        // To increment or decrement this iterator, just change node
        SLNode<value_type> *node;
        iterator(SLNode<value_type> *node_, bool reverse=false) : node(node_) {
            if(node_) {
                node_count_ = node_->count - 1;
                node_count_ref_ = node_count_;
            }
            this->reverse_ = reverse;
        }

        bool operator==(const iterator &rhs) {
            return node==rhs.node;
        }
        bool operator!=(const iterator &rhs) {
            return !(*this==rhs);
        }
        // de-reference operator
        // RVALUE returned, else skiplist could break
        value_type operator*() const {
            return node->val;
        }
        // pre-increment operator
        iterator& operator++() {
            // only if 'all' the non-unique
            // elements of the current node
            // have been 'traversed' in the
            // forward direction, move the
            // iterator forward.
            if(node_count_ != 0) {
                --node_count_;
                return *this;
            }
            if(reverse_) {
                node = node->back;
            }
            else {
                node = node->next;
            }
            if(node) {
                node_count_ = node->count - 1;
                node_count_ref_ = node_count_;
            }
            return *this;
        }
        // post-increment opreator
        iterator operator++(int) {
            if(node_count_ != 0) {
                --node_count_;
                return *this;
            }
            iterator temp(node);
            ++*this;
            return temp;
        }
        // pre-decrement operator
        iterator& operator--() {
            // only if 'all' the non-unique
            // elements of the current node
            // have been 'traversed' in the
            // backward direction, move the
            // iterator backward.
            if(node_count_ + 1 < node_count_ref_) {
                ++node_count_;
                return *this;
            }
            if(reverse_) {
                node = node->front;
            }
            else {
                node = node->back;
            }
            if(node) {
                node_count_ = node->count - 1;
                node_count_ref_ = node_count_;
            }
            return *this;
        }
        // post-decrement operator
        iterator operator--(int) {
            if(node_count_ + 1 < node_count_ref_) {
                ++node_count_;
                return *this;
            }
            iterator temp(node);
            --*this;
            return temp;
        }
    };

    class constant_iterator {
    private:
        // see iterator for documentation about the below variables
        // and their use in pre, post - increment, decrement.
        int node_count_;
        int node_count_ref_;
        bool reverse_;
    public:
        // To increment or decrement this iterator, just change node
        SLNode<value_type> *node;
        constant_iterator(SLNode<value_type> *node_, bool reverse_=false) : node(node_) {
            if(node_) {
                node_count_ = node_->count - 1;
                node_count_ref_ = node_count_;
            }
            this->reverse_ = reverse_;
        }

        bool operator==(const constant_iterator &rhs) {
            return node==rhs.node;
        }
        bool operator!=(const constant_iterator &rhs) {
            return !(*this==rhs);
        }
        // de-reference operator
        const value_type& operator*() const {
            return node->val;
        }
        // pre-increment operator
        const constant_iterator& operator++() {
            if(node_count_ != 0) {
                --node_count_;
                return *this;
            }
            if(reverse_) {
                node = node->back;
            }
            else {
                node = node->next;
            }
            if(node) {
                node_count_ = node->count - 1;
                node_count_ref_ = node_count_;
            }
            return *this;
        }
        // post-increment opreator
        constant_iterator operator++(int) {
            if(node_count_ != 0) {
                --node_count_;
                return *this;
            }
            constant_iterator temp(node);
            ++*this;
            return temp;
        }
        // pre-decrement operator
        const constant_iterator& operator--() {
            if(node_count_ + 1 < node_count_ref_) {
                ++node_count_;
                return *this;
            }
            if(reverse_) {
                node = node->front;
            }
            else {
                node = node->back;
            }
            if(node) {
                node_count_ = node->count - 1;
                node_count_ref_ = node_count_;
            }
            return *this;
        }
        // post-decrement operator
        constant_iterator operator--(int) {
            if(node_count_ + 1 < node_count_ref_) {
                ++node_count_;
                return *this;
            }
            constant_iterator temp(node);
            --*this;
            return temp;
        }
    };

    skiplist() : size_(0), last(nullptr) {
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

    // At every level, go on till nullptr and delete everything in its path
    // then go on to the upper level
    ~skiplist() {
        SLNode<value_type> *tmp;
        for(auto level: key)
            while(level) {
                tmp = level->next;
                delete level;
                level = tmp;
            }
    }

    // acts like a getter
    int size() {
        return size_;
    }

    // forward iterator to begin
    iterator begin() {
        return key.empty() ? end() : iterator(key[0]->next);
    }

    // forward iterator to one beyond last.
    iterator end() {
        return iterator(nullptr);
    };

    // reverse iterator pointing to last element
    iterator rbegin() {
        return iterator(last, true);
    }

    // sneak trick -> if the list is empty, last will be a nullptr
    // thus returning rbegin() is okay. it also provides good symmetry wrt begin
    iterator rend() {
        return key.empty() ? rbegin() : iterator(key[0], true);
    }

    // constant forward iterator
    constant_iterator cbegin() {
        return key.empty() ? cend() : constant_iterator(key[0]->next);
    }

    // constant forward iterator pointing to one beyond the last node
    constant_iterator cend() {
        return constant_iterator(nullptr);
    }

    // constant reverse iterator pointing to last element
    constant_iterator crbegin() {
        // cout << last->val;
        return constant_iterator(last, true);
    }

    // constant reverse iterator pointing to last one before the first node
    constant_iterator crend() {
        return key.empty() ? crbegin() : constant_iterator(key[0], true);
    }

    // specialized functions
    void insert(value_type value);
    // erase can be overloaded
    // this version finds the value and deletes the node if it exists
    // one more version of erase is passing an iterator object
    void erase(value_type value);
    iterator erase(iterator it);
    iterator find(value_type value);
    friend void visualize<value_type>(const skiplist<value_type>&);
};


/*
================================================================================
=================== NOTE! THE PART BELOW IS FROM THE CPP FILE ==================
================================================================================
*/

// utility function to check equality, am lazy
template<typename T, typename op>
bool _420_is_equal(T &a, T& b, op less_than) {
    return !less_than(a, b) && !less_than(b, a);
}

template<typename T, typename X>
// Inserting same value just increments count
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

    // If node already exists, just increment its count
    if(follow->next && _420_is_equal(follow->next->val, value, compare)) {
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
    if(!node->next)
        last = node;

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

template<typename T>
void visualize(const skiplist<T>& sl) {
    if (sl.key.empty()) {
      std::cout << "EMPTY SKIPLIST" << std::endl;
      return;
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
