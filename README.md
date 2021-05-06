# skip-list

C++ implementation of skip list compatible with STL.  
Documentation contains:
1. [Usage](#usage) _(i.e. how to navigate this project)_
2. Reference _(major interfaces)_
3. Examples _(how to use skiplist in your own project)_

## Usage
1. Clone the repository and `cd` into it
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
