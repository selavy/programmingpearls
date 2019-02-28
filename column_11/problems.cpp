#include "catch.hpp"
#include <iterator>
#include <cassert>
#include <algorithm>
#include <functional>

namespace ch11 {

template <class I, class C>
bool is_partitioned(I begin, I end, I mid, C cmp) {
    while (begin < end) {
        if (!cmp(*begin++, *mid)) return false;
    }
    return true;
}

template <class I>
I partition(I begin, I end) {
    auto lo = begin;
    auto hi = end;
    for (;;) {
        do { ++lo; } while (lo < end && *lo < *begin);
        do { --hi; } while (            *hi > *begin);
        if (!(lo < hi)) {
            break;
        }
        std::iter_swap(lo, hi);
    }
    std::iter_swap(begin, hi);
    return hi;
}

#if 0
template <class I>
I partition(I begin, I end) {
    auto lo = begin + 1;
    auto hi = end - 1;
    while (lo < hi) {
        if (!(*lo > *begin)) {
            ++lo;
        } else if (!(*hi < *begin)) {
            --hi;
        } else {
            std::iter_swap(lo++, hi--);
        }
    }
    auto mid = lo - 1;
    if (*begin > *lo) {
        mid = lo;
    }
    std::iter_swap(begin, mid);
    return mid;
}
#endif

template <class I>
I partition3(I begin, I end) {
    auto i = begin;
    auto j = end;
    for (;;) {
        do { ++i; } while (i < end && *i < *begin);
        do { --j; } while (*j > *begin);
        if (!(i < j)) {
            break;
        }
        std::iter_swap(i, j);
    }
    std::iter_swap(begin, j);
    return j;
}

template <class I>
I partition2(I l, I u)
{
    // invariant: [l, m) <  t
    // invariant: [m, u) >= t

    auto m = l;
    for (auto i = l + 1; i < u; ++i) {
        if (*i < *l) {
            std::iter_swap(++m, i);
        }
    }
    std::iter_swap(l, m);

    assert(is_partitioned(l, m, m, std::less<>{}));
    assert(is_partitioned(m, u, m, std::greater_equal<>{}));
    return m;
}

template <class I>
void qsort1(I begin, I end) {
    auto count = end - begin;
    if (count <= 1) {
        return;
    }
    auto mid = partition(begin, end);
    // assert(std::is_partitioned(begin, end, [mid](auto x) { return x < *mid; }));
    qsort1(begin, mid);
    qsort1(mid + 1, end);
}

template <class I>
void qsort2(I begin, I end) {
    auto count = end - begin;
    if (count <= 1) {
        return;
    }
    auto mid = partition2(begin, end);
    // assert(std::is_partitioned(begin, end, [mid](auto x) { return x < *mid; }));
    qsort2(begin, mid);
    qsort2(mid + 1, end);
}

template <class I>
void qsort3(I begin, I end) {
    auto count = end - begin;
    if (count <= 1) {
        return;
    }
    auto mid = partition3(begin, end);
    // assert(std::is_partitioned(begin, end, [mid](auto x) { return x < *mid; }));
    qsort3(begin, mid);
    qsort3(mid + 1, end);
}

} /*ch11*/

template <class C>
C copy_sort(C c) {
    std::sort(c.begin(), c.end());
    return c;
}

TEST_CASE("QuickSort", "[qs]") {
    std::vector<std::vector<int>> tests = {
        { 5, 1, 6, 3, 4, 8 },
        { 1, 1, 1, 1 },
        { 1, 2, 3, 4 },
        { 1, 2, 3 },
        { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2 },
        { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2, 1 },
    };

    for (auto&& vs : tests) {
        auto v1 = vs;
        ch11::qsort1(v1.begin(), v1.end());
        auto v2 = vs;
        ch11::qsort2(v2.begin(), v2.end());
        auto v3 = vs;
        ch11::qsort3(v3.begin(), v3.end());
        std::sort(vs.begin(), vs.end());
        REQUIRE(v1 == vs);
        REQUIRE(v2 == vs);
        REQUIRE(v3 == vs);
    }
}

TEST_CASE("Partition Tests", "[partition]") {
    std::vector<std::vector<int>> tests = {
        { 5, 3, 6, 4, 8, 9 },
        { 1, 1, 1, 1, 1, 1 },
        { 5, 3, 6, 4, 8, 9, 3 },
        { 1, 2 },
        { 1, 2, 3 },
    };

    for (auto&& vs : tests) {
        auto mid = ch11::partition2(vs.begin(), vs.end());
        REQUIRE(std::is_partitioned(vs.begin(), vs.end(), [mid](auto x) { return x < *mid; }) == true);
        REQUIRE(std::is_partitioned(vs.begin(), vs.end(), [mid](decltype(*vs.begin()) x) { return x < *mid; }) == true);
    }
}

TEST_CASE("QuickSort 2", "[qs2]") {
    std::vector<std::vector<int>> tests = {
        { 5, 1, 6, 3, 4, 8 },
        { 1, 1, 1, 1 },
        { 1, 2, 3, 4 },
        { 1, 2, 3 },
        { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2 },
        { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2, 1 },
    };

    for (auto&& vs : tests) {
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        REQUIRE(vs == expected);
    }
}
