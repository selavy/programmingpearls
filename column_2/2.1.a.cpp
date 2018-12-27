#include "catch.hpp"
#include "bitvector.h"
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>
#include <climits>
#include <iterator>
#include <limits>
#include <memory>


template <class T, class A>
void print_vector(const std::vector<T, A>& vec) {
    for (auto&& i: vec) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}


TEST_CASE("Problem #2.1a", "Find missing integer") {
    // Given a sequential file that contains at most four billion 32-bit integers
    // in random order, find a 32-bit integer that isn't in the file (and there
    // must be at least one missing -- why?).  How would you solve this problem with
    // ample quantities of main memory? How would you solve it if you could use
    // several external "scratch" files, but only a few hundred bytes of main memory?

    // Method #1: Keep presence bit-field.  First pass set bit for any number
    // that is present, then pass over bit-field and any bit that is not set
    // is a correct answer.
    //
    // However, this method is simplest if the numbers are all >0.  If not, need to
    // add 2**31 to every value.


    // // Note(peter): If we want unique numbers use this.  But decided there's
    // //              no reason they should be unique for this challenge.
    
    // constexpr size_t N = 10;
    // std::vector<uint32_t> vs;
    // vs.reserve(N);
    // for (uint32_t i = 0; i < N; ++i) {
    //     vs.push_back(i);
    // }

    // print_vector(vs);

    // constexpr size_t P = 10;
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dist(0, N-1);
    // for (int i = 0; i < P; ++i) {
    //     int a = dist(gen);
    //     int b = dist(gen);
    //     std::iter_swap(std::begin(vs)+a, std::begin(vs)+b);
    // }

    // print_vector(vs);

    // constexpr size_t N = 1000000;
    constexpr size_t N = 64000;
    std::vector<uint16_t> vs;
    vs.reserve(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::generate_n(std::back_inserter(vs), N, gen);


    using I = decltype(vs)::value_type;
    constexpr I max_value = std::numeric_limits<I>::max();
    constexpr I min_value = std::numeric_limits<I>::min();
    {
        // NOTE(peter): too big to put on the stack
        // col2::BitVector<max_value> bv;
        auto bv = std::make_unique<col2::BitVector<max_value>>();
        for (auto v: vs) {
            bv->set(v);
        }
        I answer = -1;
        for (I i = 0; i < max_value; ++i) {
            if (!bv->isset(i)) {
                // std::cout << i << "\n";
                answer = i;
                break;
            }
        }
        std::cout << "Answer #1: " << answer << "\n";
        REQUIRE(answer != -1);
        for (auto v: vs) {
            REQUIRE(v != answer);
        }
    }

    {
        // Method #2: sort values, then walk list until we find a hole
        std::sort(vs.begin(), vs.end());
        I answer = min_value;
        if (vs[0] == min_value) {
            for (auto v: vs) {
                if (v == answer) {
                    continue;
                } else if (v == answer + 1) {
                    ++answer;
                } else {
                    ++answer;
                    break;
                }
            }
        }

        std::cout << "Answer #2: " << answer << "\n";

        for (int i = 0; i < 20; ++i) {
            std::cout << vs[i] << " ";
        }
        
        for (auto v: vs) {
            REQUIRE(v != answer);
        }

    }
}
