#include "catch.hpp"
#include <vector>
#include <string>
#include <algorithm>


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

    auto&& contains = [](const std::vector<std::string> &ws, std::string w) {
        return std::find(ws.begin(), ws.end(), w) != ws.end();
    };
    
    // With no pre-processing
    auto&& find_anagrams1 = [&](std::string word) {
        std::vector<std::string> anagrams;
        // const std::string word = "deposit";
        const std::string sig = sign(word);
        for (auto&& w: dict) {
            const std::string sig2 = sign(w);
            if (sig == sig2) {
                anagrams.push_back(w);
            }
        }
        return anagrams;
    };

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
