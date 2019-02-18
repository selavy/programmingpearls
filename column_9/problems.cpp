#include "catch.hpp"
#include <vector>
#include <utility>
#include <climits>
#include <algorithm>
#include <random>

namespace ch9 {

int find_max(std::vector<int> vs) {
    int N = vs.size();
    int hold = vs[N-1];
    vs[N-1] = INT_MAX;
    int result = INT_MIN;
    for (int i = 0; ; ++i) {
        assert(i < N && "sentinel failed!");
        if (vs[i] > result) {
            if (vs[i] == INT_MAX) {
                break;
            }
            result = vs[i];
        }
    }
    if (hold > result) {
        result = hold;
    }
    vs[N-1] = hold;
    return result;
}

} // ch9

TEST_CASE("Maximum element", "[me]")
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int N = 20;
    std::vector<int> vs(N, 0);
    REQUIRE(vs.size() == N);
    std::generate(vs.begin(), vs.end(), gen);
    // vs.reserve(N);
    // std::generate_n(std::back_inserter(vs), N, gen);

    for (int i = 0; i < 100; ++i) {
        int result = ch9::find_max(vs);
        int expect = *std::max_element(vs.begin(), vs.end());
        REQUIRE(result == expect);
    }
}

