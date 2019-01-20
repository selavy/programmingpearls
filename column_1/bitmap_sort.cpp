#include <cstdio>
#include <cstdlib>
#include "bit_vector2.h"

constexpr size_t MaxValue = 10000000;

int main(int argc, char** argv) {
    BitVector2<MaxValue> vec;
    int val;
    char input[256];
    while (fgets(&input[0], sizeof(input), stdin) != NULL) {
        val = atoi(&input[0]);
        vec.set(val);
    }
    for (int i = 0; i < MaxValue; ++i) {
        if (vec.isset(i)) {
            printf("%d\n", i);
        }
    }
    return 0;
}
