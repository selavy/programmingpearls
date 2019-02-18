#include "catch.hpp"
#include <vector>
#include <utility>

namespace ch5 {

int mss1(std::vector<int> vs) {
    int maxsofar = 0;
    const int n = vs.size();
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = i; j < n; ++j) {
            sum += vs[j];
            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }
    return maxsofar;
}

int mss2(std::vector<int> vs) {
    int maxsofar = 0;
    const int n = vs.size();
    std::vector<int> cumarr(vs.size() + 1, 0);
    assert(cumarr.size() == vs.size() + 1);
    cumarr[0] = 0;
    for (int i = 1; i < vs.size(); ++i) {
        cumarr[i] = cumarr[i-1] + vs[i];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            int sum = cumarr[j-1] - cumarr[i];
            maxsofar = std::max(maxsofar, sum);
        }
    }
    return maxsofar;
}

template <class Iter>
int mss3(Iter first, Iter last) {
    int size = std::distance(first, last);
    if (size == 0)
        return 0;
    else if (size == 1)
        return std::max(*first, *(first + 1));

    auto mid = first + (size / 2);
    int lmax = 0;
    int rmax = 0;
    int sum = 0;
    for (auto it = mid - 1; it >= first; --it) {
        sum += *it;
        lmax = std::max(lmax, sum);
    }
    sum = 0;
    for (auto it = mid; it < last; ++it) {
        sum += *it;
        rmax = std::max(rmax, sum);
    }
    return std::max(std::max(lmax+rmax, mss3(first, mid)), mss3(mid, last));
}

int mss4(std::vector<int> vs) {
    const int n = vs.size();
    int maxsofar = 0;
    int maxendinghere = 0;
    for (int i = 0; i < n; ++i) {
        // invariant: maxendinghere is accurate for x[0..i-1]
        // invariant: maxsofar      is accurate for x[0..i-1]
        maxendinghere = std::max(maxendinghere + vs[i], 0);
        maxsofar      = std::max(maxsofar, maxendinghere);
    }
    return maxsofar;
}

} // ch5

TEST_CASE("Max subsequence", "[mss]")
{
    std::vector<int> vs = {
        31, -41, 59, 26, -53, 58, 97, -93, -23, 84,
    };

    auto r1 = ch5::mss1(vs);
    REQUIRE(r1 == (59 + 26 + -53 + 58 + 97));
    auto r2 = ch5::mss2(vs);
    auto r3 = ch5::mss3(vs.begin(), vs.end());
    auto r4 = ch5::mss4(vs);
    REQUIRE(r1 == r2);
    REQUIRE(r1 == r3);
    REQUIRE(r1 == r4);
}
