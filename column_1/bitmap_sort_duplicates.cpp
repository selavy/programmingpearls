#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <memory>

constexpr int MaxValue = 10000000;
constexpr int Elements = MaxValue / 2; // using 4-bits per number

// #define DESIGN_1
// #define DESIGN_2
#define DESIGN_3

#ifdef DESIGN_1

struct EE {
    uint8_t lo:4; // even
    uint8_t hi:4; // odd
};
static EE cnts[Elements];

#elif defined(DESIGN_2) || defined(DESIGN_3)

static uint8_t cnts[Elements];

#else

    #error "Must pick a design"

#endif


int main(int argc, char** argv) {
    char input[256];
    memset(&cnts[0], 0, sizeof(cnts));

    while (fgets(&input[0], sizeof(input), stdin) != NULL) {
        const int val = atoi(&input[0]);
        const int idx = val / 2;

#ifdef DESIGN_1
        if (val % 2 == 0) {
            ++cnts[idx].lo;
            assert(cnts[idx].lo <= 10 && "More than 10 duplicates!");
        } else {
            ++cnts[idx].hi;
            assert(cnts[idx].hi <= 10 && "More than 10 duplicates!");
        }
#elif defined(DESIGN_2)
        if (val % 2 == 0) {
            cnts[idx] += 1u << 0;
        } else {
            cnts[idx] += 1u << 4;
        }
        assert(((cnts[idx] >> 0) & 0x0F) <= 10 && "More than 10 duplicates!");
        assert(((cnts[idx] >> 4) & 0x0F) <= 10 && "More than 10 duplicates!");
#elif defined(DESIGN_3)
        uint8_t lo = (cnts[idx] >> 0) & 0x0F;
        uint8_t hi = (cnts[idx] >> 4) & 0x0F;
        if (val % 2 == 0) {
            ++lo;
        } else {
            ++hi;
        }
        cnts[idx] = (hi << 4) | (lo << 0);
#endif
    }

    for (int i = 0; i < Elements; ++i) {

#ifdef DESIGN_1
        int lo = cnts[i].lo;
        int hi = cnts[i].hi;
#elif defined(DESIGN_2) || defined(DESIGN_3)
        int lo = (cnts[i] >> 0) & 0x0F;
        int hi = (cnts[i] >> 4) & 0x0F;
#endif
        for (int p = 0; p < lo; ++p) {
            printf("%d\n", 2*i);
        }
        for (int p = 0; p < hi; ++p) {
            printf("%d\n", 2*i+1);
        }
    }

    return 0;
}
