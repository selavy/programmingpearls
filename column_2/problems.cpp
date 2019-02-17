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

    auto&& contains = [](const Dictionary& ws, std::string w) {
        return std::find(ws.begin(), ws.end(), w) != ws.end();
    };

    using Anagrams = std::vector<std::string>;

    // With no pre-processing
    auto&& find_anagrams1 = [&](std::string word) -> Anagrams {
        Anagrams anagrams;
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

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last,
        std::bidirectional_iterator_tag) noexcept(noexcept(std::iter_swap(first, last)))
{
    while ((first != last) && (first != --last)) {
        std::iter_swap(first++, last);
    }
}


template <typename RandomAccessIt>
void reverse(RandomAccessIt first, RandomAccessIt last,
        std::random_access_iterator_tag) noexcept(noexcept(std::iter_swap(first, last)))
{
    if (first == last) {
        return;
    }
    while (first < --last) {
        std::iter_swap(first++, last);
    }
}

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last) noexcept(noexcept(std::iter_swap(first, last)))
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

template <class BidirectionalIt>
void rotate(BidirectionalIt first, BidirectionalIt n_first, BidirectionalIt last) noexcept(noexcept(my::reverse(first, n_first)))
{
    my::reverse(first, n_first);
    my::reverse(n_first, last);
    my::reverse(first, last);
}

template<typename Iter>
Iter __rotate(Iter first, Iter middle, Iter last) noexcept
{

    using Distance  = typename std::iterator_traits<Iter>::difference_type;
    using ValueType = typename std::iterator_traits<Iter>::value_type;

    Distance n = last   - first;
    Distance k = middle - first;

    Iter p = first;
    Iter ret = first + (last - middle);

    for (;;) {
        if (k < n - k) {
            Iter q = p + k;
            for (Distance i = 0; i < n - k; ++i) {
                std::iter_swap(p, q);
                ++p;
                ++q;
            }
            n %= k;
            if (n == 0) {
                return ret;
            }
            std::swap(n, k);
            k = n - k;
        } else {
            k = n - k;
            Iter q = p + n;
            p = q - k;
            for (Distance i = 0; i < n - k; ++i) {
                --p;
                --q;
                std::iter_swap(p, q);
            }
            n %= k;
            if (n == 0) {
                return ret;
            }
            std::swap(n, k);
        }
    }
}

template <class I>
I gcd(I m, I n) noexcept
{
    while (n != 0) {
        I t = m % n;
        m = n;
        n = t;
    }
    return m;
}

// NOTE(peter): Only works for random access iterators
template <class It>
void rotate2(It first, It n_first, It last) noexcept
{
    using Distance = typename std::iterator_traits<It>::difference_type;
    using ValueType = typename std::iterator_traits<It>::value_type;

    Distance n = last - first;
    Distance i = n_first - first;
    Distance cycles = gcd(i, n);
    for (Distance u = 0; u < cycles; ++u) {
        Distance j = i + u;
        Distance k = j + i;
        Distance z = k % n;
        ValueType t = *(first + u);
        *(first + u) = *(first + j);
        do {
            *(first + j) = *(first + z);
            j = z;
            k += i;
            z = k % n;
        } while (z != u);
        *(first + j) = t;
    }
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
        std::rotate(v1.begin(), v1.begin()+i, v1.end());
        REQUIRE(v1[0] == vs[i]);

        auto v2 = vs;
        my::rotate(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);

        v2 = vs;
        my::rotate2(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate less")
    {
        std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 1;
        auto v1 = vs;
        std::rotate(v1.begin(), v1.begin()+i, v1.end());
        REQUIRE(v1[0] == vs[i]);

        auto v2 = vs;
        my::rotate(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);

        v2 = vs;
        my::rotate2(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);
    }

    SECTION("Rotate noop")
    {
        std::vector<int> vs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int i = 0;
        auto v1 = vs;
        std::rotate(v1.begin(), v1.begin()+i, v1.end());
        REQUIRE(v1[0] == vs[i]);

        auto v2 = vs;
        my::rotate(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);

        v2 = vs;
        my::rotate2(v2.begin(), v2.begin()+i, v2.end());
        REQUIRE(v2[0] == vs[i]);
        REQUIRE(v1 == v2);
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


namespace my {

template <class Iter>
void nonadj_rotate(Iter first, Iter first_last, Iter second, Iter second_last) noexcept
{
    my::reverse(first, first_last);
    my::reverse(first_last, second);
    my::reverse(second, second_last);
    my::reverse(first, second_last);
}

} // ~namespace my

TEST_CASE("Problem #5", "More rotate")
{
    // Vector rotate functions change the vector ab to ba; how would you transform
    // the vector abc to cba? (This models the problem of swapping nonadjacent blocks
    // of memory.)

    // (abc)^4 = (a^r b^r c^r)^r

    std::vector<int> v        = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> expected = { 7, 8, 9, 4, 5, 6, 1, 2, 3 };
    my::nonadj_rotate(v.begin(), v.begin()+3, v.begin()+6, v.end());
    REQUIRE(v == expected);
}

namespace my {

// NOTE(peter): for some reason in his diagram, 'Q' and 'Z' are not
//              present...
//              Otherwise could do sign(char c) = (upper(c) - 65) // 3

const std::map<char, char> ButtonMap = {
    { 'A', '2'},
    { 'B', '2'},
    { 'C', '2'},

    { 'D', '3'},
    { 'E', '3'},
    { 'F', '3'},

    { 'G', '4'},
    { 'H', '4'},
    { 'I', '4'},

    { 'J', '5'},
    { 'K', '5'},
    { 'L', '5'},

    { 'M', '6'},
    { 'N', '6'},
    { 'O', '6'},

    { 'P', '7'},
    { 'R', '7'},
    { 'S', '7'},

    { 'T', '8'},
    { 'U', '8'},
    { 'V', '8'},

    { 'W', '9'},
    { 'X', '9'},
    { 'Y', '9'},
};

[[nodiscard]]
std::string telesign(std::string first, std::string last) noexcept
{
    std::string rv;
    for (auto c: last) {
        auto found = ButtonMap.find(std::toupper(c));
        if (found != ButtonMap.end()) {
            rv += found->second;
        }
    }
    rv += '*';

    for (auto c: first) {
        auto found = ButtonMap.find(std::toupper(c));
        if (found != ButtonMap.end()) {
            rv += found->second;
        }
        break;
    }
    rv += '*';
    return rv;
}


const static std::map<char, char> ButtonMap2 = {
    { 'A', '2'},
    { 'B', '2'},
    { 'C', '2'},

    { 'D', '3'},
    { 'E', '3'},
    { 'F', '3'},

    { 'G', '4'},
    { 'H', '4'},
    { 'I', '4'},

    { 'J', '5'},
    { 'K', '5'},
    { 'L', '5'},

    { 'M', '6'},
    { 'N', '6'},
    { 'O', '6'},

    { 'P', '7'},
    { 'Q', '7'},
    { 'R', '7'},
    { 'S', '7'},

    { 'T', '8'},
    { 'U', '8'},
    { 'V', '8'},

    { 'W', '9'},
    { 'X', '9'},
    { 'Y', '9'},
    { 'Z', '9'},
};

[[nodiscard]]
std::string telesign2(std::string first, std::string last) noexcept
{
    std::string rv;
    for (auto c: last) {
        auto found = ButtonMap2.find(std::toupper(c));
        if (found != ButtonMap2.end()) {
            rv += found->second;
        }
    }
    rv += '*';

    for (auto c: first) {
        auto found = ButtonMap2.find(std::toupper(c));
        if (found != ButtonMap2.end()) {
            rv += found->second;
        }
        break;
    }
    rv += '*';
    return rv;
}

const char ButtonMap3[64] = {
    '0', '2', '2', '2', '3', '3', '3', '4',
    '4', '4', '5', '5', '5', '6', '6', '6',
    '7', '7', '7', '7', '8', '8', '8', '9',
    '9', '9', '9', '0', '0', '0', '0', '0',
    '0', '2', '2', '2', '3', '3', '3', '4',
    '4', '4', '5', '5', '5', '6', '6', '6',
    '7', '7', '7', '7', '8', '8', '8', '9',
    '9', '9', '9', '0', '0', '0', '0', '0',
};

[[nodiscard]]
char Lookup3(char c) noexcept
{
    return ButtonMap3[(c & 0x7F) - 64];
}

[[nodiscard]]
std::string telesign3(std::string first, std::string last) noexcept
{
    std::string rv;
    rv.reserve(last.size() + 3);
    for (auto c: last) {
        rv += Lookup3(c);
    }
    rv += '*';
    if (!first.empty()) {
        rv += Lookup3(first[0]);
    }
    rv += '*';
    return rv;
}

} // ~namespace my

TEST_CASE("Problem #6", "Push-button telephone")
{
    // In the last 1970's, Bell Labs deployed a "user-operated directory assistance" pro-
    // gram that allowed employees to look up a number in a company telephony direc-
    // tory using a standard push-button telephone.
    //
    // To find the number of the designer of the system, Mike Lesk, one pressed
    // "LESK*M*" (that is, "5375*6*") and the system spoke his number. Such ser-
    // vices are now ubiquitous. One problem that arises in such systems is that different
    // names may have the same push-button encoding; when this happens in Lesk's sys-
    // tem, is asks the user for more information. Given a large file of names, such as a
    // standard metropolitan telephone directory, how would you locate these "false
    // matches"? (When Lesk did this experiment on such telephone directories, he
    // found that the incidence of false matches was just 0.2 percent.) How would you
    // implement the function that is given a push-button encoding of a name and returns
    // the set of possible matching names?

    using Name = std::pair<std::string, std::string>;
    using Directory = std::vector<Name>;
    const Directory directory = {
        { "Mike", "Lesk" },
        { "Peter", "Lesslie"},
        { "Luke", "Lesslie"},
        { "Paul", "Peters"},
        { "Jordan", "Kane"},
        { "Joe", "Smith"},
        { "Jane", "Smith"},
        { "Jake", "Gipple"},
    };

    using Cache = std::map<std::string, std::vector<Name>>;
    Cache cache;
    for (auto&& c: directory) {
        const auto sig = my::telesign(c.first, c.second);
        cache[sig].push_back(c);
    }

    using Results = std::vector<Name>;
    auto&& lookup = [&cache](const std::string& sig) -> Results {
        auto&& found = cache.find(sig);
        return found == cache.end() ? Results{} : found->second;
    };

    REQUIRE(lookup("5375*6*") == Results{Name{"Mike", "Lesk"}});
    REQUIRE(lookup("5377543*7*") == Results{Name{"Peter", "Lesslie"}});
}

TEST_CASE("Problem #6 Extended", "Fast Test Cases")
{
    std::vector<std::pair<std::string, std::string>> test_cases = {
        { "Mike", "Lesk" },
        { "Peter", "Lesslie" },
        { "Mike", "Pall" },
        { "George", "Bush" },
        { "Bill", "Clinton" },
        { "Donald", "Trump" },
        { "Barack", "Obama" },
    };

    for (auto&& name: test_cases) {
        // std::cout << "Checking case " << name.first << " " << name.second << "\n";
        auto result1 = my::telesign2(name.first, name.second);
        auto result2 = my::telesign3(name.first, name.second);
        REQUIRE(result1 == result2);
    }
}
