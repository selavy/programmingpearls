// 1. If memory were not scarce, how would you implement a sort in a language with
//    libraries for representing and sorting sets?

#include <set>
#include <algorithm>
#include "catch.hpp"

TEST_CASE("Sort using set factilities", "[column1_1]") {
    std::vector<int> input = { 1, 3, 2, 5, 9, 27, 43, 101, 22 };
    std::set<int> s;
    for (auto&& i: input) {
        s.insert(i);
    }
    std::vector<int> output;
    output.reserve(input.size());
    for (auto&& i: s) {
        output.push_back(i);
    }

    auto&& in_input = [&input](int i) {
        return std::find(input.begin(), input.end(), i) != input.end();
    };
    
    auto&& in_output = [&output](int i) {
        return std::find(output.begin(), output.end(), i) != output.end();
    };

    // NOTE(peter): minimal testing for my "in_(input|output)" methods
    REQUIRE(in_input(222) == false);
    REQUIRE(in_input(22) == true);
    REQUIRE(in_output(222) == false);
    REQUIRE(in_output(22) == true);

    REQUIRE(std::is_sorted(input.begin(), input.end()) == false);    
    REQUIRE(std::is_sorted(output.begin(), output.end()) == true);

    for (auto&& i: input) {
        REQUIRE(in_output(i) == true);
    }
    for (auto&& i: output) {
        REQUIRE(in_input(i) == true);
    }
}
