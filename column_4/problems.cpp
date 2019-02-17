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

// NOTE(peter): this is how he sets it up in the book
// but the half open interval is definitely the way to
// go.
int bsearch2(int t, int *x, int n)
{
    /*
     * precondition: x[0] <= x[1] <= ... <= x[n-1]
     * postcondition:
     *     result == -1    => t not present in x
     *     0 <= result < n => x[result] == t
     */


    // auto&& mustbe = [x, t](int lo, int hi) {
    //     for (int i = lo; i <= hi; ++i) {
    //         if (x[i] == t)
    //             return;
    //     }
    //     assert(false && "invariant violated");
    // };

    int l = 0;
    int u = n - 1;
    int m;
    while (l <= u) {
        // mustbe(l, u);
        // m = (l + u) / 2; // l + ((u - l) / 2)
        m = l + ((u - l) / 2);
        if (x[m] < t) {
            l = m + 1;
            // mustbe(l, u);
        } else if (x[m] == t) {
            return m;
        } else { // x[m] > t
            u = m - 1;
            // mustbe(l, u);
        }
    }
    return -1;
}

namespace recursive {

int go(int t, int *x, int l, int u)
{
    if (l > u)        return -1;
    int m = l + ((u - l) / 2);
    if (x[m] < t)       return go(t, x, m+1, u);
    else if (x[m] == t) return m;
    else                return go(t, x, l, m-1);
}

} // namespace recursive

int bsearch_recursive(int t, int *x, int n) { return recursive::go(t, x, 0, n-1); }

int exp(int x, int n)
{
    // pre:  n >= 0
    // post: result = x^n
    auto&& square = [](int x) { return x*x; };
    if (n == 0)
        return 1;
    else if (n % 2 == 0)
        return square(exp(x, n/2));
    else
        return x*exp(x, n-1);
}

int exp2(int x, int n)
{
    int result = x;
    int i;
    printf("exp2(x=%d, n=%d)\n", x, n);
    for (i = 1; i <= n;) {
        printf("\ti=%d, result=%d\n", i, result);
        if (i % 2 == 1) {
            result *= x;
            i = i + 1;
        } else {
            result *= result;
            i = i * 2;
        }
    }
    printf("\tloop exit: i=%d, result=%d\n", i, result);
    // TODO: i think this can just be 1 loop iteration max
    while (i-- > n) {
        result /= x;
    }
    printf("exp2(x=%d, n=%d) = %d\n\n", x, n, result);
    return result;
}

} // ~namespace ch4

TEST_CASE("Exp", "recursive")
{
    std::vector<std::tuple<int, int, int>> vs = {
        // x,  n, expected
        {  2,  2,    4 },
        {  3,  3,   27 },
        {  4,  4,  256 },
        {  2,  4,   16 },
        {  2,  5,   32 },
        {  2,  6,   64 },
        {  2,  7,  128 },
        {  2,  8,  256 },
        {  2,  9,  512 },
        {  2, 10, 1024 },
    };

    for (auto&& t : vs) {
        auto x = std::get<0>(t);
        auto n = std::get<1>(t);
        auto expected = std::get<2>(t);
        auto result = ch4::exp(x, n);
        auto r2 = ch4::exp2(x, n);
        REQUIRE(result == expected);
        REQUIRE(r2 == expected);
    }
}

TEST_CASE("Binary Search #2", "c bsearch")
{
    SECTION("odd elements") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        for (auto it : vs) {
            int i = ch4::bsearch2(it, vs.data(), vs.size());
            int i2 = ch4::bsearch_recursive(it, vs.data(), vs.size());
            REQUIRE(vs[i] == it);
            REQUIRE(vs[i2] == it);
            REQUIRE(i == i2);
        }
        int i = ch4::bsearch2(42, vs.data(), vs.size());
        int i2 = ch4::bsearch_recursive(42, vs.data(), vs.size());
        REQUIRE(i == -1);
        REQUIRE(i2 == -1);
    }

    SECTION("even elements") {
        std::vector<int> vs = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        for (auto it : vs) {
            int i = ch4::bsearch2(it, vs.data(), vs.size());
            int i2 = ch4::bsearch_recursive(it, vs.data(), vs.size());
            REQUIRE(vs[i] == it);
            REQUIRE(vs[i2] == it);
            REQUIRE(i == i2);
        }
        int i = ch4::bsearch2(42, vs.data(), vs.size());
        int i2 = ch4::bsearch_recursive(42, vs.data(), vs.size());
        REQUIRE(i == -1);
        REQUIRE(i2 == -1);
    }
}

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

namespace ch4 {

void print_beans(const std::vector<char>& bs)
{
//     for (auto b: bs) {
//         printf("%c ", b);
//     }
//     printf("\n");
}

void simulate(std::vector<char> beans)
{
    // while (!beans.empty()) {
    while (beans.size() >= 2) {
        print_beans(beans);
        const auto N = beans.size();
        auto b1 = beans.back(); beans.pop_back();
        auto b2 = beans.back(); beans.pop_back();
        if (b1 == b2) {
            beans.push_back('B');
        } else {
            beans.push_back('W');
        }
        assert(beans.size() == N-1); // decreased # of beans on each iteration
    }

    print_beans(beans);
}

} // ~namespace ch4

TEST_CASE("Chapter 4 Problem #6", "Coffee Can Problem")
{
    char W = 'W';
    char B = 'B';
    auto&& make_bag = [](int white, int black) {
        std::vector<char> out;
        for (int i = 0; i < white; ++i) {
            out.push_back('W');
        }
        for (int i = 0; i < black; ++i) {
            out.push_back('B');
        }
        return out;
    };
    auto beans = make_bag(3, 4);
    ch4::simulate(beans);
}


namespace ch4 {

using Line  = std::pair<double, double>;

template <class Iter>
std::pair<Iter, Iter> find_line(Iter first, Iter last, const double x, const double y)
{
    std::vector<double> ys;
    ys.reserve(last - first);
    for (auto it = first; it != last; ++it) {
        // y_i = a_i*x + b_i
        ys.push_back(it->first*x + it->second);
        // printf("%0.1f, %0.1f -> %0.1f\n", it->first, it->second, ys.back());
    }

    assert(std::is_sorted(ys.begin(), ys.end()));
    auto lo1 = std::upper_bound(ys.begin(), ys.end(), y);
    auto hi1 = std::lower_bound(ys.begin(), ys.end(), y);

    auto lo = lo1 != ys.end() ? first + (lo1 - ys.begin()) : last;
    auto hi = hi1 != ys.end() ? first + (hi1 - ys.begin()) : last;

    // auto&& compare = [x](const Line& line, double y) {
    //     return (line.first*x + line.second) < y;
    // };
    // auto lo = std::lower_bound(first, last, y, compare);
    // auto hi = std::upper_bound(first, last, y, compare);
    return std::make_pair(lo, hi);
}


} // ~namespace ch4

TEST_CASE("Chapter 4 Problem #7", "Lines")
{
    std::vector<ch4::Line> lines = {
        {  1., 0.  },
        {  2., 1.  },
        { -1., 4.5 },
    };
    auto res = ch4::find_line(lines.begin(), lines.end(), 0.5, 1.0);
//     if (res.first != lines.end()) {
//         printf("Lower: (%0.1f, %0.1f)\n", res.first->first, res.first->second);
//     } else {
//         printf("Lower: (unk, unk)\n");
//     }
//     if (res.second != lines.end()) {
//         printf("Upper: (%0.1f, %0.1f)\n", res.second->first, res.second->second);
//     } else {
//         printf("Lower: (unk, unk)\n");
//     }
}
