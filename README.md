# skip-list

C++ implementation of skip list compatible with STL.  
Documentation contains:
1. [Usage](#usage) _(i.e. how to navigate this project)_
2. [Reference](#reference) _(major interfaces)_
3. Examples _(how to use skiplist in your own project)_

## Usage
1. Clone the repository `https://github.com/Samyak2/skip-list/` and `cd` into it
2. To build all the sample binaries from the `example` folder: 
```bash
mkdir build
cd build
cmake ..
make
```

This will generate the sample binaries (`tester`, etc.)  
They act as test-cases, covering all major functionalities of skiplist.  

### Running benchmarks

Requirements:
 - Python 3
 - `matplotlib` -> `python3 -m pip install matplotlib`

The benchmarking application generates graphs to visualize the time
complexity of the algorithms of skiplist.  
To view them `cd` into `build` and then:

```bash
cmake..
make benchmarks
python3 ../examples/benchmarks.py
```

The graphs will be saved in the same `build` directory.

## Reference
Skiplist is defined in `skiplist.hpp`:  
```cpp
template<
    typename val_type,
    typename compare_t = std::less<val_type>
>
class skiplist;
```  
`skiplist` is a sorted container, similar to `std::multiset`.   
Internally it uses a Skip-list which is a
simpler alternative to a height balanced binary search tree.  
Sorting is done based on the `compare_t` functor class which by default uses `<`.  
Multiple elements with the same `val_type` (i.e. key value) are allowed 
and are accessed in the order they were inserted.  
This order does not change.
Following the standard library, equivalence is defined as: `!comp(a, b) && !comp(b, a)`.  
All the classes within the header file are canonical classes.  

### Member types (of iterator, not skiplist)
* difference_type = std::ptrdiff_t;  
* value_type = val_type;  
* pointer = val_type*;  
* reference = val_type&;  
* iterator_category = std::bidirectional_iterator_tag;  

### Member functions
* constructor -> default, move, copy, from pair of iterators, initialization list
* destructor
* operator= -> move, copy

#### Iterators (All invalidated on a modify operation)
* begin  
  cbegin
* end  
  cend
* rbegin  
  crbegin
* rend  
  crend
  
#### Capacity
* size() -> Returns total number of elements in skiplist (including non-unique ones)

#### Modifiers
* insert(val_type) -> insert an element in logarithmic time
* erase(val_type / iterator) -> remove an element in logarithmic time

#### Lookup
* count(val_type) -> return number of elements matching a specific key
* find(val_type) -> finds an element in logarithmic time

### Non-member functions
* operator<< -> prints out the skip list level by level

### Notes
Like other sorted associative containers a skiplist's iterators are just aliases to its constant iterators.  
BUG REPORTS - Write it in a file `bug-report.txt` and send it to `/dev/null`
