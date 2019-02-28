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
    using ValueType = typename std::iterator_traits<I>::value_type;

    // partition [begin, end) around *begin
    auto lo = begin + 1;
    auto hi = end - 1;
    while (lo < hi) {
        // invariant: [begin + 1, lo ) <= *begin
        // invariant: (hi       , end) >= *begin
        assert(is_partitioned(begin + 1, lo , begin, std::less_equal<>{}));
        assert(is_partitioned(hi    + 1, end, begin, std::greater_equal<>{}));

        if (!(*lo > *begin)) {
            ++lo;
        } else if (!(*hi < *begin)) {
            --hi;
        } else {
            std::iter_swap(lo++, hi--);
        }

        assert(is_partitioned(begin + 1, lo , begin, std::less_equal<>{}));
        assert(is_partitioned(hi    + 1, end, begin, std::greater_equal<>{}));
    }

    auto mid = lo - 1;
    if (*begin > *lo) {
        mid = lo;
    }
    std::iter_swap(begin, mid);

    // assert is partitioned
    assert(is_partitioned(begin  , mid, mid, std::less_equal<>{}));
    assert(is_partitioned(mid + 1, end, mid, std::greater_equal<>{}));

    return mid;
}

template <class I>
void qsort(I begin, I end) {
    auto count = end - begin;
    if (count <= 1) {
        return;
    }
    auto mid = partition(begin, end);
    qsort(begin, mid);
    qsort(mid + 1, end);
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
void qsort2(I l, I u) {
    if ((u - l) <= 1) {
        return;
    }
    auto m = partition2(l, u);
    qsort2(l, m);
    qsort2(m+1, u);
}

} /*ch11*/

TEST_CASE("QuickSort", "[qs]") {
    {
        std::vector<int> vs = { 5, 1, 6, 3, 4, 8 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
    }

    {
        std::vector<int> vs = { 1, 1, 1, 1 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
    }

    {
        std::vector<int> vs = { 1, 2, 3, 4 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
    }

    {
        std::vector<int> vs = { 1, 2, 3 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
    }

    {
        std::vector<int> vs = { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
    }

    {
        std::vector<int> vs = { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2, 1 };
        ch11::qsort(vs.begin(), vs.end());
        REQUIRE(std::is_sorted(vs.begin(), vs.end()));
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

template <class C>
C copy_sort(C c) {
    std::sort(c.begin(), c.end());
    return c;
}

TEST_CASE("QuickSort2", "[qs2]") {
    {
        std::vector<int> vs = { 5, 1, 6, 3, 4, 8 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }

    {
        std::vector<int> vs = { 1, 1, 1, 1 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }

    {
        std::vector<int> vs = { 1, 2, 3, 4 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }

    {
        std::vector<int> vs = { 1, 2, 3 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }

    {
        std::vector<int> vs = { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }

    {
        std::vector<int> vs = { 5, 6, 7, 7, 1, 2, 10, 9, 8, 2, 1 };
        auto expected = copy_sort(vs);
        ch11::qsort2(vs.begin(), vs.end());
        // REQUIRE(std::is_sorted(vs.begin(), vs.end()));
        REQUIRE(vs == expected);
    }
}
