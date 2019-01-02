#include "catch.hpp"
#include <vector>
#include <algorithm>
#include <cassert>


namespace ch4 {

// Lesson Learned: Must specify the problem correctly!
//   +Use half closed interval [first, last)
//   +Get return values of lower_bound correct so the natural implementation falls out
//   +Can build binary search from lower_bound

// Return first element that is NOT less than :value:, else last
// if no such element is found.
template <typename T, typename Iter>
Iter lower_bound(Iter first, Iter last, const T& value, std::forward_iterator_tag) noexcept
{
    // NOTE(peter): not the most efficient version possible, just doing linear scan.
    // can instead use std::distance and do less comparisons
    while (first < last) {
        if (!(*first < value)) {
            break;
        }
        ++first;
    }
    return first;
}

template <typename T, typename Iter>
Iter lower_bound(Iter first, Iter last, const T& value, std::bidirectional_iterator_tag) noexcept
{
    // TODO(peter): implement, not sure that I'm going to bother implementing this
    // NOTE(peter): it's annoying that RAI doesn't match this tag.
    // NOTE(peter): g++ STL does not break these out by concept, but instead uses std::distance() and std::advance()
    return first;
}

template <typename T, typename Iter>
Iter lower_bound(Iter first, Iter last, const T& value, std::random_access_iterator_tag) noexcept
{
    while (first < last) {
        auto mid = (last - first) / 2 + first;
        if (value < *mid) {
            last = mid;
        } else if (*mid < value) {
            first = mid + 1;
        } else {
            return mid;
        }
    }
    return value < *first ? first : last;
}

template <typename T, typename Iter>
Iter lower_bound(Iter first, Iter last, const T& value) noexcept
{
    using IterCategory = typename std::iterator_traits<Iter>::iterator_category;
    return lower_bound(first, last, value, IterCategory{});
}

template <class T, typename Iter>
Iter bsearch(Iter first, Iter last, const T& value) {
    auto found = ch4::lower_bound(first, last, value);
    return found != last && *found == value ? found : last;
}

} // ~namespace ch4

TEST_CASE("Binary Search", "bsearch")
{
    SECTION("even elements") {
        std::vector<int> vs = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; ++i) {
            auto r1 = ch4::bsearch(vs.begin(), vs.end(), i);
            auto r2 = ch4::lower_bound(vs.begin(), vs.end(), i);
            REQUIRE(r1 == r2);
            REQUIRE(*r1 == i);
            REQUIRE(r1 == vs.begin()+i);
        }

        // If value is greater than max element, should return last
        auto not_found_too_hi_lb = ch4::lower_bound(vs.begin(), vs.end(), 4);
        REQUIRE(not_found_too_hi_lb == vs.end());
        REQUIRE(ch4::bsearch(vs.begin(), vs.end(), 4) == vs.end());

        // If value is lower than min element, should return first
        auto not_found_too_lo_lb = ch4::lower_bound(vs.begin(), vs.end(), -1);
        REQUIRE(not_found_too_lo_lb == vs.begin());
        REQUIRE(ch4::bsearch(vs.begin(), vs.end(), -1) == vs.end());
    }

    SECTION("odd elements find missing in middle") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9, 10 };
        auto it = ch4::lower_bound(vs.begin(), vs.end(), 5);
        REQUIRE(*it == 6);
        REQUIRE(it == vs.begin()+6);
        auto stl_it = std::lower_bound(vs.begin(), vs.end(), 5);
        REQUIRE(it == stl_it);
    }

    SECTION("even elements find missing in middle") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
        auto it = ch4::lower_bound(vs.begin(), vs.end(), 5);
        REQUIRE(*it == 6);
        REQUIRE(it == vs.begin()+6);
        auto stl_it = std::lower_bound(vs.begin(), vs.end(), 5);
        REQUIRE(it == stl_it);
    }

    SECTION("odd elements") {
        std::vector<int> vs = { 0, 1, 2, 3, 4 };
        for (int i = 0; i < 5; ++i) {
            auto r1 = ch4::bsearch(vs.begin(), vs.end(), i);
            auto r2 = ch4::lower_bound(vs.begin(), vs.end(), i);
            REQUIRE(r1 == r2);
            REQUIRE(*r1 == i);
            REQUIRE(r1 == vs.begin()+i);
        }

        // If value is greater than max element, should return last
        auto not_found_too_hi_lb = ch4::lower_bound(vs.begin(), vs.end(), 5);
        REQUIRE(not_found_too_hi_lb == vs.end());
        REQUIRE(ch4::bsearch(vs.begin(), vs.end(), 5) == vs.end());

        // If value is less than min element, should return first
        auto lo = ch4::lower_bound(vs.begin(), vs.end(), -1);
        REQUIRE(lo == vs.begin());
        REQUIRE(ch4::bsearch(vs.begin(), vs.end(), -1) == vs.end());
    }
}
