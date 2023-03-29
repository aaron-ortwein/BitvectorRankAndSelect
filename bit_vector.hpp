#ifndef __BIT_VECTOR_H__
#define __BIT_VECTOR_H__

#include <iostream>
#include <fstream>
#include <random>
#include "compact_vector.hpp"

class BitVector {
    private:
        compact::vector<unsigned int> b;

    public:
        BitVector(uint64_t size, float density = 0) : b(1) {
            b.resize(size);

            // The following algorithm to generate random bit vectors with a certain density was
            // accomplished with the help of:
            // * https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
            // * https://stackoverflow.com/questions/21096015/how-to-generate-64-bit-random-numbers
            uint64_t set_bits = size * density;
            for (uint64_t i = 0; i < set_bits; i++) this->set(i);
            if (set_bits) {
                std::random_device rd;
                std::mt19937_64 mte(rd());
                for (uint64_t i = 0; i < set_bits; i++) {
                    std::uniform_int_distribution<long long int> distribution(i, b.size() - 1);
                    uint64_t j = distribution(mte);

                    uint64_t temp = b[i];
                    b[i] = b[j];
                    b[j] = temp;
                }
            }
        }

        uint64_t get(uint64_t i) {
            return b.at(i);
        }

        void set(uint64_t i) {
            b[i] = 1;
        }

        uint64_t get_int(unsigned int offset, unsigned int bits) {
            if (bits == 0) return 0;
            return (b.cbegin() + offset).get_bits(bits);
        }

        uint64_t size() {
            return b.size();
        }

        void save(std::string fname) {
            std::ofstream file;
            file.open(fname, std::ios::out | std::ios::binary);
            file.write(reinterpret_cast<const char *>(b.get()), b.bytes());
            file.close();
        }

        void load(std::string fname) {
            std::ifstream file;
            file.open(fname, std::ios::in | std::ios::binary);
            file.read(reinterpret_cast<char *>(b.get()), b.bytes());
            file.close();
        }
};

std::ostream& operator<<(std::ostream &s, BitVector& b) {
    for (uint64_t i = 0; i < b.size(); i++) s << b.get(i);
    return s;
}

#endif
