#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <memory>

constexpr int MaxValue = 10000000;
constexpr int Elements = MaxValue / 2; // using 4-bits per number

struct {
    uint8_t lo:4; // even
    uint8_t hi:4; // odd
} cnts[MaxValue / 2];


int main(int argc, char** argv) {
    char input[256];
    memset(&cnts[0], 0, sizeof(cnts));
    while (fgets(&input[0], sizeof(input), stdin) != NULL) {
        const int val = atoi(&input[0]);
        const int idx = val / 2;
        if (val % 2 == 0) {
            ++cnts[idx].lo;
            assert(cnts[idx].lo <= 10 && "More than 10 duplicates!");
        } else {
            ++cnts[idx].hi;
            assert(cnts[idx].hi <= 10 && "More than 10 duplicates!");
        }
    }

    for (int i = 0; i < MaxValue/2; ++i) {
        for (int p = 0; p < cnts[i].lo; ++p) {
            printf("%d\n", 2*i);
        }
        for (int p = 0; p < cnts[i].hi; ++p) {
            printf("%d\n", 2*i+1);
        }
    }

    return 0;
}
