#ifndef __RANK_SUPPORT_H__
#define __RANK_SUPPORT_H__

#include <iostream>
#include <fstream>
#include <cmath>
#include <bit>
#include "compact_vector.hpp"
#include "bit_vector.hpp"

class RankSupport {
    private:
        BitVector& bv;
        std::unique_ptr<compact::vector<unsigned int>> cum_ranks;
        std::unique_ptr<compact::vector<unsigned int>> rel_cum_ranks;

    public:
        RankSupport(BitVector& b) : bv { b } {
            uint64_t n = this->bv.size();
            double log2n = std::log2(n);
            uint64_t chunk_size = std::ceil(log2n * log2n);
            uint64_t subchunk_size = std::ceil(log2n / 2);
            uint64_t num_chunks = std::ceil(static_cast<float>(n) / chunk_size);
            uint64_t subchunks_per_chunk = std::ceil(static_cast<float>(chunk_size) / subchunk_size);

            cum_ranks = std::unique_ptr<compact::vector<unsigned int>>(new compact::vector<unsigned int>(std::ceil(log2n)));
            rel_cum_ranks = std::unique_ptr<compact::vector<unsigned int>>(new compact::vector<unsigned int>(std::ceil(std::log2(chunk_size))));
            
            cum_ranks->resize(num_chunks);
            rel_cum_ranks->resize(num_chunks * subchunks_per_chunk);

            uint64_t cum_rank = 0;
            for (uint64_t i = 0; i < num_chunks; i++) {
                cum_ranks->operator[](i) = cum_rank;
                for (uint64_t j = 0; j < subchunks_per_chunk; j++) {
                    rel_cum_ranks->operator[](i * subchunks_per_chunk + j) = cum_rank - cum_ranks->operator[](i);
                    for (uint64_t k = 0; k < subchunk_size; k++) {
                        uint64_t idx = i * chunk_size + j * subchunk_size + k;
                        if (idx >= this->bv.size()) return;
                        else if (idx >= (i + 1) * chunk_size) break;
                        cum_rank += this->bv.get(idx);
                    }
                }
            } 
        }

        BitVector& get_bitvector() {
            return bv;
        }

        uint64_t rank1(uint64_t i) {
            i = i - 1;

            uint64_t n = this->bv.size();
            double log2n = std::log2(n);
            uint64_t chunk_size = std::ceil(log2n * log2n);
            uint64_t subchunk_size = std::ceil(log2n / 2);
            uint64_t subchunks_per_chunk = std::ceil(static_cast<float>(chunk_size) / subchunk_size);

            uint64_t chunk = i / chunk_size;
            uint64_t subchunk = chunk * subchunks_per_chunk + (i % chunk_size) / subchunk_size;
            uint64_t subchunk_offset = chunk * chunk_size + (subchunk - (chunk * subchunks_per_chunk)) * subchunk_size;
            
            uint64_t cum_rank = cum_ranks->operator[](chunk);
            uint64_t rel_cum_rank = rel_cum_ranks->operator[](subchunk);
            uint64_t rank_within_subchunk = std::popcount(bv.get_int(subchunk_offset, (i + 1) - subchunk_offset));

            return cum_rank + rel_cum_rank + rank_within_subchunk;
        }

        uint64_t overhead() {
            return (cum_ranks->bytes() * CHAR_BIT) + (rel_cum_ranks->bytes() * CHAR_BIT);
        }

        void save(std::string fname) {
            bv.save(fname);

            std::ofstream file;
            file.open(fname, std::ios::out | std::ios::binary | std::ios::app);
            file.write(reinterpret_cast<const char *>(cum_ranks->get()), cum_ranks->bytes());
            file.write(reinterpret_cast<const char *>(rel_cum_ranks->get()), rel_cum_ranks->bytes());
            file.close();
        }

        void load(std::string fname) {
            bv.load(fname);

            std::ifstream file;
            file.open(fname, std::ios::in | std::ios::binary);
            file.seekg(bv.size());
            file.read(reinterpret_cast<char *>(cum_ranks->get()), cum_ranks->bytes());
            file.read(reinterpret_cast<char *>(rel_cum_ranks->get()), rel_cum_ranks->bytes());
            file.close();
        }       
};

#endif
