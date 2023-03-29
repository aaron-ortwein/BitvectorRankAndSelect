#ifndef __SELECT_SUPPORT_H__
#define __SELECT_SUPPORT_H__

#include <iostream>
#include <fstream>
#include "rank_support.hpp"

class SelectSupport {
    private:
        RankSupport& rs;

    public:
        SelectSupport(RankSupport& r) : rs { r } {}

        uint64_t select1(uint64_t i) {
            BitVector& bv = rs.get_bitvector();

            uint64_t l = 1;
            uint64_t r = bv.size();

            while (l <= r && l < bv.size()) {
                uint64_t m = (l + r) / 2;
                uint64_t rank = rs.rank1(m);
 
                if (rank >= i) r = m;
                else l = m + 1;

                if (l == r && rs.rank1(l) == i) return l;
            }

            return std::numeric_limits<uint64_t>::max();
        }

        uint64_t overhead() {
            return rs.overhead();
        }

        void save(std::string fname) {
            rs.save(fname);
        }

        void load(std::string fname) {
            rs.load(fname);
        }
};

#endif
