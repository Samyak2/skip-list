# skip-list

C++ implementation of skip list compatible with STL

## Usage

```bash
mkdir build
cd build
cmake ..
make
```

This will generate the example binaries (`tester`, etc.).

### Running benchmarks

Requirements:
 - Python 3
 - `matplotlib`

After building from above, run the following (while inside `build` directory):

```bash
make benchmarks
python ../examples/benchmarks.py
```

The graphs will be saved in the same `build` directory.
