#include "catch.hpp"
#include <vector>
#include <utility>

namespace ch5 {

std::pair<int, int> maxsubseq(std::vector<int> vs) {
    int maxsofar = 0;
    int lo = 0;
    int hi = 0;
    const int n = vs.size();

    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = i; j < n; ++j) {
            sum += vs[j];
            if (sum > maxsofar) {
                lo = i; hi = j; maxsofar = sum;
            }
        }
    }

    return std::make_pair(lo, hi);
}

} // ch5

TEST_CASE("Max subsequence", "[mss]")
{
    std::vector<int> vs = {
        31, -41, 59, 26, -53, 58, 97, -93, -23, 84,
    };

    auto result = ch5::maxsubseq(vs);
    REQUIRE(result.first  == 2);
    REQUIRE(result.second == 6);
}
