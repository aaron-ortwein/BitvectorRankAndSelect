#ifndef __SPARSE_ARRAY_H__
#define __SPARSE_ARRAY_H__

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include "bit_vector.hpp"
#include "rank_support.hpp"
#include "select_support.hpp"

class SparseArray {
    private:
        BitVector b;
        std::unique_ptr<RankSupport> rs = nullptr;
        std::unique_ptr<SelectSupport> ss = nullptr;
        std::vector<std::string> strings;
    
    public:
        SparseArray(uint64_t size) : b(size) {}

        void append(std::string elem, uint64_t pos) {
            if (pos >= b.size()) throw std::invalid_argument("position must be less than bit vector size");
            b.set(pos);
            strings.push_back(elem);
        }

        void finalize() {
            this->rs = std::unique_ptr<RankSupport>(new RankSupport(this->b));
            this->ss = std::unique_ptr<SelectSupport>(new SelectSupport(*this->rs));
        }

        bool get_at_rank(uint64_t r, std::string& elem) {
            if (r == 0 || r > this->num_elem()) return false;
            elem = strings[r - 1];
            return true;
        }

        bool get_at_index(uint64_t r, std::string& elem) {
            if (b.get(r)) {
                elem = strings[rs->rank1(r)];
                return true;
            }
            return false;
        }

        uint64_t get_index_of(uint64_t r) {
            if (r == 0 || r > this->num_elem()) return std::numeric_limits<uint64_t>::max();
            return ss->select1(r) - 1;
        }

        uint64_t num_elem_at(uint64_t r) {
            return rs->rank1(r + 1);
        }

        uint64_t size() {
            return this->b.size();
        }

        uint64_t num_elem() {
            return this->strings.size();
        }

        void save(std::string fname) {
            b.save(fname);

            std::ofstream file;
            file.open(fname, std::ios::out | std::ios::binary | std::ios::app);
            std::copy(strings.cbegin(), strings.cend(), std::ostream_iterator<std::string>(file, "\n"));
            file.close();
        }

        void load(std::string fname) {
            b.load(fname);
            strings.clear();

            std::ifstream file;
            file.open(fname, std::ios::in | std::ios::binary);
            file.seekg(b.size());
            std::string line;
            while (std::getline(file, line)) strings.push_back(line);
            file.close();
        }
};

#endif
