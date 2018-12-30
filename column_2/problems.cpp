#include "catch.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <random>
#include <set>
#include <iostream>
#include <list>


TEST_CASE("Problem 2.6.1", "anagrams") {
    // 1. Consider the problem of finding all the anagrams of a given input word.  How
    // would you solve this problem given only the word and the dictionary?  What if
    // you could spend some time and space to process the dictionary before answering
    // any queries?

    using Dictionary = std::vector<std::string>;
    const Dictionary dict = {
        "deposit",
        "dopiest",
        "posited",
        "topside",
        "peter",
        "word",
        "sail",
        "smile",
        "ant",
        "tan",
    };

    auto&& sign = [](std::string w) -> std::string {
        std::sort(w.begin(), w.end());
        return w;
    };

    auto&& contains = [](const std::vector<std::string> &ws, std::string w) {
        return std::find(ws.begin(), ws.end(), w) != ws.end();
    };

    using Anagrams = std::vector<std::string>;

    // With no pre-processing
    auto&& find_anagrams1 = [&](std::string word) -> Anagrams {
        Anagrams anagrams;
        // const std::string word = "deposit";
        const std::string sig = sign(word);
        for (auto&& w: dict) {
            const std::string sig2 = sign(w);
            if (sig == sig2) {
                anagrams.push_back(w);
            }
        }
        return anagrams;
    };

    {
        auto as1 = find_anagrams1("deposit");
        REQUIRE(as1.size() == 4);
        REQUIRE(contains(as1, "deposit") == true);
        REQUIRE(contains(as1, "dopiest") == true);
        REQUIRE(contains(as1, "posited") == true);
        REQUIRE(contains(as1, "topside") == true);

        auto as2 = find_anagrams1("ant");
        REQUIRE(as2.size() == 2);
        REQUIRE(contains(as2, "ant") == true);
        REQUIRE(contains(as2, "tan") == true);

        auto as3 = find_anagrams1("peter");
        REQUIRE(as3.size() == 1);
        REQUIRE(contains(as3, "peter") == true);
    }

    // With preprocessing
    using Preprocess = std::map<std::string, std::vector<std::string>>;
    Preprocess pp;
    for (auto&& w: dict) {
        const std::string sig = sign(w);
        pp[sig].push_back(w);
    }
    auto&& find_anagrams2 = [&](std::string word) -> Anagrams {
        const std::string sig = sign(word);
        auto&& it = pp.find(sig);
        return it != pp.end() ? it->second : Anagrams{};
    };

    {
        auto as1 = find_anagrams2("deposit");
        REQUIRE(as1.size() == 4);
        REQUIRE(contains(as1, "deposit") == true);
        REQUIRE(contains(as1, "dopiest") == true);
        REQUIRE(contains(as1, "posited") == true);
        REQUIRE(contains(as1, "topside") == true);

        auto as2 = find_anagrams2("ant");
        REQUIRE(as2.size() == 2);
        REQUIRE(contains(as2, "ant") == true);
        REQUIRE(contains(as2, "tan") == true);

        auto as3 = find_anagrams2("peter");
        REQUIRE(as3.size() == 1);
        REQUIRE(contains(as3, "peter") == true);
    }
}


TEST_CASE("Problem 2.6.2", "duplicate") {
    // 2. Given a sequential file containing 4,300,000,000 32-bit integers, how can you find
    // one that appears at least twice?

    // NOTE(peter): we know that at least 1 appears twice because 2**32 = 4294967296

    // Solution #1: Sort and look for dups
    // Solution #2: Bitvector
    // Solution #3: keep std::set<>
    // Solution #4: (brute force): map of value -> count

    // Solution #1:
    using IntType = uint16_t;
    constexpr size_t N = (1 << 16) + 100;
    std::vector<IntType> vs;
    vs.reserve(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::generate_n(std::back_inserter(vs), N, gen);

    std::sort(vs.begin(), vs.end());
    auto end = vs.end() - 1;
    IntType answer1;
    for (auto it = vs.begin(); it != end; ++it) {
        if (*it == *std::next(it)) {
            answer1 = *it;
            break;
        }
    }

    // Solution #2:
    std::set<IntType> seen;
    IntType answer2;
    for (auto v: vs) {
        if (seen.count(v) > 0) {
            answer2 = v;
            break;
        }
        seen.insert(v);
    }

    // Brute force
    std::map<IntType, int> counts;
    for (auto v: vs) {
        ++counts[v];
    }
    REQUIRE(counts[answer1] > 1);
    REQUIRE(counts[answer2] > 1);
}

// stupid ADL...
namespace my {

// template <class BidirIt>
// // NOTE(peter): just begin pedantic and enforcing that the iterator is infact a BidirectionalIterator
// std::enable_if_t<
//     std::is_base_of<
//         std::bidirectional_iterator_tag,
//         typename std::iterator_traits<BidirIt>::iterator_category
//     >::value
// >
// reverse(BidirIt begin, BidirIt end) {
//     if (!(begin < end)) {
//         return;
//     }
//     while (begin < --end) {
//         std::iter_swap(begin++, end);
//     }
// }

// To be pedantically correct, BidirectionalIterator does not
// support operator<

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last,
        std::bidirectional_iterator_tag)
{
    while ((first != last) && (first != --last)) {
        std::iter_swap(first++, last);
    }
}


template <typename RandomAccessIt>
void reverse(RandomAccessIt first, RandomAccessIt last,
        std::random_access_iterator_tag)
{
    if (first == last) {
        return;
    }
    while (first < --last) {
        std::iter_swap(first++, last);
    }
}

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last)
{
    using iterator_category = typename std::iterator_traits<BidirectionalIt>::iterator_category;
    reverse(first, last, iterator_category());
}

template <class Cont>
auto reverse_copy(Cont cont)
{
    my::reverse(std::begin(cont), std::end(cont));
    return cont;
}

template <class ForwardIt>
void print_range(ForwardIt first, ForwardIt last)
{
    while (first != last) {
        std::cout << *first++ << " ";
    }
    std::cout << "\n";
}

// NOTE(peter): the std algorithms only constrains to ForwardIt,
//              but since I'm using reverse, I have to constrain
//              to BidirectionIterator.

// template <class ForwardIt>
// void rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
template <class BidirectionalIt>
void rotate(BidirectionalIt first, BidirectionalIt n_first, BidirectionalIt last)
{
    // n_first - first swaps
    my::reverse(first, n_first);
    // last - n_first swaps
    my::reverse(n_first, last);
    // last - first swaps
    my::reverse(first, last);
    // (n_first - first) + (last - n_first) + (last - first)
    // = n_first - first + last - n_first + last - first]
    // = 2*last - 2*first
    // = 2*(last - first) = O(2N)
}

template <class It>
void rotate2(It first, It n_first, It last) {
    print_range(first, last);

    using Distance = typename std::iterator_traits<It>::difference_type;
    using ValueType = typename std::iterator_traits<It>::value_type;
    Distance n = last - first;
    Distance i = n_first - first;
    // int moves = 0;

    for (Distance u = 0; u < 1; ++u) {
        Distance j = i + u;
        Distance k = j+i;
        Distance z = k % n;
        ValueType t = *(first + u);
        *(first + u) = *(first + j);
        // ++moves;
        do {
            *(first + j) = *(first + z);
            // ++moves;
            j = z;
            k += i;
            z = k % n;
        } while (z != 0);
        *(first + j) = t;

        // if (moves >= n) {
        //     break;
        // }
    }

    print_range(first, last);
}

} // ~namespace my

TEST_CASE("Reverse", "Reverse portion of array part of rotate") {
    // Using RandomAccessIterator
    static_assert(
        std::is_same<
            std::iterator_traits<std::vector<int>::iterator>::iterator_category,
            std::random_access_iterator_tag
        >::value,
        "Vector iterator isn't random access iterator!"
    );
    SECTION("Odd number of elements") {
        std::vector<int> values = { 1, 2, 3, 4, 5, 6, 7 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values != r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(values == r2);
    }

    SECTION("Even number of elements") {
        std::vector<int> values = { 1, 2, 3, 4, 5, 6 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values != r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(values == r2);
    }

    SECTION("No elements") {
        std::vector<int> values;
        auto r1 = my::reverse_copy(values);
        REQUIRE(values == r1);
        REQUIRE(r1.empty());
    }

    SECTION("1 element") {
        std::vector<int> values = { 1 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values == r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(r1 == r2);
    }

    // Using BidirectionalIterator
    static_assert(
        std::is_same<
            std::iterator_traits<std::list<int>::iterator>::iterator_category,
            std::bidirectional_iterator_tag
        >::value,
        "List iterator isn't bidirectional iterator!"
    );
    SECTION("Odd number of elements") {
        std::list<int> values = { 1, 2, 3, 4, 5, 6, 7 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values != r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(values == r2);
    }

    SECTION("Even number of elements") {
        std::list<int> values = { 1, 2, 3, 4, 5, 6 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values != r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(values == r2);
    }

    SECTION("No elements") {
        std::list<int> values;
        auto r1 = my::reverse_copy(values);
        REQUIRE(values == r1);
        REQUIRE(r1.empty());
    }

    SECTION("1 element") {
        std::list<int> values = { 1 };
        auto r1 = my::reverse_copy(values);
        REQUIRE(values == r1);
        auto r2 = my::reverse_copy(r1);
        REQUIRE(r1 == r2);
    }

}

template <typename ForwardIt>
ForwardIt adv(ForwardIt it, int dist) {
    while (dist-- > 0) {
        ++it;
    }
    return it;
}


TEST_CASE("Rotate", "rotate using reverse algo")
{
    // Tests using RandomAccessIterator
    SECTION("Rotate around middle")
    {
        std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 5;
        auto v1 = vs;
        my::rotate(v1.begin(), v1.begin()+i, v1.end());
        REQUIRE(v1[0] == vs[i]);
        auto v2 = vs;
        std::rotate(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate less")
    {
        std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 1;
        auto v1 = vs;
        my::rotate(v1.begin(), v1.begin()+i, v1.end());
        REQUIRE(v1[0] == vs[i]);
        auto v2 = vs;
        std::rotate(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate noop")
    {
        std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto v1 = vs;
        my::rotate(v1.begin(), v1.begin(), v1.end());
        REQUIRE(v1 == vs);
        auto v2 = vs;
        std::rotate(v2.begin(), v2.begin(), v2.end());
        REQUIRE(v2 == vs);
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate #2")
    {
        std::cout << "SECOND ROTATE ALGO\n";
        //std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        std::vector<int> vs = { 1, 2, 3, 4, 5 };
        my::print_range(vs.begin(), vs.end());
        int i = 3;
        auto v1 = vs;
        my::rotate2(v1.begin(), v1.begin()+i, v1.end());
        std::cout << "ANSWER FROM ALGO #2:\n";
        my::print_range(v1.begin(), v1.end());
        auto v2 = vs;
        std::rotate(v2.begin(), v2.begin()+i, v2.end());
        std::cout << "CORRECT ANSWER:\n";
        my::print_range(v2.begin(), v2.end());
    }

    // Tests using BidirectionalIterator
    SECTION("Rotate around middle")
    {
        std::list<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 5;
        auto v1 = vs;
        my::rotate(v1.begin(), adv(v1.begin(), i), v1.end());
        REQUIRE(v1.front() == *adv(vs.begin(), i));
        auto v2 = vs;
        std::rotate(v2.begin(), adv(v2.begin(), i), v2.end());
        REQUIRE(v2.front() == *adv(vs.begin(), i));
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate less")
    {
        std::list<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 1;
        auto v1 = vs;
        my::rotate(v1.begin(), adv(v1.begin(), i), v1.end());
        REQUIRE(v1.front() == *adv(vs.begin(), i));
        auto v2 = vs;
        std::rotate(v2.begin(), adv(v2.begin(), i), v2.end());
        REQUIRE(v2.front() == *adv(vs.begin(), i));
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate noop")
    {
        std::list<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto v1 = vs;
        my::rotate(v1.begin(), v1.begin(), v1.end());
        REQUIRE(v1 == vs);
        auto v2 = vs;
        std::rotate(v2.begin(), v2.begin(), v2.end());
        REQUIRE(v2 == vs);
        REQUIRE(v1 == v2);
    }
}
