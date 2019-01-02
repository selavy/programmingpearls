#include "catch.hpp"
#include <vector>
#include <algorithm>
#include <cassert>


namespace ch4 {

template <typename T, typename Iter>
// requires RandomAccessIterator<Iter>
// requires LessThan<T>
// // NOTE(peter): assumes "Equal = !LessThan(a, b) and !LessThan(b, a)"
Iter lower_bound(T value, Iter first, Iter last) noexcept
{
    // TODO: special case for outside range?
    if (value < *first) {
       return first;
    } else if (*(last - 1) < value) {
       return last - 1;
    }

    while (first < last) {
        auto mid = first + ((last - first) / 2);
        if (value < *mid) {
            last = mid;
        } else if (*mid < value) {
            first = mid + 1;
        } else {
            return mid;
        }
    }

    return *first < value ? first: last - 1;
}

template <class T, typename Iter>
Iter bsearch(T value, Iter first, Iter last) {
    auto found = ch4::lower_bound(value, first, last);
    assert(found >= first);
    assert(found < last);
    return *found == value ? found : last;
}

} // ~namespace ch4

TEST_CASE("Binary Search", "bsearch")
{
    SECTION("even elements") {
        std::vector<int> vs = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; ++i) {
            auto r = ch4::bsearch(i, vs.begin(), vs.end());
            REQUIRE(*r == i);
            REQUIRE(r == vs.begin()+i);
        }

        auto hi = ch4::lower_bound(4, vs.begin(), vs.end());
        REQUIRE(hi == vs.begin() + 3);

        auto hi2 = ch4::bsearch(4, vs.begin(), vs.end());
        REQUIRE(hi2 == vs.end());


        auto lo = ch4::lower_bound(-1, vs.begin(), vs.end());
        REQUIRE(lo == vs.begin());

        auto lo2 = ch4::bsearch(-1, vs.begin(), vs.end());
        REQUIRE(lo2 == vs.end());
    }

    SECTION("odd elements find missing in middle") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9, 10 };

        auto it = ch4::lower_bound(5, vs.begin(), vs.end());
        REQUIRE(it == vs.begin()+5);
    }

    SECTION("even elements find missing in middle") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

        auto it = ch4::lower_bound(5, vs.begin(), vs.end());
        REQUIRE(it == vs.begin()+5);
    }

    SECTION("odd elements") {
        std::vector<int> vs = { 0, 1, 2, 3, 4 };

        for (int i = 0; i < 5; ++i) {
            auto r = ch4::bsearch(i, vs.begin(), vs.end());
            REQUIRE(*r == i);
            REQUIRE(r == vs.begin()+i);
        }

        auto hi = ch4::lower_bound(5, vs.begin(), vs.end());
        REQUIRE(hi == vs.begin() + 4);

        auto hi2 = ch4::bsearch(5, vs.begin(), vs.end());
        REQUIRE(hi2 == vs.end());


        auto lo = ch4::lower_bound(-1, vs.begin(), vs.end());
        REQUIRE(lo == vs.begin());

        auto lo2 = ch4::bsearch(-1, vs.begin(), vs.end());
        REQUIRE(lo2 == vs.end());
    }
}
