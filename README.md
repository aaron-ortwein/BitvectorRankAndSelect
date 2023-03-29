# Build

The project can be built simply by running `make`, which will produce the executable `main`.
The use of C++20 is required for the built-in `std::popcount` instruction.

# Use

`main` runs benchmarks for the RankSupport, SelectSupport, and SparseArray classes, plus runs
some tests for the SparseArray class (one of which involves saving and loading to a file called
`sparsearray.txt`).

The public API of BitVector, RankSupport, SelectSupport, and SparseArray can be accessed by including
the appropriate header file. BitVector provides a very thin wrapper around a limited set of functionality
from `compact::vector`, including allocating the bit vector, getting the values of bits, setting the value
of bits (to 1), extracting a slice of (< 64) bits from an offset, and saving to and loading from a file.
The APIs of the RankSupport, SelectSupport, and SparseArray classes are exactly as described in the Homework 2
specifications, with the exception of the fact that the SparseArray class does not have a `create()` member
function. Instead, the underlying bit vector size is provided as an argument to the constructor. It should
also be noted that the SparseArray class consistently uses an *inclusive* definitition of rank. An example
of using the SparseArray class can be found in the `main()` function of `main.cpp`.

# References

Generating random bit vectors with various densities was accomplished with the help of the following resources:
* https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
* https://stackoverflow.com/questions/21096015/how-to-generate-64-bit-random-numbers