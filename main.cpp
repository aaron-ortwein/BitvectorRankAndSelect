#include <iostream>
#include <fstream>
#include "bit_vector.hpp"
#include "rank_support.hpp"
#include "select_support.hpp"
#include "sparse_array.hpp"

void rank_benchmark() {
    uint64_t sizes[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    uint64_t trials = 10000000;

    std::cout << "=== Rank1 Benchmarks ===" << std::endl;

    std::cout << std::setw(10) << "Size\t\tAvg. Time\tOverhead" << std::endl;
    for (uint64_t size : sizes) {
        BitVector b(size, 0.1);
        RankSupport rs(b);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) rs.rank1(b.size());
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << size << std::setw(16) << std::left << avg_time << rs.overhead() << std::endl;
    }

    std::cout << "========================" << std::endl;
    std::cout << std::endl;
}

void select_benchmark() {
    uint64_t sizes[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    uint64_t trials = 1000000;

    std::cout << "=== Select1 Benchmarks ===" << std::endl;

    std::cout << std::setw(10) << "Size\t\tTime\t\tOverhead" << std::endl;
    for (uint64_t size : sizes) {
        BitVector b(size, 0.25);
        RankSupport rs(b);
        SelectSupport ss(rs);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) ss.select1(b.size() * 0.25);
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << size << std::setw(16) << std::left << avg_time << ss.overhead() << std::endl;
    }

    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
}

SparseArray generate_sparse_array(uint64_t size, float density) {
    BitVector b(size, density);
    SparseArray s(size);

    for (uint64_t i = 0; i < size; i++) {
        if (b.get(i)) s.append("foo", i);
    }

    return s;
}

void sparse_array_size_benchmark() {
    uint64_t sizes[] = {1000, 10000, 100000, 1000000};
    float density = 0.05; 
    uint64_t trials = 1000000;

    std::cout << "*** Sparse Array Size Benchmarks ***" << std::endl;
    std::cout << std::endl;

    std::cout << "=== Generating Rank Data Structure ===" << std::endl;
    std::cout << std::setw(10) << "Size\t\tTime" << std::endl;
    for (uint64_t size : sizes) {
        SparseArray s = generate_sparse_array(size, density);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 1000; i++) s.finalize();
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / 1000;
 
        std::cout << std::setw(16) << std::left << size << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;

    std::cout << "=== num_elem_at() (rank1) ===" << std::endl;
    std::cout << std::setw(10) << "Size\t\tTime" << std::endl;
    for (uint64_t size : sizes) {
        SparseArray s = generate_sparse_array(size, density);
        s.finalize();

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) s.num_elem_at(size - 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << size << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "=============================" << std::endl;
    std::cout << std::endl;

    std::cout << "=== get_index_of() (select1) ===" << std::endl;
    std::cout << std::setw(10) << "Size\t\tTime" << std::endl;
    for (uint64_t size : sizes) {
        SparseArray s = generate_sparse_array(size, density);
        s.finalize();

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) s.get_index_of(size * density - 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << size << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    std::cout << "************************************" << std::endl;
    std::cout << std::endl;
}

void sparse_array_density_benchmark() {
    uint64_t size = 100000;
    float densities[] = {0.01, 0.05, 0.1, 0.25, 0.5, 0.9, 0.99}; 
    uint64_t trials = 1000000;

    std::cout << "*** Sparse Array Density Benchmarks ***" << std::endl;
    std::cout << std::endl;

    std::cout << "=== Generating Rank Data Structure ===" << std::endl;
    std::cout << std::setw(10) << "Density\t\tTime" << std::endl;
    for (auto density : densities) {
        SparseArray s = generate_sparse_array(size, density);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 1000; i++) s.finalize();
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / 1000;
 
        std::cout << std::setw(16) << std::left << density << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;

    std::cout << "=== num_elem_at() (rank1) ===" << std::endl;
    std::cout << std::setw(10) << "Density\t\tTime" << std::endl;
    for (auto density : densities) {
        SparseArray s = generate_sparse_array(size, density);
        s.finalize();

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) s.num_elem_at(size - 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << density << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "=============================" << std::endl;
    std::cout << std::endl;

    std::cout << "=== get_index_of() (select1) ===" << std::endl;
    std::cout << std::setw(10) << "Density\t\tTime" << std::endl;
    for (auto density : densities) {
        SparseArray s = generate_sparse_array(size, density);
        s.finalize();

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < trials; i++) s.get_index_of(size * density - 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        double avg_time = static_cast<float>(time) / trials;
 
        std::cout << std::setw(16) << std::left << density << std::setw(16) << std::left << avg_time << std::endl;
    }
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    std::cout << "************************************" << std::endl;
    std::cout << std::endl;
}

int main() {
    rank_benchmark();
    select_benchmark();
    sparse_array_size_benchmark();
    sparse_array_density_benchmark();
    
    std::cout << "=== Sparse Array Tests ===" << std::endl;
    SparseArray s(11);
    s.append("foo", 1);
    s.append("bar", 5);
    s.append("baz", 9);
    s.append("quux", 10);
    s.finalize();
    std::cout << "underlying bit vector size: " << s.size() << std::endl;
    std::cout << "num elems: " << s.num_elem() << std::endl;
    std::string e;
    std::cout << s.get_index_of(4) << std::endl;
    std::cout << s.num_elem_at(10) << std::endl;
    std::cout << s.get_at_index(10, e) << std::endl;
    std::cout << e << std::endl;
    std::cout << s.get_at_rank(3, e) << std::endl;
    std::cout << e << std::endl;
    std::cout << s.get_at_index(2, e) << std::endl;
    std::cout << "saving..." << std::endl;
    s.save("sparsearray.txt");
    SparseArray s2(11);
    std::cout << "loading..." << std::endl;
    s2.load("sparsearray.txt");
    s2.finalize();
    std::cout << s2.get_index_of(4) << std::endl;
    std::cout << s2.num_elem_at(10) << std::endl;
    std::cout << s2.get_at_index(10, e) << std::endl;
    std::cout << e << std::endl;
    std::cout << s2.get_at_rank(3, e) << std::endl;
    std::cout << e << std::endl;
    std::cout << s2.get_at_index(2, e) << std::endl;

    return 0;
}